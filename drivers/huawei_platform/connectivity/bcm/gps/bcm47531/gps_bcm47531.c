#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/consumer.h>
#include <linux/printk.h>
#include <huawei_platform/connectivity/huawei_gps.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/stat.h>
#include <linux/rfkill.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/workqueue.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/version.h>
#include <linux/workqueue.h>
#include <linux/unistd.h>
#include <linux/bug.h>
#include <linux/mutex.h>
#include <linux/pm_wakeup.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>

#ifdef CONFIG_HWCONNECTIVITY
#include <huawei_platform/connectivity/hw_connectivity.h>
#endif

#define DTS_COMP_GPS_POWER_NAME "huawei,gps_power"
#define GPS_REF_CLK_FREQ_19M   (19200000L)

#define GPS_STANDBY         0
#define GPS_ACTIVE          1

#define GPS_IC_TYPE_4752 4752
#define GPS_IC_TYPE_47531 47531
extern int get_gps_ic_type(void);

typedef struct gps_bcm_info {
	struct gpio gpioid_en;
	struct clk *clk;
	struct clk *refclk;
	struct pinctrl *pctrl;
	struct pinctrl_state *pins_normal;
	struct pinctrl_state *pins_idle;
	struct gpio gpioid_hostwake;
	bool is_hostwake;
} GPS_BCM_INFO;

static struct clk *gps_ref_clk;
static GPS_BCM_INFO *g_gps_bcm;

#define HOST_WAKE_MODULE_NAME "gps_geofence_wake"
struct gps_geofence_wake {
	/* irq from gpio_to_irq() */
	int irq;
	/* HOST_WAKE_GPIO */
	int host_req_pin;
	/* misc driver structure */
	struct miscdevice misc;
	/* wake_lock */
	struct wakeup_source wake_lock;
};
static struct gps_geofence_wake g_geofence_wake;
static int gps_geofence_wake_open(struct inode *inode, struct file *filp)
{
	pr_info("%s\n", __func__);
	return 0;
}

static int gps_geofence_wake_release(struct inode *inode, struct file *filp)
{
	pr_info("%s\n", __func__);
	return 0;
}

static int gps_geofence_wake_ioctl(struct file *filp,
				   unsigned int cmd, unsigned int arg)
{
	pr_info("%s\n", __func__);
	return 0;
}

static const struct file_operations gps_geofence_wake_fops = {
	.owner = THIS_MODULE,
	.open = gps_geofence_wake_open,
	.release = gps_geofence_wake_release,
	.unlocked_ioctl = gps_geofence_wake_ioctl
};

/* set/reset wake lock by HOST_WAKE level
 \param gpio the value of HOST_WAKE_GPIO */
static void gps_geofence_wake_lock(int gpio)
{
	struct gps_geofence_wake *ac_data = &g_geofence_wake;
	/* we need to use wake_lock_timeout instead of wake_unlock */
	__pm_wakeup_event(&ac_data->wake_lock, jiffies_to_msecs(5 * HZ));
}

static irqreturn_t gps_host_wake_isr(int irq, void *dev)
{
	struct gps_geofence_wake *ac_data = &g_geofence_wake;
	int gps_host_wake = ac_data->host_req_pin;
	char gpio_value = 0x00;

	pr_info("%s\n", __func__);

	gpio_value = gpio_get_value(gps_host_wake);

	/* wake_lock */
	gps_geofence_wake_lock(gpio_value);

	return IRQ_HANDLED;
}

/* initialize GPIO and IRQ
 \param gpio the GPIO of HOST_WAKE
 \return if SUCCESS, return the id of IRQ, if FAIL, return -EIO */
static int gps_gpio_irq_init(int gpio)
{
	int ret = 0;
	int irq = 0;

	pr_info("[gps]%s\n", __func__);
	/* 1. Set GPIO */
	if ((gpio_request(gpio, "gps_host_wake"))) {
		pr_info("[gps]Can't request HOST_REQ GPIO %d.It may be already registered\n",
			gpio);
		return -EIO;
	}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0))
	gpiod_export(gpio_to_desc(gpio), false);
