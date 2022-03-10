

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
/* 1 头文件包含 */
#include "hmac_hcc_adapt.h"
#include "mac_resource.h"
#include "oal_hcc_host_if.h"
#include "frw_event_main.h"
#include "hmac_vap.h"

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_HCC_ADAPT_C

/* 2 全局变量定义 */
OAL_STATIC oal_uint8 g_hcc_sched_stat[FRW_EVENT_TYPE_BUTT];
OAL_STATIC oal_uint8 g_hcc_flowctrl_stat[FRW_EVENT_TYPE_BUTT];
OAL_STATIC oal_uint32 g_hcc_sched_event_pkts[FRW_EVENT_TYPE_BUTT] = { 0 };
OAL_STATIC oal_uint8 g_wlan_queue_to_dmac_queue[WLAN_NET_QUEUE_BUTT];

extern oal_uint32 hmac_hcc_tx_netbuf(frw_event_mem_stru *pst_hcc_event_mem,
                                     oal_netbuf_stru *pst_netbuf, oal_uint32 ul_hdr_len,
                                     oal_uint32 fc_type,
                                     oal_uint32 queue_id);
oal_uint32 hmac_hcc_tx_netbuf_auto(frw_event_mem_stru *pst_hcc_event_mem,
                                   oal_netbuf_stru *pst_netbuf, oal_uint32 ul_hdr_len);
extern oal_void hmac_hcc_tx_data(frw_event_mem_stru *pst_hcc_event_mem, oal_netbuf_stru *pst_netbuf);

/* 3 函数实现 */
oal_void hmac_tx_net_queue_map_init(oal_void)
{
    memset_s(g_wlan_queue_to_dmac_queue, OAL_SIZEOF(g_wlan_queue_to_dmac_queue), DATA_LO_QUEUE,
             OAL_SIZEOF(g_wlan_queue_to_dmac_queue));
    g_wlan_queue_to_dmac_queue[WLAN_HI_QUEUE] = DATA_HI_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_NORMAL_QUEUE] = DATA_LO_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_TCP_DATA_QUEUE] = DATA_TCP_DATA_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_TCP_ACK_QUEUE] = DATA_TCP_ACK_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_UDP_BK_QUEUE] = DATA_UDP_BK_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_UDP_BE_QUEUE] = DATA_UDP_BE_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_UDP_VI_QUEUE] = DATA_UDP_VI_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_UDP_VO_QUEUE] = DATA_UDP_VO_QUEUE;

    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_HI_QUEUE, WLAN_HI_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_LO_QUEUE, WLAN_NORMAL_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_TCP_DATA_QUEUE, WLAN_TCP_DATA_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_TCP_ACK_QUEUE, WLAN_TCP_ACK_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_UDP_BK_QUEUE, WLAN_UDP_BK_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_UDP_BE_QUEUE, WLAN_UDP_BE_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_UDP_VI_QUEUE, WLAN_UDP_VI_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_UDP_VO_QUEUE, WLAN_UDP_VO_QUEUE);
}

#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
oal_int32 hmac_tx_event_pkts_info_print(oal_void *data, char *buf, oal_int32 buf_len)
{
    int i;
    oal_int32 ret = 0;
    oal_uint64 total = 0;
    struct hcc_handler *hcc = hcc_get_110x_handler();
    if (hcc == NULL) {
        return ret;
    }

    ret += snprintf_s(buf + ret, buf_len - ret, buf_len - ret - 1, "tx_event_pkts_info_show\n");
    for (i = 0; i < FRW_EVENT_TYPE_BUTT; i++) {
        if (g_hcc_sched_event_pkts[i]) {
            ret += snprintf_s(buf + ret, buf_len - ret, buf_len - ret - 1, "event:%d, pkts:%10u\n",
                i, g_hcc_sched_event_pkts[i]);
        }
        total += g_hcc_sched_event_pkts[i];
    }

    if (total) {
        ret += snprintf_s(buf + ret, buf_len - ret, buf_len - ret - 1, "total:%llu\n", total);
    }
    return ret;
}
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
OAL_STATIC declare_wifi_panic_stru(hmac_panic_hcc_adapt, hmac_tx_event_pkts_info_print);
#endif

oal_void hmac_tx_sched_info_init(oal_void)
{
    memset_s(g_hcc_sched_stat, OAL_SIZEOF(g_hcc_sched_stat), DATA_LO_QUEUE, OAL_SIZEOF(g_hcc_sched_stat));

    /* set the event sched PRI, TBD */
    g_hcc_sched_stat[FRW_EVENT_TYPE_HIGH_PRIO] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_HOST_CRX] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_HOST_DRX] = DATA_LO_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_HOST_CTX] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_HOST_SDT_REG] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_WLAN_CRX] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_WLAN_DRX] = DATA_LO_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_WLAN_CTX] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_WLAN_TX_COMP] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_TBTT] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_TIMEOUT] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_DMAC_MISC] = DATA_HI_QUEUE;

    memset_s(g_hcc_flowctrl_stat, OAL_SIZEOF(g_hcc_flowctrl_stat), HCC_FC_NONE, OAL_SIZEOF(g_hcc_flowctrl_stat));

    /* 来自HOST的事件，如果从Kernel Net过来选择网络层流控+丢包的方式，
     * 如果是Wlan To Wlan 的方式，直接丢包!
	 */
    g_hcc_flowctrl_stat[FRW_EVENT_TYPE_HOST_DRX] = HCC_FC_DROP | HCC_FC_NET;
}

OAL_STATIC OAL_INLINE oal_void hmac_hcc_adapt_extend_hdr_init(frw_event_mem_stru *pst_hcc_event_mem,
                                                              oal_netbuf_stru *pst_netbuf)
{
    struct frw_hcc_extend_hdr *pst_hdr;
    frw_event_hdr_stru *pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    pst_hdr = (struct frw_hcc_extend_hdr *)oal_netbuf_data(pst_netbuf);
    pst_hdr->en_nest_type = pst_event_hdr->en_type;
    pst_hdr->uc_nest_sub_type = pst_event_hdr->uc_sub_type;
    pst_hdr->device_id = pst_event_hdr->uc_device_id;
    pst_hdr->chip_id = pst_event_hdr->uc_chip_id;
    pst_hdr->vap_id = pst_event_hdr->uc_vap_id;
}

