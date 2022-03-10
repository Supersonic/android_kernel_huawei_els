

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
/* 1 ͷ�ļ����� */
#include "hmac_opmode.h"
#include "mac_mib.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_OPMODE_C

/* 2 ȫ�ֱ������� */
/* 3 ����ʵ�� */

uint32_t hmac_check_opmode_notify(hmac_vap_stru *hmac_vap, uint8_t *mac_hdr,
                                  uint8_t *payload_offset, uint32_t msg_len,
                                  hmac_user_stru *hmac_user)
{
    uint8_t *opmode_notify_ie;
    mac_vap_stru *mac_vap;
    mac_user_stru *mac_user;
    mac_opmode_notify_stru *opmode_notify = NULL;
    uint32_t relt;

    /* ���ָ���Ѿ��ڵ��ú�����֤�ǿգ�����ֱ��ʹ�ü��� */
    mac_vap = &(hmac_vap->st_vap_base_info);
    mac_user = &(hmac_user->st_user_base_info);

    if (oal_any_zero_value2(mac_mib_get_VHTOptionImplemented(mac_vap),
        mac_mib_get_OperatingModeNotificationImplemented(mac_vap))) {
        return OAL_SUCC;
    }

    opmode_notify_ie = mac_find_ie(MAC_EID_OPMODE_NOTIFY, payload_offset, (int32_t)msg_len);
    /* STA������vht��,��vap��˫�ռ���������²Ž�����ie */
    if (oal_value_nq_and_eq(NULL, opmode_notify_ie, MAC_OPMODE_NOTIFY_LEN, opmode_notify_ie[1])) {
        opmode_notify = (mac_opmode_notify_stru *)(opmode_notify_ie + MAC_IE_HDR_LEN);

        /* SMPS�Ѿ����������¿ռ�����������ռ���������SMPS��OPMODE�Ŀռ�����Ϣ��ͬ */
        if (mac_user->en_avail_num_spatial_stream > opmode_notify->bit_rx_nss ||
            (oal_value_nq_and_eq(WLAN_SINGLE_NSS, opmode_notify->bit_rx_nss,
                                 WLAN_SINGLE_NSS, mac_user->en_avail_num_spatial_stream))) {
            oam_warning_log0(0, OAM_SF_OPMODE, "{hmac_check_opmode_notify::SMPS and OPMODE show different nss!}");
            if (oal_value_eq_any2(mac_user->en_cur_protocol_mode, WLAN_HT_MODE, WLAN_HT_ONLY_MODE)) {
                return OAL_FAIL;
            }
        }

        relt = mac_ie_proc_opmode_field(mac_vap, mac_user, opmode_notify, OAL_FALSE);
        if (oal_unlikely(relt != OAL_SUCC)) {
            oam_warning_log1(mac_user->uc_vap_id, OAM_SF_OPMODE,
                "{hmac_check_opmode_notify::mac_ie_proc_opmode_field failed[%d].}", relt);
            return relt;
        }
    }
    return OAL_SUCC;
}


uint32_t hmac_mgmt_rx_opmode_notify_frame(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
                                          oal_netbuf_stru *netbuf)
{
    mac_opmode_notify_stru *opmode_notify = NULL;
    uint8_t *frame_payload;
    mac_user_stru *mac_user;
    mac_rx_ctl_stru *rx_ctrl;
    uint8_t *data;
    uint32_t ret;

    if (oal_any_null_ptr3(hmac_vap, hmac_user, netbuf)) {
        oam_error_log0(0, OAM_SF_OPMODE,
            "{hmac_mgmt_rx_opmode_notify_frame::pst_hmac_vap or pst_hmac_user or netbuf is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (oal_any_zero_value2(mac_mib_get_VHTOptionImplemented(&hmac_vap->st_vap_base_info),
        mac_mib_get_OperatingModeNotificationImplemented(&hmac_vap->st_vap_base_info))) {
        oam_info_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_OPMODE,
                      "{hmac_mgmt_rx_opmode_notify_frame::the vap is not support OperatingModeNotification!}\r\n");
        return OAL_SUCC;
    }

    rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

    mac_user = &(hmac_user->st_user_base_info);
    if (mac_user == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_OPMODE,
                       "{hmac_mgmt_rx_opmode_notify_frame::pst_mac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ֡��ָ�� */
    data = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(rx_ctrl);

    /****************************************************/
    /*   OperatingModeNotification Frame - Frame Body   */
    /* ------------------------------------------------- */
    /* |   Category   |   Action   |   OperatingMode   | */
    /* ------------------------------------------------- */
    /* |   1          |   1        |   1               | */
    /* ------------------------------------------------- */
    /*                                                  */
    /****************************************************/
    /* ��ȡpayload��ָ�� */
    frame_payload = (uint8_t *)data + MAC_80211_FRAME_LEN;
    opmode_notify = (mac_opmode_notify_stru *)(frame_payload + MAC_ACTION_OFFSET_ACTION + 1);

    ret = mac_ie_proc_opmode_field(&(hmac_vap->st_vap_base_info),
        mac_user, opmode_notify, OAL_FALSE);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(mac_user->uc_vap_id, OAM_SF_OPMODE,
                         "{hmac_mgmt_rx_opmode_notify_frame::hmac_config_send_event failed[%d].}", ret);
        return ret;
    }

    return OAL_SUCC;
}


