#ifndef __HI_PMU_H__
#define __HI_PMU_H__ 
#define PMIC_VOLT_MIN 0
#define PMIC_VOLT_MAX 35
#define PMIC_VOLT_NUM (PMIC_VOLT_MAX - PMIC_VOLT_MIN + 1)
#define EPMU_INVALID 0xFFFFFFFF
#define THIRD_PMU_REPLICA_ID 2
#define SECONDARY_PMU_SALVE_ID 8
#define PRIMARY_PMU_SALVE_ID 9
#define BBP5G_PMU_ID (25)
#define HI_PMIC_ASIC_ADDR 0xe400c000
#define HI_M_SPMI_ADDR 0xc400c000
#define MDM_LR_SOC_ID 0
#define VOLT_UNIT_CONVERSION 1000
#ifdef CONFIG_PMIC_LCDIO_SUPPLY
#define PMIC_LCDIO_ENABLE_REG 0x14
#define PMIC_LCDIO_ENABLE_MASK 0x1
#define PMIC_LCDIO_ENABLE_VALUE 0x1
#define PMIC_LCDIO_DISABLE_REG 0x14
#define PMIC_LCDIO_DISABLE_MASK 0x1
#define PMIC_LCDIO_DISABLE_VALUE 0
#endif
#define PMIC_LCDANALOG_ENABLE_REG 0x26
#define PMIC_LCDANALOG_ENABLE_MASK 0x1
#define PMIC_LCDANALOG_ENABLE_VALUE 0x1
#define PMIC_LCDANALOG_DISABLE_REG 0x26
#define PMIC_LCDANALOG_DISABLE_MASK 0x1
#define PMIC_LCDANALOG_DISABLE_VALUE 0
#define PMIC_LCDANALOG_VOLT_REG 0x69
#define PMIC_LCDANALOG_VOLT_MASK 0x7
#define PMIC_LCDANALOG_VOLT_VALUE 0x3
#define PMIC_EFUSE_ENABLE_REG 0x18
#define PMIC_EFUSE_DISABLE_REG 0x18
#define PMIC_EFUSE_ENABLE_VALUE 1
#define PMIC_EFUSE_DISABLE_VALUE 0
#define PMIC_EFUSE_ENABLE_MASK 0x1
#define PMIC_EFUSE_DISABLE_MASK 0x1
#define PMIC_POWERKEY_STATE_REG 0x6
#define PMIC_POWERKEY_STATE_MASK 0x80
#define PMIC_USB_STATE_REG 0x6
#define PMIC_USB_STATE_MASK 0x2
#define PMIC_GPIO_IRQ_NUM 1
#define PMIC_GPIO_IRQ_NCP_BASE 0x2bd
#define PMIC_IRQ_NCP_BASE 0x2b3
#define PMIC_IRQ_NUM 2
#define PMIC_OCP_IRQ_NUM 6
#define PMIC_OCP_NCP_BASE 0x2c4
#define PMIC_SCP_IRQ_NUM 2
#define PMIC_RECORD_NUM 6
#define PMIC_RECORD_NCP_BASE 0x2cc
#define PMIC_HRESET_STATE_REG_RECINDEX 0
#define PMIC_HRESET_STATE_MASK 0x20
#define PMIC_HRESET_PWRDOWN_CTRL 0x2e7
#define PMIC_HRESET_STATE_REG 0x2cc
#define PMIC_OTP_MASK 0x10
#define PMIC_OVP_MASK 0x1
#define PMU_INT_OTMP 0
#define PMU_INT_USB_IN 1
#define PMU_INT_USB_OUT 2
#define PMU_INT_ALARM_INT 3
#define PMU_INT_POWER_KEY_6S_PRESS 4
#define PMU_INT_POWER_KEY_1S_PRESS 5
#define PMU_INT_POWER_KEY_20MS_RELEASE 6
#define PMU_INT_POWER_KEY_20MS_PRESS 7
#define PMU_INT_IRQ_OCP_SCP 8
#define PMU_INT_COUL 9
#define PMU_INT_SIM0_IN_RAISE 10
#define PMU_INT_SIM0_IN_FALL 11
#define PMU_INT_SIM1_OUT_RAISE 12
#define PMU_INT_SIM1_OUT_FALL 13
#define RAMP_UDELAY_UP_HOLD 75
#define RAMP_UDELAY_DOWN_HOLD 0
#endif