oal_void get_simple_mac_tx_ctl(mac_tx_ctl_cut_stru *pst_simple_mac_tx_ctl, mac_tx_ctl_stru *pst_tx_ctrl)
{
    pst_simple_mac_tx_ctl->uc_alg_pktno = (oal_uint8)pst_tx_ctrl->ul_alg_pktno;
    pst_simple_mac_tx_ctl->bit_en_event_type = pst_tx_ctrl->en_event_type;
    pst_simple_mac_tx_ctl->bit_frame_header_length = pst_tx_ctrl->uc_frame_header_length;
    pst_simple_mac_tx_ctl->bit_is_needretry = pst_tx_ctrl->bit_is_needretry;
    pst_simple_mac_tx_ctl->bit_is_vipframe = pst_tx_ctrl->bit_is_vipframe;
    pst_simple_mac_tx_ctl->bit_mpdu_num = pst_tx_ctrl->uc_mpdu_num;
    pst_simple_mac_tx_ctl->bit_netbuf_num = pst_tx_ctrl->uc_netbuf_num;
    pst_simple_mac_tx_ctl->bit_retried_num = pst_tx_ctrl->uc_retried_num;
    pst_simple_mac_tx_ctl->bit_tx_user_idx = (oal_uint8)pst_tx_ctrl->us_tx_user_idx;
    pst_simple_mac_tx_ctl->bit_tx_vap_index = pst_tx_ctrl->uc_tx_vap_index;
    pst_simple_mac_tx_ctl->en_ismcast = pst_tx_ctrl->en_ismcast;
    pst_simple_mac_tx_ctl->en_is_first_msdu = pst_tx_ctrl->uc_is_first_msdu;
    pst_simple_mac_tx_ctl->en_is_get_from_ps_queue = pst_tx_ctrl->en_is_get_from_ps_queue;
    pst_simple_mac_tx_ctl->en_is_probe_data = pst_tx_ctrl->en_is_probe_data;
    /* useless struct member */
    pst_simple_mac_tx_ctl->us_mpdu_bytes = pst_tx_ctrl->us_mpdu_bytes;
    pst_simple_mac_tx_ctl->bit_mgmt_frame_id = pst_tx_ctrl->bit_mgmt_frame_id;
    pst_simple_mac_tx_ctl->bit_is_eapol_key_ptk = pst_tx_ctrl->bit_is_eapol_key_ptk;
    pst_simple_mac_tx_ctl->bit_need_rsp = pst_tx_ctrl->bit_need_rsp;
    pst_simple_mac_tx_ctl->bit_ac = pst_tx_ctrl->uc_ac;
    pst_simple_mac_tx_ctl->en_is_amsdu = pst_tx_ctrl->en_is_amsdu;
    pst_simple_mac_tx_ctl->en_ack_policy = pst_tx_ctrl->en_ack_policy;
    pst_simple_mac_tx_ctl->bit_ether_head_including = pst_tx_ctrl->bit_ether_head_including;
    pst_simple_mac_tx_ctl->bit_is_large_skb_amsdu = pst_tx_ctrl->bit_is_large_skb_amsdu;
    pst_simple_mac_tx_ctl->bit_tid = (pst_tx_ctrl->uc_tid & 0x0F);
    /* amsdu+ampdu聚合对齐用 */
    pst_simple_mac_tx_ctl->bit_align_padding_offset = pst_tx_ctrl->bit_align_padding_offset;
    pst_simple_mac_tx_ctl->bit_data_frame_type = pst_tx_ctrl->bit_data_frame_type;
}


oal_void get_mac_rx_ctl(mac_rx_ctl_stru *pst_mac_rx_ctl, mac_rx_ctl_cut_stru *pst_mac_rx_cut_ctl)
{
    pst_mac_rx_ctl->bit_amsdu_enable = pst_mac_rx_cut_ctl->bit_amsdu_enable;
    pst_mac_rx_ctl->bit_buff_nums = pst_mac_rx_cut_ctl->bit_buff_nums;
    pst_mac_rx_ctl->bit_is_last_buffer = pst_mac_rx_cut_ctl->bit_is_last_buffer;
    pst_mac_rx_ctl->bit_is_first_buffer = pst_mac_rx_cut_ctl->bit_is_first_buffer;
    pst_mac_rx_ctl->bit_is_fragmented = pst_mac_rx_cut_ctl->bit_is_fragmented;
    pst_mac_rx_ctl->uc_mac_header_len = pst_mac_rx_cut_ctl->bit_mac_header_len;
    pst_mac_rx_ctl->bit_data_frame_type = pst_mac_rx_cut_ctl->bit_data_frame_type;
    pst_mac_rx_ctl->us_ta_user_idx = pst_mac_rx_cut_ctl->bit_ta_user_idx;
    pst_mac_rx_ctl->bit_vap_id = pst_mac_rx_cut_ctl->bit_vap_id;
    pst_mac_rx_ctl->uc_msdu_in_buffer = pst_mac_rx_cut_ctl->uc_msdu_in_buffer;
    pst_mac_rx_ctl->us_frame_len = pst_mac_rx_cut_ctl->us_frame_len;
    pst_mac_rx_ctl->uc_mac_vap_id = pst_mac_rx_cut_ctl->uc_mac_vap_id;
    pst_mac_rx_ctl->uc_channel_number = pst_mac_rx_cut_ctl->uc_channel_number;
    pst_mac_rx_ctl->bit_is_beacon = pst_mac_rx_cut_ctl->bit_is_beacon;
    pst_mac_rx_ctl->bit_has_tcp_ack_info = pst_mac_rx_cut_ctl->bit_has_tcp_ack_info;
}


oal_uint32 check_headroom_add_length(mac_tx_ctl_stru *pst_tx_ctrl, frw_event_type_enum_uint8 en_nest_type,
                                     oal_uint8 uc_nest_sub_type)
{
    oal_uint32 ul_headroom_add;

    if (pst_tx_ctrl->bit_80211_mac_head_type == 1) {
        /* case 1: data from net, mac head is maintence in netbuff */
        /*lint -e778*/
        ul_headroom_add = OAL_SIZEOF(mac_tx_ctl_cut_stru) - (MAC_80211_QOS_HTC_4ADDR_FRAME_LEN -
                                                             MAX_MAC_HEAD_LEN);  // 结构体肯定大于4
        /*lint +e778*/
    } else if ((en_nest_type == FRW_EVENT_TYPE_WLAN_CTX) &&
               ((uc_nest_sub_type == DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT) ||
                (uc_nest_sub_type == DMAC_WLAN_CTX_EVENT_SUB_TYPE_ACTION))) {
        /* case 2: mgmt frame, mac header is maintence in payload part */
        ul_headroom_add = OAL_SIZEOF(mac_tx_ctl_cut_stru) + (MAX_MAC_HEAD_LEN - MAC_80211_FRAME_LEN);
    } else {
        /* case 3: data from net, mac head not maintence in netbuff */
        /* case 4: netbuff alloced in adapt layer */
        ul_headroom_add = MAX_MAC_HEAD_LEN + OAL_SIZEOF(mac_tx_ctl_cut_stru);
    }

    return ul_headroom_add;
}


