

#ifndef __HMAC_MGMT_AP_H__
#define __HMAC_MGMT_AP_H__

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "wlan_spec.h"
#include "mac_frame.h"
#include "hmac_vap.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MGMT_AP_H
/* 2 宏定义 */
#define is_wep_cipher(uc_cipher) (((uc_cipher) == WLAN_80211_CIPHER_SUITE_WEP_40) || ((uc_cipher) == WLAN_80211_CIPHER_SUITE_WEP_104))

/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
extern oal_uint32 hmac_ap_up_rx_mgmt(hmac_vap_stru *pst_hmac_vap, oal_void *p_param);
extern oal_uint32 hmac_ap_up_misc(hmac_vap_stru *pst_hmac_vap, oal_void *p_param);
extern oal_uint32 hmac_ap_wait_start_rx_mgmt(hmac_vap_stru *pst_hmac_vap, oal_void *p_param);
extern oal_uint32 hmac_ap_wait_start_misc(hmac_vap_stru *pst_hmac_vap, oal_void *p_param);
extern oal_uint32 hmac_mgmt_timeout_ap(oal_void *p_param);
extern oal_void hmac_handle_disconnect_rsp_ap(hmac_vap_stru *pst_hmac_vap,
                                              hmac_user_stru *pst_hmac_user);
extern oal_uint32 hmac_ap_clean_bss(hmac_vap_stru *pst_hmac_vap);
extern oal_bool_enum hmac_go_is_auth(mac_vap_stru *pst_mac_vap);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_mgmt_ap.h */