#else
	gpio_export(gpio, false);
#endif
	gpio_direction_input(gpio);

	/* 2. Set IRQ */
	irq = gpio_to_irq(gpio);
	if (irq < 0) {
		pr_info("[gps]Could not get HOST_WAKE_GPIO = %d!, err = %d\n",
		       gpio, irq);
		gpio_free(gpio);
		return -EIO;
	}

	ret =
	    request_irq(irq, gps_host_wake_isr,
			IRQF_TRIGGER_RISING | IRQF_NO_SUSPEND, "gps_host_wake",
			NULL);
	if (ret) {
		pr_info("[gps]Request_host wake irq failed.\n");
		gpio_free(gpio);
		return -EIO;
	}

	ret = irq_set_irq_wake(irq, 1);

	return irq;
}

/* cleanup GPIO and IRQ */
static void gps_gpio_irq_cleanup(int gpio, int irq)
{
	pr_debug("[gps]%s\n", __func__);
	gpio_free(gpio);
	free_irq(irq, NULL);
}

static ssize_t gps_write_proc_nstandby(struct file *filp,
				       const char __user *buffer, size_t len,
				       loff_t *off)
{
	char gps_nstandby = '0';
	pr_info("[GPS] gps_write_proc_nstandby\n");

	if (buffer == NULL) {
		pr_err("[GPS]NULL Pointer to platform device\n");
		return -EINVAL;
	}
	if ((len < 1) || (g_gps_bcm == NULL)) {
		pr_err("[GPS] gps_write_proc_nstandby g_gps_bcm is NULL or read length = 0\n");
		return -EINVAL;
	}

	if (copy_from_user(&gps_nstandby, buffer, sizeof(gps_nstandby))) {
		pr_err("[GPS] gps_write_proc_nstandby copy_from_user failed!\n");
		return -EFAULT;
	}

	if (gps_nstandby == '0') {
		pr_info("[GPS] gps nstandby.\n");
		gpio_set_value(g_gps_bcm->gpioid_en.gpio, GPS_STANDBY);
	} else if (gps_nstandby == '1') {
		pr_info("[GPS] gps active.\n");
		gpio_set_value(g_gps_bcm->gpioid_en.gpio, GPS_ACTIVE);
	} else {
		pr_err("[GPS] gps nstandby write error code[%d].\n",
		       gps_nstandby);
	}

	return len;
}

static ssize_t gps_read_proc_nstandby(struct file *filp,
				      char __user *buffer, size_t len,
				      loff_t *off)
{
	int gps_nstandby = 0;
	char tmp[2] = {0};
	if (buffer == NULL) {
		pr_err("[GPS]NULL Pointer to platform device\n");
		return -EINVAL;
	}

	pr_info("[GPS] gps_read_proc_nstandby\n");
	if (len < 1 || g_gps_bcm == NULL) {
		pr_err("[GPS] gps_read_proc_nstandby g_gps_bcm is NULL or read length = 0.\n");
		return -EINVAL;
	}
	len = 1;
	pr_info("[GPS] gps_read_proc_nstandby off = %d\n",
	       (unsigned int)*off);
	if ((unsigned int)*off > 0) {
		return 0;
	}
	gps_nstandby = gpio_get_value(g_gps_bcm->gpioid_en.gpio);
	if (gps_nstandby  == 0) {
		tmp[0] = '0';
	} else if (gps_nstandby == 1) {
		tmp[0] = '1';
	}

	pr_info("[GPS] gps nstandby status[%s]\n", tmp);

	if (copy_to_user(buffer, tmp, len)) {
		pr_err("[GPS] gps_read_proc_nstandby copy_to_user failed!\n");
		return -EFAULT;
	}
	*off += len;
	return len;
}

static const struct file_operations gps_proc_fops_nstandby = {
	.owner = THIS_MODULE,
	.write = gps_write_proc_nstandby,
	.read = gps_read_proc_nstandby,
};