oal_void hmac_adjust_netbuf_data(oal_netbuf_stru *pst_netbuf, mac_tx_ctl_stru *pst_tx_ctrl,
                                 frw_event_type_enum_uint8 en_nest_type, oal_uint8 uc_nest_sub_type)
{
    oal_uint8 *puc_data_hdr;
    mac_tx_ctl_cut_stru st_simple_mac_tx_ctl;
    oal_int32 l_ret = EOK;

    /* 在进入HCC之前，将CB字段和Mac头连续存放至payload之前 */
    puc_data_hdr = oal_netbuf_data(pst_netbuf);

    memset_s(&st_simple_mac_tx_ctl, OAL_SIZEOF(mac_tx_ctl_cut_stru), 0, OAL_SIZEOF(mac_tx_ctl_cut_stru));
    get_simple_mac_tx_ctl(&st_simple_mac_tx_ctl, pst_tx_ctrl);

    if ((en_nest_type == FRW_EVENT_TYPE_WLAN_CTX) &&
        ((uc_nest_sub_type == DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT) ||
         (uc_nest_sub_type == DMAC_WLAN_CTX_EVENT_SUB_TYPE_ACTION))) {
        /* case 1: mgmt frame, mac header is maintence in payload part */
        l_ret += memcpy_s(puc_data_hdr, OAL_SIZEOF(mac_tx_ctl_cut_stru),
            (oal_uint8 *)&st_simple_mac_tx_ctl, OAL_SIZEOF(mac_tx_ctl_cut_stru));
        /* copy mac hdr */
        l_ret += memmove_s(puc_data_hdr + OAL_SIZEOF(mac_tx_ctl_cut_stru), MAC_80211_FRAME_LEN,
                           puc_data_hdr + OAL_SIZEOF(mac_tx_ctl_cut_stru) + (MAX_MAC_HEAD_LEN - MAC_80211_FRAME_LEN),
                           MAC_80211_FRAME_LEN);
    } else if (pst_tx_ctrl->pst_frame_header != OAL_PTR_NULL) {
        /* case 2: data from net, mac head not maintence in netbuff */
        /* case 3: netbuff alloced in adapt layer */
        l_ret += memmove_s(puc_data_hdr + OAL_SIZEOF(mac_tx_ctl_cut_stru), MAX_MAC_HEAD_LEN,
                           (oal_uint8 *)pst_tx_ctrl->pst_frame_header, MAX_MAC_HEAD_LEN);

        l_ret += memcpy_s(puc_data_hdr, OAL_SIZEOF(mac_tx_ctl_cut_stru),
            (oal_uint8 *)&st_simple_mac_tx_ctl, OAL_SIZEOF(mac_tx_ctl_cut_stru));

        /* 帧头和帧体不连续，帧头重新申请了事件内存，此处需要释放 */
        if (pst_tx_ctrl->bit_80211_mac_head_type == 0) {
            oal_mem_free_m((oal_uint8 *)pst_tx_ctrl->pst_frame_header, OAL_TRUE);
        }
    }
    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_adjust_netbuf_data::memcpy or memmove fail!");
        return;
    }
}


oal_uint32 hmac_hcc_tx_netbuf_auto(frw_event_mem_stru *pst_hcc_event_mem,
                                   oal_netbuf_stru *pst_netbuf, oal_uint32 ul_hdr_len)
{
    oal_uint32 fc_type;
    oal_uint32 queue_id;
    frw_event_hdr_stru *pst_event_hdr;
    frw_event_type_enum_uint8 en_type;
    pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    en_type = pst_event_hdr->en_type;

    if (oal_warn_on(en_type >= FRW_EVENT_TYPE_BUTT)) {
        oal_netbuf_free(pst_netbuf);
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{hmac_hcc_tx_netbuf_auto::FRW_EVENT_TYPE[%d] over limit!}", en_type);
        return OAL_FAIL;
    }

    queue_id = g_hcc_sched_stat[en_type];
    fc_type = g_hcc_flowctrl_stat[en_type];

    /* 对于从以太网报文获取其队列号 */
    if (en_type == FRW_EVENT_TYPE_HOST_DRX) {
        queue_id = oal_skb_get_queue_mapping(pst_netbuf);
        if (oal_warn_on(queue_id >= WLAN_NET_QUEUE_BUTT)) {
            queue_id = DATA_LO_QUEUE;
        } else {
            queue_id = g_wlan_queue_to_dmac_queue[queue_id];
        }
    }

    return hmac_hcc_tx_netbuf(pst_hcc_event_mem, pst_netbuf, ul_hdr_len, fc_type, queue_id);
}


