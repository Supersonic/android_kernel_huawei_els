

/* 头文件包含 */
#include "oal_workqueue.h"

#include "oal_ext_if.h"
#include "oam_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_WORKQUEUE_C

/* 全局变量定义 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_workqueue_stru *g_oal_workqueue; /* oal工作队列全局变量 */

oal_int32 oal_workqueue_schedule(oal_work_stru *pst_work)
{
    if (g_oal_workqueue == OAL_PTR_NULL) {
        OAL_IO_PRINT("g_oal_workqueue is null.\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    return oal_queue_work(g_oal_workqueue, pst_work);
}

oal_int32 oal_workqueue_delay_schedule(oal_delayed_work *pst_work, oal_ulong delay)
{
    if (g_oal_workqueue == OAL_PTR_NULL) {
        OAL_IO_PRINT("g_oal_workqueue is null.\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    return oal_queue_delayed_work(g_oal_workqueue, pst_work, delay);
}

/*
 * 函 数 名  : oal_workqueue_init
 * 功能描述  : oal workqueue初始化
 * 返 回 值  : OAL_SUCC
 */
oal_uint32 oal_workqueue_init(oal_void)
{
    g_oal_workqueue = oal_create_singlethread_workqueue_m("oal_workqueue");
    if (g_oal_workqueue == OAL_PTR_NULL) {
        OAL_IO_PRINT("oal_workqueue_init: create oal workqueue failed.\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : oal_workqueue_exit
 * 功能描述  : oal workqueue退出函数
 * 返 回 值  : OAL_SUCC
 */
oal_uint32 oal_workqueue_exit(oal_void)
{
    /* 删除工作队列 */
    oal_destroy_workqueue(g_oal_workqueue);

    return OAL_SUCC;
}
oal_module_symbol(oal_workqueue_schedule);

oal_module_symbol(oal_workqueue_delay_schedule);

oal_module_license("GPL");

#endif
