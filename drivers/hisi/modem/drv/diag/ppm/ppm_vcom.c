/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * 1 ???????????????
 */
#include "ppm_vcom.h"
#include <product_config.h>
#include <mdrv_diag_system.h>
#include <securec.h>
#include <bsp_slice.h>
#include "diag_port_manager.h"
#include "ppm_port_switch.h"
#include "diag_system_debug.h"


/*
 * 2 ??????????????????
 */
/* ???????????? VCOM ??????????????????????????? */
mdrv_ppm_vcom_debug_info_s g_ppm_vcom_debug_info[3] = {};

/*
 * ??? ??? ???: ppm_vcom_cfg_send_data
 * ????????????: ???VCOM????????????????????????
 * ????????????: pucVirAddr:   ???????????????
 * pucPhyAddr:   ???????????????
 * data_len: ????????????
 */
u32 ppm_vcom_cfg_send_data(u8 *virt_addr, u8 *phy_addr, u32 len)
{
    g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_num++;
    g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_len += len;

    if (PPM_VCOM_SEND_DATA(PPM_VCOM_CHAN_CTRL, virt_addr, len, PPM_VCOM_DATA_MODE_TRANSPARENT) != 0) {
        g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_err_num++;
        g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_err_len += len;

        diag_error("vcom cnf cmd failed, cnf sum len 0x%x, cnf err len 0x%x.\n",
            g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_len,
            g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_err_len);

        return CPM_SEND_ERR;
    }

    /* ?????????????????????????????????????????????????????????????????????????????????????????? */
    diag_crit("vcom cnf cmd success, cnf sum len 0x%x, cnf err len 0x%x.\n",
        g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_len, g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_err_len);

    return CPM_SEND_OK;
}

/*
 * ??? ??? ???: PPM_VComCfgEvtCB
 * ????????????: ??????VCOM????????????????????????
 * ????????????: chan_id :?????????
 * ulEvent:   ??????????????????
 */
void ppm_vcom_event_cb(u32 chan, u32 event)
{
    unsigned int logic_port;
    bool send_flag = false;

    diag_crit("ppm_vcom_event_cb Chan:%s Event:%s.\n", (chan == PPM_VCOM_CHAN_CTRL) ? "cnf" : "ind",
        (event == PPM_VCOM_EVT_CHAN_OPEN) ? "open" : "close");

    if (chan == PPM_VCOM_CHAN_CTRL) {
        logic_port = CPM_OM_CFG_COMM;
    } else if (chan == PPM_VCOM_CHAN_DATA) {
        logic_port = CPM_OM_IND_COMM;
    } else {
        diag_error("Error channel NO %d\n", chan);
        return;
    }

    /* ???????????????????????? */
    if (event == PPM_VCOM_EVT_CHAN_OPEN) {
        diag_crit("ppm_vcom_event_cb open, do nothing.\n");
        return;
    } else if (event == PPM_VCOM_EVT_CHAN_CLOSE) {
        send_flag = false;

        if ((cpm_query_phy_port(CPM_OM_CFG_COMM) == CPM_VCOM_CFG_PORT) &&
            (cpm_query_phy_port(CPM_OM_IND_COMM) == CPM_VCOM_IND_PORT)) {
            send_flag = true;
        }
        if (send_flag == true) {
            diag_crit("ppm_vcom_event_cb close, disconnect all ports.\n");
            ppm_disconnect_port(logic_port);
        }
    } else {
        diag_error("Error Event State %d\n", event);
    }

    return;
}

/*
 * ??? ??? ???: ppm_vcom_cfg_read_data
 * ????????????: NAS?????????????????????OM ????????????
 * ????????????:  ucDevIndex: ????????????
 * data    : ????????????
 * uslength : ????????????
 */
u32 ppm_vcom_cfg_read_data(u32 index, u8 *data, u32 len)
{
    u32 ret;

    if (index != PPM_VCOM_CHAN_CTRL) {
        diag_error("PhyPort port is error: %d\n", index);

        return ERR_MSP_FAILURE;
    }

    g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_rcv_num++;
    g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_rcv_len += len;

    if ((data == NULL) || (len == 0)) {
        diag_error("Send data is NULL\n");

        return ERR_MSP_FAILURE;
    }

    /* ?????????????????????????????????????????????????????????????????????????????????????????? */
    diag_crit("vcom receive cmd, length : 0x%x, sum length : 0x%x.\n", len,
        g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_rcv_len);

    ret = cpm_com_receive(CPM_VCOM_CFG_PORT, data, len);
    if (ret != BSP_OK) {
        diag_error("cpm_com_receive error(0x%x)\n", ret);
        g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_rcv_err_num++;
        g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_rcv_err_len += len;
    }

    return BSP_OK;
}

/*
 * ??? ??? ???: ppm_vcom_ind_send_data
 * ????????????: Vcom????????????OM IND????????????????????????NAS??????
 * ????????????: pucVirAddr:   ???????????????
 * pucPhyAddr:   ???????????????
 * data_len:    ????????????
 */