oal_uint32 hmac_hcc_tx_netbuf(frw_event_mem_stru *pst_hcc_event_mem,
                              oal_netbuf_stru *pst_netbuf, oal_uint32 ul_hdr_len,
                              oal_uint32 fc_type,
                              oal_uint32 queue_id)
{
#ifdef CONFIG_PRINTK
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
#endif
    oal_int32 ul_headroom_add = 0;
    oal_uint32 ret = OAL_SUCC;

    declare_hcc_tx_param_initializer(st_hcc_transfer_param,
                                     HCC_ACTION_TYPE_WIFI,
                                     0,
                                     ul_hdr_len + OAL_SIZEOF(struct frw_hcc_extend_hdr),
                                     fc_type,
                                     queue_id);

    if (oal_unlikely(pst_netbuf == NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_hcc_tx_netbuf:: pst_netbuf is null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (oal_unlikely(oal_netbuf_headroom(pst_netbuf) < OAL_SIZEOF(struct frw_hcc_extend_hdr))) {
        ul_headroom_add = (oal_int32)OAL_SIZEOF(struct frw_hcc_extend_hdr) - (oal_int32)oal_netbuf_headroom(pst_netbuf);
    }

    if (ul_headroom_add > 0) {
        oal_int32 ul_head_room = (oal_int32)oal_netbuf_headroom(pst_netbuf);
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{hmac_hcc_tx_netbuf expand head done![%d]}", ul_head_room);
        ret = (oal_uint32)oal_netbuf_expand_head(pst_netbuf, (oal_int32)ul_headroom_add, 0, GFP_ATOMIC);
        if (oal_warn_on(ret != OAL_SUCC)) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: alloc head room failed.}");
            oal_netbuf_free(pst_netbuf);
            return OAL_ERR_CODE_ALLOC_MEM_FAIL;
        }
    }

    /* add frw hcc extend area */
    oal_netbuf_push(pst_netbuf, OAL_SIZEOF(struct frw_hcc_extend_hdr));
    hmac_hcc_adapt_extend_hdr_init(pst_hcc_event_mem, pst_netbuf);

    // expand 14B后性能下降40%,待确认!
#ifdef CONFIG_PRINTK
    pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);

    ret = (oal_uint32)hcc_tx(hcc_get_110x_handler(), pst_netbuf, &st_hcc_transfer_param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        /* hcc 关闭时下发了命令,报警需要清理 */
        if (oal_warn_on(ret == -OAL_EBUSY)) {
            OAL_IO_PRINT("[E]hmac_tx event[%u:%u] drop!\n", pst_event_hdr->en_type, pst_event_hdr->uc_sub_type);
            ret = OAL_SUCC;
            declare_dft_trace_key_info("hcc_is_busy", OAL_DFT_TRACE_OTHER);
        }

        if (ret == -OAL_EIO) {
            /* hcc exception, drop the pkts */
            ret = OAL_SUCC;
        }

        oal_netbuf_free(pst_netbuf);
    } else {
        if (oal_likely(pst_event_hdr->en_type < FRW_EVENT_TYPE_BUTT)) {
            g_hcc_sched_event_pkts[pst_event_hdr->en_type]++;
        }
    }

    return ret;
#else
    /* UT Failed! Should remove this macro when DMT! */
    return ret;
#endif
}


oal_void hmac_set_netbuf_4byte_aligned(oal_netbuf_stru *pst_netbuf)
{
    oal_uint ul_netbuf_old_addr;
    oal_uint ul_netbuf_new_addr;
    oal_uint ul_addr_offset;

    ul_netbuf_old_addr =
        (oal_uint)(uintptr_t)(oal_netbuf_data(pst_netbuf) + OAL_SIZEOF(mac_tx_ctl_cut_stru) + MAX_MAC_HEAD_LEN);
    ul_netbuf_new_addr = OAL_ROUND_DOWN(ul_netbuf_old_addr, 4); /* 向下取整，使netbuf 4字节对齐 */
    ul_addr_offset = ul_netbuf_old_addr - ul_netbuf_new_addr;

    /* 未对齐时在host侧做数据搬移，此处牺牲host，解放device */
    if ((ul_addr_offset != 0) && (ul_addr_offset < oal_netbuf_headroom(pst_netbuf))) {
        if (memmove_s((oal_uint8 *)oal_netbuf_data(pst_netbuf) - ul_addr_offset, oal_netbuf_len(pst_netbuf),
            (oal_uint8 *)oal_netbuf_data(pst_netbuf), oal_netbuf_len(pst_netbuf)) != EOK) {
            OAM_ERROR_LOG0(0, 0, "hmac_set_netbuf_4byte_aligned::memmove_s fail!");
            return;
        }
        oal_netbuf_push(pst_netbuf, ul_addr_offset);
        oal_netbuf_trim(pst_netbuf, ul_addr_offset);
    }
}

oal_void hmac_hcc_tx_data(frw_event_mem_stru *pst_hcc_event_mem, oal_netbuf_stru *pst_netbuf)
{
    frw_event_hdr_stru *pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    frw_event_type_enum_uint8 en_type = pst_event_hdr->en_type;
    oal_uint8 uc_sub_type = pst_event_hdr->uc_sub_type;
    mac_tx_ctl_stru *pst_tx_ctrl = OAL_PTR_NULL;
    oal_uint32 ul_headroom_add;
    oal_int32 ret = OAL_SUCC;
    oal_uint8 auc_macheader[MAC_80211_QOS_HTC_4ADDR_FRAME_LEN] = { 0 };
    oal_uint32 ul_ret;

    if (oal_unlikely(pst_netbuf == NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: pst_netbuf is null}");
        return;
    }
    pst_tx_ctrl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    if (oal_warn_on(pst_tx_ctrl->en_use_4_addr)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: use 4 address.}");
        oal_netbuf_free(pst_netbuf);
        return;
    }
    ul_headroom_add = check_headroom_add_length(pst_tx_ctrl, en_type, uc_sub_type);
    if (ul_headroom_add > oal_netbuf_headroom(pst_netbuf)) {
        if (pst_tx_ctrl->bit_80211_mac_head_type == 1) {
            if (memcpy_s(auc_macheader, MAC_80211_QOS_HTC_4ADDR_FRAME_LEN,
                (oal_uint8 *)pst_tx_ctrl->pst_frame_header, MAX_MAC_HEAD_LEN) != EOK) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_hcc_tx_data::memcpy fail!");
            }
        }
        ret = oal_netbuf_expand_head(pst_netbuf,
            (oal_int32)ul_headroom_add - (oal_int32)oal_netbuf_headroom(pst_netbuf), 0, GFP_ATOMIC);
        if (oal_warn_on(ret != OAL_SUCC)) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: alloc head room failed.}");
            oal_netbuf_free(pst_netbuf);
            return;
        }

        if (pst_tx_ctrl->bit_80211_mac_head_type == 1) {
            if (memcpy_s(oal_netbuf_data(pst_netbuf), MAX_MAC_HEAD_LEN, auc_macheader, MAX_MAC_HEAD_LEN) != EOK) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_hcc_tx_data::memcpy fail!");
            }
            pst_tx_ctrl->pst_frame_header = (mac_ieee80211_frame_stru *)oal_netbuf_data(pst_netbuf);
        }
    }

    /* 修改netbuff的data指针和len */
    oal_netbuf_push(pst_netbuf, ul_headroom_add);
    hmac_adjust_netbuf_data(pst_netbuf, pst_tx_ctrl, en_type, uc_sub_type);
    hmac_set_netbuf_4byte_aligned(pst_netbuf);
    /* netbuf不管成功与否都由发送函数释放! */
    ul_ret = hmac_hcc_tx_netbuf_auto(pst_hcc_event_mem, pst_netbuf, OAL_SIZEOF(mac_tx_ctl_cut_stru) + MAX_MAC_HEAD_LEN);
    return;
}