static int create_gps_proc_file(void)
{
	int ret = 0;
	struct proc_dir_entry *gps_dir = NULL;
	struct proc_dir_entry *gps_nstandby_file = NULL;
	gps_dir = proc_mkdir("gps", NULL);
	if (!gps_dir) {
		pr_err("[GPS] proc dir create failed\n");
		ret = -ENOMEM;
		return ret;
	}

	gps_nstandby_file =
	    proc_create("nstandby", S_IRUGO | S_IWUSR | S_IWGRP | S_IFREG,
			gps_dir, &gps_proc_fops_nstandby);
	if (!gps_nstandby_file) {
		pr_err("[GPS] proc nstandby file create failed\n");
		ret = -ENOMEM;
		return ret;
	}
	pr_info("[GPS] gps create proc file ok.\n");

	return ret;
}

static int k3_gps_bcm_probe(struct platform_device *pdev)
{
	if (pdev == NULL) {
		pr_err("[GPS]NULL Pointer to platform device\n");
		return -ENOMEM;
	}
	GPS_BCM_INFO *gps_bcm = NULL;
	struct device *gps_power_dev = &pdev->dev;
	struct device_node *np = gps_power_dev->of_node;
	enum of_gpio_flags gpio_flags;
	int ret = 0;
	int irq = 0;
	struct gps_geofence_wake *ac_data = &g_geofence_wake;

	pr_info("[GPS] start find gps_power and ic type is 47531\n");
	gps_bcm = kzalloc(sizeof(GPS_BCM_INFO), GFP_KERNEL);
	if (!gps_bcm) {
		pr_err("[GPS] Alloc memory failed\n");
		return -ENOMEM;
	}

	gps_bcm->gpioid_en.gpio = of_get_named_gpio(np, "huawei,gps_en", 0);

	if (!gpio_is_valid(gps_bcm->gpioid_en.gpio)) {
		pr_err("[GPS] get huawei,gps_en gpio failed.\n");
		ret = -1;
		goto err_free_gps;
	}

	ret = gpio_request(gps_bcm->gpioid_en.gpio, "gps_enbale");
	if (ret) {
		pr_err("[GPS] gpio_direction_output %d failed, ret:%d\n",
		       gps_bcm->gpioid_en.gpio, ret);
		goto err_free_gps_en;
	}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0))
	ret = gpiod_export(gpio_to_desc(gps_bcm->gpioid_en.gpio), false);
#else
	ret = gpio_export(gps_bcm->gpioid_en.gpio, false);