u32 ppm_vcom_ind_send_data(u8 *virt_addr, u8 *phy_addr, u32 len)
{
    u32 in_slice;
    u32 out_slice;
    u32 write_slice;
    int ret;

    g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_num++;
    g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_len += len;

    in_slice = bsp_get_slice_value();
    ret = PPM_VCOM_SEND_DATA(PPM_VCOM_CHAN_DATA, virt_addr, len, PPM_VCOM_DATA_MODE_TRANSPARENT);

    diag_system_debug_vcom_len(len);
    diag_system_debug_send_data_end();
    out_slice = bsp_get_slice_value();

    write_slice = (in_slice > out_slice) ? (0xffffffff - in_slice + out_slice) : (out_slice - in_slice);

    if (write_slice > g_ppm_vcom_debug_info[CPM_OM_IND_COMM].max_time_len) {
        g_ppm_vcom_debug_info[CPM_OM_IND_COMM].max_time_len = write_slice;
    }

    if (ret != 0) {
        g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_err_num++;
        g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_err_len += len;

        diag_system_debug_vcom_fail_len(len);

        return CPM_SEND_ERR;
    }
    g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_sn++;
    diag_system_debug_vcom_sucess_len(len);

    return CPM_SEND_OK;
}

mdrv_ppm_vcom_debug_info_s *ppm_vcom_get_ind_info(void)
{
    return &g_ppm_vcom_debug_info[CPM_OM_IND_COMM];
}

/*
 * ??? ??? ???: ppm_vcom_cfg_port_init
 * ????????????: ?????? Vcom ???OM CFG??????????????????
 */
void ppm_vcom_cfg_port_init(void)
{
    struct diag_vcom_cb_ops ops;

    ops.event_cb = ppm_vcom_event_cb;
    ops.data_rx_cb = ppm_vcom_cfg_read_data;
    diag_vcom_register_ops(PPM_VCOM_CHAN_CTRL, &ops);

    cpm_phy_send_reg(CPM_VCOM_CFG_PORT, ppm_vcom_cfg_send_data);

    return;
}

/*
 * ??? ??? ???: ppm_vcom_ind_port_init
 * ????????????: ?????? Vcom ???OM IND??????????????????
 */
void ppm_vcom_ind_port_init(void)
{
    struct diag_vcom_cb_ops ops;

    ops.event_cb = ppm_vcom_event_cb;
    ops.data_rx_cb = NULL;
    diag_vcom_register_ops(PPM_VCOM_CHAN_DATA, &ops);

    cpm_phy_send_reg(CPM_VCOM_IND_PORT, ppm_vcom_ind_send_data);

    return;
}

/*
 * ??? ??? ???: ppm_vcom_port_init
 * ????????????: ?????? Vcom ????????????????????????
 */
void ppm_vcom_port_init(void)
{
    (void)memset_s(g_ppm_vcom_debug_info, sizeof(g_ppm_vcom_debug_info), 0, sizeof(g_ppm_vcom_debug_info));

    /* Vcom ???OM IND?????????????????? */
    ppm_vcom_ind_port_init();

    /* Vcom ???OM CNF?????????????????? */
    ppm_vcom_cfg_port_init();

    return;
}

/*
 * ??? ??? ???: ppm_vcom_info_show
 * ????????????: ???????????? Vcom ?????????????????????
 */
void ppm_vcom_info_show(void)
{
    diag_crit(" VCom28 Send num is %d.\n", g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_num);
    diag_crit(" VCom28 Send Len is %d.\n", g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_len);

    diag_crit(" VCom28 Send Error num is %d.\n", g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_err_num);
    diag_crit(" VCom28 Send Error Len is %d.\n", g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_send_err_len);

    diag_crit(" VCom28 receive num is %d.\n", g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_rcv_num);
    diag_crit(" VCom28 receive Len is %d.\n", g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_rcv_len);

    diag_crit(" VCom28 receive Error num is %d.\n", g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_rcv_err_num);
    diag_crit(" VCom28 receive Error Len is %d.\n\n", g_ppm_vcom_debug_info[CPM_OM_CFG_COMM].vcom_rcv_err_len);

    diag_crit(" VCom31 Send num is %d.\n", g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_num);
    diag_crit(" VCom31 Send Len is %d.\n", g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_len);
    diag_crit(" VCom31 Send sn is %d.\n", g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_sn);

    diag_crit(" VCom31 Send Error num is %d.\n", g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_err_num);
    diag_crit(" VCom31 Send Error Len is %d.\n", g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_send_err_len);

    diag_crit(" VCom31 receive num is %d.\n", g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_rcv_num);
    diag_crit(" VCom31 receive Len is %d.\n", g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_rcv_len);

    diag_crit(" VCom31 receive Error num is %d.\n", g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_rcv_err_num);
    diag_crit(" VCom31 receive Error Len is %d.\n", g_ppm_vcom_debug_info[CPM_OM_IND_COMM].vcom_rcv_err_len);

    return;
}
EXPORT_SYMBOL(ppm_vcom_info_show);