oal_uint32 hmac_hcc_tx_netbuf_adapt(frw_event_mem_stru *pst_hcc_event_mem,
                                    oal_netbuf_stru *pst_netbuf)
{
    return hmac_hcc_tx_netbuf_auto(pst_hcc_event_mem, pst_netbuf, 0);
}

oal_uint32 hmac_hcc_tx_event_buf_to_netbuf(frw_event_mem_stru *pst_event_mem,
                                           oal_uint8 *pst_buf,
                                           oal_uint32 payload_size)
{
    oal_netbuf_stru *pst_netbuf;
    /* 申请netbuf存放事件payload */
    pst_netbuf = hcc_netbuf_alloc(payload_size);
    if (oal_warn_on(pst_netbuf == NULL)) {
        OAL_IO_PRINT("hmac_hcc_tx_event_buf_to_netbuf alloc netbuf failed!\n");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* 将结构体拷贝到netbuff数据区 */
    oal_netbuf_put(pst_netbuf, payload_size);
    if (memcpy_s((oal_uint8 *)(oal_netbuf_data(pst_netbuf)),
                 payload_size, (oal_uint8 *)pst_buf, payload_size) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_hcc_tx_event_buf_to_netbuf::memcpy fail!}");
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    return hmac_hcc_tx_netbuf_adapt(pst_event_mem, pst_netbuf);
}


oal_uint32 hmac_hcc_tx_event_payload_to_netbuf(frw_event_mem_stru *pst_event_mem,
                                               oal_uint32 payload_size)
{
    oal_uint8 *pst_event_payload = OAL_PTR_NULL;

    if (oal_warn_on(pst_event_mem == NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_hcc_tx_event_payload_to_netbuf:pst_event_mem null!");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 取业务事件信息 */
    pst_event_payload = frw_get_event_payload(pst_event_mem);
    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem, pst_event_payload, payload_size);
}


oal_void hmac_hcc_rx_event_comm_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    oal_uint8 bit_mac_header_len;
    frw_event_hdr_stru *pst_event_hdr;
    hcc_event_stru *pst_hcc_event_payload;

    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    oal_uint8 *puc_hcc_extend_hdr = OAL_PTR_NULL;

    /* step1 提取嵌套的业务事件类型 */
    pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);
    /* 完成从51Mac rx ctl 到02 Mac rx ctl的拷贝,
     * 传到此处, pad_payload已经是0
     */
    /*
     * hcc protocol header
     * |-------hcc total(64B)-----|-----------package mem--------------|
     * |hcc hdr|pad hdr|hcc extend|pad_payload|--------payload---------|
     */
    if (oal_warn_on(pst_hcc_event_payload->pst_netbuf == NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_hcc_rx_event_comm_adapt:did't found netbuf!");
        return;
    }

    puc_hcc_extend_hdr = oal_netbuf_data((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);
    bit_mac_header_len = ((mac_rx_ctl_cut_stru *)puc_hcc_extend_hdr)->bit_mac_header_len;
    if (bit_mac_header_len) {
        if (bit_mac_header_len > MAX_MAC_HEAD_LEN) {
            oam_error_log3(pst_event_hdr->uc_vap_id, OAM_SF_ANY, "invaild mac header len:%d,main:%d,sub:%d",
                           bit_mac_header_len, pst_event_hdr->en_type, pst_event_hdr->uc_sub_type);
            oal_print_hex_dump(puc_hcc_extend_hdr,
                               (oal_int32)oal_netbuf_len((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf),
                               32, "invaild mac header len"); /* 32表示groupsize */
            return;
        }

        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);
        get_mac_rx_ctl(pst_rx_ctrl, (mac_rx_ctl_cut_stru *)puc_hcc_extend_hdr);

        /* 需要修改pst_rx_ctrl中所有指针 */
        pst_rx_ctrl->pul_mac_hdr_start_addr = (oal_uint32 *)(puc_hcc_extend_hdr + OAL_MAX_CB_LEN + MAX_MAC_HEAD_LEN -
                                                             pst_rx_ctrl->uc_mac_header_len);

        /* 将mac header的内容向高地址偏移8个字节拷贝，使得mac header和payload的内容连续 */
        if (memmove_s((oal_uint8 *)pst_rx_ctrl->pul_mac_hdr_start_addr, pst_rx_ctrl->uc_mac_header_len,
            (oal_uint8 *)((oal_uint8 *)pst_rx_ctrl->pul_mac_hdr_start_addr -
            (MAX_MAC_HEAD_LEN - pst_rx_ctrl->uc_mac_header_len)),
            pst_rx_ctrl->uc_mac_header_len) != EOK) {
            OAM_ERROR_LOG0(pst_event_hdr->uc_vap_id, OAM_SF_ANY, "hmac_hcc_rx_event_comm_adapt::memmove fail!");
        }

        /* 将netbuff data指针移到payload位置 */
        oal_netbuf_pull(pst_hcc_event_payload->pst_netbuf,
                        OAL_MAX_CB_LEN + (MAX_MAC_HEAD_LEN - pst_rx_ctrl->uc_mac_header_len));
    } else {
        oal_netbuf_pull(pst_hcc_event_payload->pst_netbuf, (OAL_MAX_CB_LEN + MAX_MAC_HEAD_LEN));
    }

    return;
}


frw_event_mem_stru *hmac_hcc_expand_rx_adpat_event(frw_event_mem_stru *pst_hcc_event_mem, oal_uint32 event_size)
{
    frw_event_hdr_stru *pst_hcc_event_hdr;
    hcc_event_stru *pst_hcc_event_payload;
    oal_netbuf_stru *pst_hcc_netbuf;
    frw_event_type_enum_uint8 en_type;
    oal_uint8 uc_sub_type;
    oal_uint8 uc_chip_id;
    oal_uint8 uc_device_id;
    oal_uint8 uc_vap_id;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* 业务事件相关信息 */

    /* 提取HCC事件信息 */
    pst_hcc_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);
    pst_hcc_netbuf = pst_hcc_event_payload->pst_netbuf;
    en_type = pst_hcc_event_hdr->en_type;
    uc_sub_type = pst_hcc_event_hdr->uc_sub_type;
    uc_chip_id = pst_hcc_event_hdr->uc_chip_id;
    uc_device_id = pst_hcc_event_hdr->uc_device_id;
    uc_vap_id = pst_hcc_event_hdr->uc_vap_id;

    /* 强制转换为uint16前先做判断 */
    if (oal_unlikely(event_size > 0xffff)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY,
                         "hmac_hcc_rx_netbuf_convert_to_event::invalid event len:%d", event_size);
        /* 释放hcc事件中申请的netbuf内存 */
        oal_netbuf_free(pst_hcc_netbuf);
        return OAL_PTR_NULL;
    }

    /* 申请业务事件 */
    pst_event_mem = frw_event_alloc_m((oal_uint16)event_size);
    if (oal_warn_on(pst_event_mem == OAL_PTR_NULL)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "hmac_hcc_rx_netbuf_convert_to_event  alloc event failed,event len:%d",
                         event_size);
        /* 释放hcc事件中申请的netbuf内存 */
        oal_netbuf_free(pst_hcc_netbuf);
        return OAL_PTR_NULL;
    }

    /* 填业务事件头 */
    frw_event_hdr_init(frw_get_event_hdr(pst_event_mem),
                       en_type,
                       uc_sub_type,
                       (oal_uint16)event_size,
                       FRW_EVENT_PIPELINE_STAGE_1,
                       uc_chip_id,
                       uc_device_id,
                       uc_vap_id);

    return pst_event_mem;
}