#endif
	if (ret)
		pr_err("[GPS] gpio_export %d failed, ret:%d\n",
		       gps_bcm->gpioid_en.gpio, ret);

	ret = gpio_direction_output(gps_bcm->gpioid_en.gpio, 0);
	if (ret) {
		pr_err("[GPS] gpio_direction_output %d failed, ret:%d\n",
		       gps_bcm->gpioid_en.gpio, ret);
		goto err_free_gps_en;
	}
	pr_info("[GPS] finish gpio_direction_output gps_power\n");

	ret = create_gps_proc_file();
	if (ret) {
		pr_err("[GPS] gps create proc file failed.\n");
		goto err_free_gps_en;
	}

	gps_bcm->gpioid_hostwake.gpio =
	    of_get_named_gpio(np, "huawei,gps_hostwake", 0);
	if (!gpio_is_valid(gps_bcm->gpioid_hostwake.gpio)) {
		ret = -1;
		pr_err("[GPS] get huawei,gps_hostwake failed.\n");
		goto err_free_gps_en;
	}
	/* 1. Init GPIO and IRQ for HOST_WAKE */
	pr_info("[gps]%s,gps_bcm->gpioid_hostwake.gpio=%d\n", __func__,
	       gps_bcm->gpioid_hostwake.gpio);

	/* 2. Register Driver */
	memset(ac_data, 0, sizeof(struct gps_geofence_wake));

	/* 2.1 Misc device setup */
	ac_data->misc.minor = MISC_DYNAMIC_MINOR;
	ac_data->misc.name = HOST_WAKE_MODULE_NAME;
	ac_data->misc.fops = &gps_geofence_wake_fops;

	/* 2.2 Information that be used later */
	ac_data->irq = irq;
	ac_data->host_req_pin = gps_bcm->gpioid_hostwake.gpio;

	pr_info("[gps]misc register, name %s, irq %d, host req pin num %d\n",
	       ac_data->misc.name, irq, ac_data->host_req_pin);
	/* 2.3 Register misc driver */
	ret = misc_register(&ac_data->misc);
	if (ret != 0) {
		pr_info("[gps]cannot register gps geofence wake miscdev on minor = %d ret = %d\n",
		     MISC_DYNAMIC_MINOR, ret);
		goto err_free_host_wake;
	}
	/* 3. Init wake_lock */
	wakeup_source_init(&ac_data->wake_lock,
		       "gps_geofence_wakelock");
	pr_info("[gps]wake_lock_init done\n");
	irq = gps_gpio_irq_init(gps_bcm->gpioid_hostwake.gpio);
	if (irq < 0) {
		pr_info("[gps]hostwake irq error\n");
		goto err_free_misc_register;
	}

	/* Set 32KC clock */
	gps_bcm->clk = of_clk_get_by_name(np, "gps_32k");
	if (IS_ERR(gps_bcm->clk)) {
		pr_err("[GPS] clk_32k get failed!\n");
		ret = -1;
		goto err_free_misc_register;
	}
	ret = clk_prepare_enable(gps_bcm->clk);
	if (ret) {
		pr_err("[GPS] clk_32k enable is failed\n");
		goto err_free_clk;
	}
	pr_info("[GPS] clk_32k is finished\n");

#if 1
	gps_bcm->pctrl = devm_pinctrl_get(gps_power_dev);
	if (IS_ERR(gps_bcm->pctrl)) {
		pr_err("[GPS] pinctrl get error!\n");
		ret = -1;
		goto err_free_clk;
	}

	gps_bcm->pins_normal = pinctrl_lookup_state(gps_bcm->pctrl, "default");
	if (IS_ERR(gps_bcm->pins_normal)) {
		pr_err("[GPS] gps_bcm->pins_normal lookup error!\n");
		ret = -1;
		goto err_free_pinctrl;
	}

	gps_bcm->pins_idle = pinctrl_lookup_state(gps_bcm->pctrl, "idle");
	if (IS_ERR(gps_bcm->pins_idle)) {
		pr_err("[GPS] gps_bcm->pins_idle lookup error!\n");
		ret = -1;
		goto err_free_pinctrl;
	}

	ret = pinctrl_select_state(gps_bcm->pctrl, gps_bcm->pins_normal);
	if (ret) {
		pr_err("[GPS] pinctrl_select_state error!\n");
		goto err_free_pinctrl;
	}
	pr_info("[GPS] pinctrl is finish\n");

	gps_bcm->refclk = of_clk_get_by_name(np, "clk_gps");
	if (IS_ERR(gps_bcm->refclk)) {
		pr_err("[GPS] @@@ref_clk get failed!\n");
		ret = -1;
		goto err_free_pinctrl;
	}

	ret = clk_set_rate(gps_bcm->refclk, GPS_REF_CLK_FREQ_19M);
	if (ret < 0) {
		pr_err("[GPS] clk_set_rate HI3635_CLK_RATE failed\n");
		goto err_free_refclk;
	}

	gps_ref_clk = gps_bcm->refclk;
#endif
	pr_info("[GPS] ref clk is finished!\n");
	platform_set_drvdata(pdev, gps_bcm);
	g_gps_bcm = gps_bcm;
	return 0;

err_free_refclk:
	clk_put(gps_bcm->refclk);
err_free_pinctrl:
	pinctrl_put(gps_bcm->pctrl);