void mac_set_opmode_field(uint8_t *pst_vap, uint8_t *puc_buffer, wlan_nss_enum_uint8 en_nss)
{
    mac_vap_stru           *pst_mac_vap       = (mac_vap_stru *)pst_vap;
    mac_opmode_notify_stru *pst_opmode_notify = (mac_opmode_notify_stru *)puc_buffer;
    wlan_bw_cap_enum_uint8  en_cp_bw          = WLAN_BW_CAP_BUTT;

    /******************************************************************/
    /* -------------------------------------------------------------- */
    /* |B0-B1         |B2        |B3       |B4-B6   |B7         |     */
    /* -------------------------------------------------------------- */
    /* |Channel Width |160M/80+80|no_LDPC  |Rx Nss  |Rx Nss Type|     */
    /* -------------------------------------------------------------- */
    /******************************************************************/
    /* bit_rx_nss_typeĿǰд��0���ɣ����������������ϣ�����txbf��С������ʱ������Ҫ����1���� ����չTBD */
    memset_s(pst_opmode_notify, sizeof(mac_opmode_notify_stru), 0, sizeof(mac_opmode_notify_stru));
    mac_vap_get_bandwidth_cap(pst_mac_vap, &en_cp_bw);
     /* 16��11acЭ�� 160M�Ҳ�ͨ */
    if ((en_cp_bw == WLAN_BW_CAP_160M) && (pst_mac_vap->bit_ap_chip_oui == WLAN_AP_CHIP_OUI_BCM)) {
        pst_opmode_notify->bit_channel_width = WLAN_BW_CAP_80M;
        pst_opmode_notify->bit_160or8080     = OAL_TRUE;
    } else {
        /* 13��Э�� */
        pst_opmode_notify->bit_channel_width = en_cp_bw;
    }

    /* Ҫ�л�������������vap�µ�nss�����Ļ�����Ҫ��ǰ��vap��nss�������ӿڲ���������Σ���߳�������չ�� */
    pst_opmode_notify->bit_rx_nss        = en_nss;
    pst_opmode_notify->bit_rx_nss_type   = 0;
}



void mac_set_opmode_notify_ie(uint8_t *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)pst_vap;

    /********************************************
            -------------------------------------
            |ElementID | Length | Operating Mode|
            -------------------------------------
    Octets: |1         | 1      | 1             |
            -------------------------------------

    ********************************************/
    if ((OAL_FALSE == mac_mib_get_VHTOptionImplemented(pst_mac_vap)) ||
        (OAL_FALSE == mac_mib_get_OperatingModeNotificationImplemented(pst_mac_vap))) {
        *puc_ie_len = 0;
        return;
    }

    puc_buffer[0] = MAC_EID_OPMODE_NOTIFY;
    puc_buffer[1] = MAC_OPMODE_NOTIFY_LEN;

    mac_set_opmode_field((void *)pst_vap, (puc_buffer + MAC_IE_HDR_LEN), pst_mac_vap->en_vap_rx_nss);

    *puc_ie_len = MAC_IE_HDR_LEN + MAC_OPMODE_NOTIFY_LEN;
}

#endif