frw_event_mem_stru *hmac_hcc_rx_netbuf_convert_to_event(frw_event_mem_stru *pst_hcc_event_mem,
                                                        oal_uint32 revert_size)
{
    hcc_event_stru *pst_hcc_event_payload = OAL_PTR_NULL;
    oal_netbuf_stru *pst_hcc_netbuf = OAL_PTR_NULL;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* 业务事件相关信息 */

    if (oal_warn_on(pst_hcc_event_mem == NULL)) {
        return NULL;
    }

    /* filter the extend buf */
    hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem);

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);
    pst_hcc_netbuf = pst_hcc_event_payload->pst_netbuf;

    if (oal_warn_on(pst_hcc_netbuf == NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "Fatal Error,payload did't contain any netbuf!");
        return OAL_PTR_NULL;
    }

    if (revert_size > oal_netbuf_len(pst_hcc_netbuf)) {
        revert_size = oal_netbuf_len(pst_hcc_netbuf);
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, revert_size);
    if (pst_event_mem == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    if (revert_size)
        if (memcpy_s((oal_uint8 *)frw_get_event_payload(pst_event_mem), revert_size,
                     (oal_uint8 *)oal_netbuf_data(pst_hcc_netbuf), revert_size) != EOK) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "hmac_hcc_rx_netbuf_convert_to_event::memcpy fail!");
            oal_netbuf_free(pst_hcc_netbuf);
            frw_event_free_m(pst_event_mem);
            return OAL_PTR_NULL;
        }

    /* 释放hcc事件中申请的netbuf内存 */
    oal_netbuf_free(pst_hcc_netbuf);

    return pst_event_mem;
}


frw_event_mem_stru *hmac_hcc_rx_convert_netbuf_to_event_default(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload = OAL_PTR_NULL;

    if (oal_warn_on(pst_hcc_event_mem == OAL_PTR_NULL)) {
        return OAL_PTR_NULL;
    }

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);
    return hmac_hcc_rx_netbuf_convert_to_event(pst_hcc_event_mem, pst_hcc_event_payload->ul_buf_len);
}


frw_event_mem_stru *hmac_hcc_test_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    hcc_event_stru *pst_hcc_rx_event = OAL_PTR_NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem);

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(hcc_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* 填业务事件信息 */
    pst_hcc_rx_event = (hcc_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_hcc_rx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_hcc_rx_event->ul_buf_len = (oal_uint32)oal_netbuf_len((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);

    return pst_event_mem;
}

frw_event_mem_stru *hmac_rx_convert_netbuf_to_netbuf_default(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    dmac_tx_event_stru *pst_ctx_event = OAL_PTR_NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem);

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(dmac_tx_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    pst_ctx_event = (dmac_tx_event_stru *)frw_get_event_payload(pst_event_mem);

    pst_ctx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_ctx_event->us_frame_len = (oal_uint16)oal_netbuf_len((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);

    oam_info_log2(0, OAM_SF_ANY, "{hmac_rx_convert_netbuf_to_netbuf_default::netbuf = %p, frame len = %d.}",
                  (uintptr_t)(pst_ctx_event->pst_netbuf), pst_ctx_event->us_frame_len);

    return pst_event_mem;
}


frw_event_mem_stru *hmac_rx_process_data_sta_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload = OAL_PTR_NULL;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    dmac_wlan_drx_event_stru *pst_wlan_rx_event = OAL_PTR_NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem);

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(dmac_wlan_drx_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* 填业务事件信息 */
    pst_wlan_rx_event = (dmac_wlan_drx_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_wlan_rx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_wlan_rx_event->us_netbuf_num = 1;  // 目前不支持通过SDIO后组链，默认都是单帧

    return pst_event_mem;
}

frw_event_mem_stru *hmac_rx_process_mgmt_event_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    dmac_wlan_crx_event_stru *pst_crx_event = OAL_PTR_NULL;

    /* 取HCC事件信息 */
    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem);

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(dmac_wlan_crx_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* 填业务事件信息 */
    pst_crx_event = (dmac_wlan_crx_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_crx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;

    return pst_event_mem;
}


frw_event_mem_stru *hmac_cali2hmac_misc_event_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload = OAL_PTR_NULL;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    hal_cali_hal2hmac_event_stru *pst_cali_save_event = OAL_PTR_NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem);

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(hal_cali_hal2hmac_event_stru));
    if (pst_event_mem == NULL) {
        OAL_IO_PRINT("cali_hmac_rx_adapt_fail\r\n");
        return NULL;
    }

    /* 填业务事件信息 */
    pst_cali_save_event = (hal_cali_hal2hmac_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_cali_save_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_cali_save_event->us_netbuf_num = 1;  // 目前不支持通过SDIO后组链，默认都是单帧

    return pst_event_mem;
}

#ifdef _PRE_WLAN_FEATURE_APF

frw_event_mem_stru *hmac_apf_program_report_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;
    frw_event_mem_stru *pst_event_mem;
    mac_apf_report_event_stru *pst_report_event;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem);

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(mac_apf_report_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* 填业务事件信息 */
    pst_report_event = (mac_apf_report_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_report_event->p_program = pst_hcc_event_payload->pst_netbuf;

    return pst_event_mem;
}
#endif


oal_uint32 hmac_proc_add_user_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_add_user_stru));
}


oal_uint32 hmac_proc_del_user_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_del_user_stru));
}