err_free_clk:
	clk_put(gps_bcm->clk);
err_free_misc_register:
	misc_deregister(&ac_data->misc);
	wakeup_source_trash(&ac_data->wake_lock);
	pr_err("%s: misc_deregister!\n", __func__);
err_free_host_wake:
	gpio_free(gps_bcm->gpioid_hostwake.gpio);
	pr_err("%s: err_free_host_wake!\n", __func__);
err_free_gps_en:
	gpio_free(gps_bcm->gpioid_en.gpio);
err_free_gps:
	kfree(gps_bcm);
	gps_bcm = NULL;
	g_gps_bcm = NULL;
	return ret;
}

static void K3_gps_bcm_shutdown(struct platform_device *pdev)
{
	if (pdev == NULL) {
		pr_err("[GPS]NULL Pointer to platform device\n");
		return;
	}
	GPS_BCM_INFO *gps_bcm = platform_get_drvdata(pdev);

	pr_info("[GPS] K3_gps_bcm_shutdown!\n");

	if (!gps_bcm) {
		pr_err("[GPS] gps_bcm is NULL,just return.\n");
		return;
	}

	platform_set_drvdata(pdev, NULL);
	kfree(gps_bcm);
	gps_bcm = NULL;
	g_gps_bcm = NULL;
	return;
}

static const struct of_device_id gps_power_match_table[] = {
	{
	 .compatible = DTS_COMP_GPS_POWER_NAME, /* compatible must match with which defined in dts */
	 .data = NULL,
	 },
	{
	 },
};

MODULE_DEVICE_TABLE(of, gps_power_match_table);

static struct platform_driver k3_gps_bcm_driver = {
	.probe = k3_gps_bcm_probe,
	.suspend = NULL,
	.remove = NULL,
	.shutdown = K3_gps_bcm_shutdown,
	.driver = {
		   .name = "huawei,gps_power_47531",
		   .owner = THIS_MODULE,
		   .of_match_table = of_match_ptr(gps_power_match_table), /* dts required code */
		   },
};

static int __init k3_gps_bcm_init(void)
{
#ifdef CONFIG_HWCONNECTIVITY
	/* For OneTrack, we need check it's the right chip type or not.
	 If it's not the right chip type, don't init the driver */
	if (!isMyConnectivityChip(CHIP_TYPE_BCM)) {
		pr_err("gps chip type is not match, skip driver init");
		return -EINVAL;
	} else {
		pr_info("gps chip type is matched with Broadcom, continue");
	}
#endif

	if (get_gps_ic_type() != GPS_IC_TYPE_47531) {
		pr_info("gps chip type is matched with Broadcom, but is not 47531");
		return -EINVAL;
	}
	pr_info("gps chip type is matched with Broadcom, and it is 47531\n");
	return platform_driver_register(&k3_gps_bcm_driver);
}

static void __exit k3_gps_bcm_exit(void)
{
	platform_driver_unregister(&k3_gps_bcm_driver);
}

int set_gps_ref_clk_enable_bcm47531(bool enable)
{
	int ret = 0;

	pr_info("[GPS] set_gps_ref_clk_enable %d\n", enable);
	if (IS_ERR(gps_ref_clk)) {
		pr_err("[GPS] ERROR: refclk is invalid!\n");
		return -1;
	}

	if (enable) {
		ret = clk_prepare_enable(gps_ref_clk);
		if (ret < 0) {
			pr_err("[GPS] ERROR: refclk enable failed!\n");
			return -1;
		}
	} else {
		clk_disable_unprepare(gps_ref_clk);
	}
	pr_info("[GPS] set_gps_ref_clk_enable finish\n");

	return 0;
}

module_init(k3_gps_bcm_init);
module_exit(k3_gps_bcm_exit);

MODULE_AUTHOR("DRIVER_AUTHOR");
MODULE_DESCRIPTION("GPS Boardcom 47511 driver");
MODULE_LICENSE("GPL");