/*lint -e413*/
oal_uint32 hmac_proc_config_syn_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    hmac_to_dmac_cfg_msg_stru *pst_syn_cfg_payload;
    pst_syn_cfg_payload = (hmac_to_dmac_cfg_msg_stru *)frw_get_event_payload(pst_event_mem);

    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem,
                                               (pst_syn_cfg_payload->us_len +
                                                (oal_uint32)OAL_OFFSET_OF(hmac_to_dmac_cfg_msg_stru, auc_msg_body)));
}
/*lint +e413*/

/*lint -e413*/
oal_uint32 hmac_proc_config_syn_alg_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    hmac_to_dmac_cfg_msg_stru *pst_syn_cfg_payload;
    pst_syn_cfg_payload = (hmac_to_dmac_cfg_msg_stru *)frw_get_event_payload(pst_event_mem);

    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem,
                                               (pst_syn_cfg_payload->us_len +
                                                (oal_uint32)OAL_OFFSET_OF(hmac_to_dmac_cfg_msg_stru, auc_msg_body)));
}
/*lint +e413*/
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE)

oal_uint32 hmac_sdt_recv_sample_cmd_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    oal_uint32 ul_len;

    ul_len = (oal_uint32)((frw_get_event_stru(pst_event_mem))->st_event_hdr.us_length);
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, ul_len - OAL_SIZEOF(frw_event_hdr_stru));
}
#endif


oal_uint32 hmac_proc_tx_host_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    oal_netbuf_stru *pst_current_netbuf;
    oal_netbuf_stru *pst_current_netbuf_tmp = NULL;
    dmac_tx_event_stru *pst_dmac_tx_event_payload;

    /* 取业务事件信息 */
    pst_dmac_tx_event_payload = (dmac_tx_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_current_netbuf = pst_dmac_tx_event_payload->pst_netbuf;

    while (pst_current_netbuf != OAL_PTR_NULL) {
        /* 必须在netbuf抛出之前指向下一个netbuf，防止frw_event_dispatch_event 中重置 netbuf->next */
        pst_current_netbuf_tmp = pst_current_netbuf;
        pst_current_netbuf = oal_netbuf_next(pst_current_netbuf);

        /* netbuf 失败由被调函数释放! */
        hmac_hcc_tx_data(pst_event_mem, pst_current_netbuf_tmp);
    }
    return OAL_SUCC;
}


oal_uint32 hmac_proc_mgmt_ctx_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_proc_tx_host_tx_adapt(pst_event_mem);
}


oal_uint32 hmac_proc_tx_process_action_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_proc_tx_host_tx_adapt(pst_event_mem);
}


oal_uint32 hmac_proc_set_edca_param_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru));
}


oal_uint32 hmac_scan_proc_scan_req_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    mac_scan_req_stru *pst_h2d_scan_req_params = OAL_PTR_NULL; /* 下发的扫描参数 */

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_info_log0(0, OAM_SF_ANY, "{hmac_scan_proc_scan_req_event_tx_adapt:: scan req, enter into tx adapt.}");
    pst_h2d_scan_req_params = (mac_scan_req_stru *)frw_get_event_payload(pst_event_mem);

    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem, (oal_uint8 *)pst_h2d_scan_req_params,
                                           OAL_SIZEOF(mac_scan_req_stru));
}

oal_uint32 hmac_send_event_netbuf_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    dmac_tx_event_stru *pst_dmac_tx_event = OAL_PTR_NULL;

    if (pst_event_mem == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_CALIBRATE, "{hmac_send_event_netbuf_tx_adapt::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_tx_event = (dmac_tx_event_stru *)frw_get_event_payload(pst_event_mem);
    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem, (oal_uint8 *)oal_netbuf_data(pst_dmac_tx_event->pst_netbuf),
                                           pst_dmac_tx_event->us_frame_len);
}

#ifdef _PRE_WLAN_FEATURE_IP_FILTER

oal_uint32 hmac_config_update_ip_filter_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    dmac_tx_event_stru *pst_dmac_tx_event;

    if (pst_event_mem == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_config_update_ip_filter_tx_adapt:: pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_tx_event = (dmac_tx_event_stru *)frw_get_event_payload(pst_event_mem);
    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem, (oal_uint8 *)oal_netbuf_data(pst_dmac_tx_event->pst_netbuf),
                                           pst_dmac_tx_event->us_frame_len);
}

#endif  // _PRE_WLAN_FEATURE_IP_FILTER

oal_uint32 hmac_scan_proc_sched_scan_req_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    mac_pno_scan_stru *pst_h2d_pno_scan_req_params = OAL_PTR_NULL; /* 下发PNO调度扫描请求 */

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_h2d_pno_scan_req_params = (mac_pno_scan_stru *)(uintptr_t)(*(oal_uint *)frw_get_event_payload(pst_event_mem));

    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem, (oal_uint8 *)pst_h2d_pno_scan_req_params,
                                           OAL_SIZEOF(mac_pno_scan_stru));
}


oal_uint32 hmac_mgmt_update_user_qos_table_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_asoc_set_reg_stru));
}


oal_uint32 hmac_proc_join_set_reg_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    oam_info_log0(0, OAM_SF_ANY, "{hmac_proc_join_set_reg_event_tx_adapt::tx adapt.}");
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_join_req_set_reg_stru));
}


oal_uint32 hmac_proc_join_set_dtim_reg_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    oam_info_log0(0, OAM_SF_ANY, "{hmac_proc_join_set_dtim_reg_event_tx_adapt::tx adapt.}");
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_set_dtim_tsf_reg_stru));
}


oal_uint32 hmac_hcc_tx_convert_event_to_netbuf_uint32(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(oal_uint32));
}


oal_uint32 hmac_hcc_tx_convert_event_to_netbuf_uint16(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(oal_uint16));
}


oal_uint32 hmac_hcc_tx_convert_event_to_netbuf_uint8(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(oal_uint8));
}


oal_uint32 hmac_user_add_notify_alg_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    oam_info_log0(0, OAM_SF_ANY, "{hmac_user_add_notify_alg_tx_adapt::tx adapt.}");
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_add_user_stru));
}


oal_uint32 hmac_proc_rx_process_sync_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_action_event_stru));
}

oal_uint32 hmac_chan_select_channel_mac_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_set_chan_stru));
}


oal_uint32 hmac_chan_initiate_switch_to_new_channel_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_set_ch_switch_info_stru));
}

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP

oal_uint32 hmac_edca_opt_stat_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    /* 16个oal_uint8的大小表示payload_size */
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(oal_uint8) * 16);
}

#endif

oal_int32 hmac_rx_extend_hdr_vaild_check(struct frw_hcc_extend_hdr *pst_extend_hdr)
{
    if (oal_unlikely(pst_extend_hdr->en_nest_type >= FRW_EVENT_TYPE_BUTT)) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102A_HOST) && !defined(WIN32)
oal_int32 hmac_rx_wifi_post_action_function(struct hcc_handler *hcc, oal_uint8 stype,
                                            hcc_netbuf_stru *pst_hcc_netbuf, oal_uint8 *pst_context)
#else
oal_int32 hmac_rx_wifi_post_action_function(oal_uint8 stype,
                                            hcc_netbuf_stru *pst_hcc_netbuf, oal_uint8 *pst_context)
#endif

{
    oal_int32 ret;
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
#endif
    struct frw_hcc_extend_hdr *pst_extend_hdr = OAL_PTR_NULL;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* event mem */
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hcc_event_stru *pst_event_payload = OAL_PTR_NULL;
    mac_rx_ctl_cut_stru *pst_rx_ctl = OAL_PTR_NULL;
    oal_uint8 *puc_hcc_extend_hdr = OAL_PTR_NULL;
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102A_HOST) && !defined(WIN32)
    oal_reference(hcc);
#endif

    pst_extend_hdr = (struct frw_hcc_extend_hdr *)oal_netbuf_data(pst_hcc_netbuf->pst_netbuf);
    if (hmac_rx_extend_hdr_vaild_check(pst_extend_hdr) != OAL_TRUE) {
        oal_print_hex_dump(oal_netbuf_data(pst_hcc_netbuf->pst_netbuf), /* 32表示groupsize */
                           (oal_int32)oal_netbuf_len(pst_hcc_netbuf->pst_netbuf), 32, "invaild frw extend hdr: ");
        return -OAL_EINVAL;
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_extend_hdr->vap_id);

    frw_event_task_lock();
    if (oal_unlikely(pst_hmac_vap == NULL)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "hmac rx adapt ignored,pst vap is null, vap id:%u", pst_extend_hdr->vap_id);
        frw_event_task_unlock();
        oal_netbuf_free(pst_hcc_netbuf->pst_netbuf);
        return -OAL_EINVAL;
    }

    if (oal_unlikely(pst_hmac_vap->st_vap_base_info.uc_init_flag != MAC_VAP_VAILD)) {
        if (pst_extend_hdr->vap_id == 0) {
            /* 配置VAP不过滤 */
        } else {
            oam_warning_log2(pst_extend_hdr->vap_id, OAM_SF_ANY, "hmac rx adapt ignored,main:%u,sub:%u",
                             pst_extend_hdr->en_nest_type, pst_extend_hdr->uc_nest_sub_type);
            frw_event_task_unlock();
            oal_netbuf_free(pst_hcc_netbuf->pst_netbuf);
            return -OAL_ENOMEM;
        }
    }
    frw_event_task_unlock();
#endif

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(hcc_event_stru));
    if (pst_event_mem == NULL) {
        OAL_IO_PRINT("[WARN]event mem alloc failed\n");
        return -OAL_ENOMEM;
    }

    /* trim the frw hcc extend header */
    oal_netbuf_pull(pst_hcc_netbuf->pst_netbuf, OAL_SIZEOF(struct frw_hcc_extend_hdr));

    /* event hdr point */
    pst_event = frw_get_event_stru(pst_event_mem);

    /* fill event hdr */
    frw_event_hdr_init(&(pst_event->st_event_hdr), pst_extend_hdr->en_nest_type, pst_extend_hdr->uc_nest_sub_type,
                       OAL_SIZEOF(hcc_event_stru), FRW_EVENT_PIPELINE_STAGE_1, pst_extend_hdr->chip_id,
                       pst_extend_hdr->device_id, pst_extend_hdr->vap_id);

    pst_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_event_payload->pst_netbuf = pst_hcc_netbuf->pst_netbuf;
    pst_event_payload->ul_buf_len = oal_netbuf_len(pst_hcc_netbuf->pst_netbuf);

    puc_hcc_extend_hdr = oal_netbuf_data((oal_netbuf_stru *)pst_event_payload->pst_netbuf);
    pst_rx_ctl = (mac_rx_ctl_cut_stru *)puc_hcc_extend_hdr;

    if (!(pst_rx_ctl->bit_is_beacon)) {
        g_pm_wifi_rxtx_count++;  // 收包统计 for pm
    }

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if ((wlan_pm_wkup_src_debug_get() == OAL_TRUE) && ((pst_extend_hdr->en_nest_type != FRW_EVENT_TYPE_WLAN_DRX))) {
        OAL_IO_PRINT("wifi_wake_src:event[%d],subtype[%d]!\n", pst_extend_hdr->en_nest_type,
                     pst_extend_hdr->uc_nest_sub_type);

        /* 管理帧事件，开关在管理帧处理流程中打印具体的管理帧类型后关闭 */
        if (!((pst_extend_hdr->en_nest_type == FRW_EVENT_TYPE_WLAN_CRX) &&
              (pst_extend_hdr->uc_nest_sub_type == DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX))) {
            wlan_pm_wkup_src_debug_set(OAL_FALSE);
        }
    }
#endif
    frw_event_task_lock();
    ret = (oal_int32)frw_event_dispatch_event(pst_event_mem);
    frw_event_task_unlock();
    if (oal_warn_on(ret != OAL_SUCC)) {
        /* 如果事件入队失败，内存失败由该函数释放，直接调用的由rx adapt函数释放! */
        OAL_IO_PRINT("[WARN]hcc rx post event failed!!!ret=%u,main:%d,sub:%d\n",
                     ret, pst_extend_hdr->en_nest_type,
                     pst_extend_hdr->uc_nest_sub_type);
    }
    frw_event_free_m(pst_event_mem);

    return ret;
}

oal_void hmac_hcc_adapt_init(oal_void)
{
    hmac_tx_net_queue_map_init();
    hmac_tx_sched_info_init();

    hcc_rx_register(hcc_get_110x_handler(), HCC_ACTION_TYPE_WIFI, hmac_rx_wifi_post_action_function, NULL);
#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
    hwifi_panic_log_register(&hmac_panic_hcc_adapt, NULL);
#endif
}
oal_void hmac_hcc_adapt_deinit(oal_void)
{
    hcc_rx_unregister(hcc_get_110x_handler(), HCC_ACTION_TYPE_WIFI);
#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
    hwifi_panic_log_unregister(&hmac_panic_hcc_adapt);
#endif
}

#endif


