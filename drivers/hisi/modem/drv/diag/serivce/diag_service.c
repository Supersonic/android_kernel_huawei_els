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

#include "diag_service.h"
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <mdrv.h>
#include <securec.h>
#include <bsp_slice.h>
#include <osl_spinlock.h>
#include <osl_sem.h>
#include <osl_list.h>
#include <osl_malloc.h>
#include <osl_sem.h>
#include "msp_service.h"
#include "soc_socp_adapter.h"
#include "diag_system_debug.h"
#include "scm_ind_src.h"
#include "scm_cnf_src.h"


#define THIS_MODU mod_diag

#define DIAG_PKT_TIMEOUT_LEN (32768 * 3) /* 3s */

spinlock_t g_ind_src_buf_spin_lock;
spinlock_t g_cnf_src_buf_spin_lock;

diag_srv_proc_cb g_diag_srv_fn = NULL;

typedef struct {
    osl_sem_id sem_id;
    struct list_head list_header;
} diag_srv_list_s;

diag_srv_list_s g_diag_srv_list = {};

diag_srv_ctrl_s g_diag_srv_ctrl = {
    SOCP_CODER_SRC_PS_IND,
    SOCP_CODER_SRC_CNF,
};

typedef struct {
    diag_service_head_s stHead;
    u32 timestamp; /* ????????????????????????????????????????????? */
    u32 ulFrameDataLen;
    u32 ulFrameOffset;
    struct list_head FrameList;
    diag_frame_head_s *pFrame;
} diag_srv_node_s;

void diag_srv_fill_head(diag_srv_head_s *srv_head)
{
    u32 timestamp_low = 0;
    u32 timestamp_high = 0;

    /* ??????????????????????????? */
#ifdef DIAG_SYSTEM_5G

    srv_head->socp_header.hisi_magic = DIAG_SOCP_HEAD_MAGIC;
    srv_head->socp_header.data_len = 0;

    srv_head->frame_header.srv_head.sid_4b = DIAG_FRAME_MSP_SID_DIAG_SERVICE;
    srv_head->frame_header.srv_head.ver_4b = DIAG_FRAME_MSP_VER_5G;
    srv_head->frame_header.srv_head.mdmid_3b = 0;
    srv_head->frame_header.srv_head.rsv_5b = 0;
    srv_head->frame_header.srv_head.ssid_8b = DIAG_FRAME_SSID_APP_CPU;
    /* ????????????????????? */
    srv_head->frame_header.srv_head.mt_2b = DIAG_FRAME_MT_IND;
    srv_head->frame_header.srv_head.index_4b = 0;
    srv_head->frame_header.srv_head.eof_1b = 0;
    /* ??????????????? */
    srv_head->frame_header.srv_head.ff_1b = 0;
    srv_head->frame_header.srv_head.socp_encdst_id = 0;
    srv_head->frame_header.srv_head.msg_trans_id = 0;

    (void)mdrv_timer_get_accuracy_timestamp(&timestamp_high, &timestamp_low);
    srv_head->frame_header.srv_head.timestamp[0] = timestamp_low;

#else
    srv_head->socp_header.hisi_magic = DIAG_SOCP_HEAD_MAGIC;
    srv_head->socp_header.data_len = 0;

    srv_head->frame_header.srv_head.sid_8b = DIAG_FRAME_MSP_SID_DIAG_SERVICE;
    srv_head->frame_header.srv_head.mdmid_3b = 0;
    srv_head->frame_header.srv_head.rsv_1b = 0;
    srv_head->frame_header.srv_head.ssid_4b = DIAG_FRAME_SSID_APP_CPU;
    srv_head->frame_header.srv_head.sessionid_8b = MSP_SERVICE_SESSION_ID;
    /* ????????????????????? */
    srv_head->frame_header.srv_head.mt_2b = DIAG_FRAME_MT_IND;
    srv_head->frame_header.srv_head.index_4b = 0;
    srv_head->frame_header.srv_head.eof_1b = 0;
    srv_head->frame_header.srv_head.ff_1b = 0;
    srv_head->frame_header.srv_head.msg_trans_id = 0;

    (void)mdrv_timer_get_accuracy_timestamp(&timestamp_high, &timestamp_low);
    srv_head->frame_header.srv_head.timestamp[0] = timestamp_low;
    srv_head->frame_header.srv_head.timestamp[1] = timestamp_high;
#endif
}
/*
 * Function Name: diag_srv_pkt_timeout_clear
 * Description: ????????????????????????????????????????????????????????????
 */
void diag_srv_pkt_timeout_clear(void)
{
    diag_srv_node_s *p_curr_node = NULL;
    diag_srv_node_s *p_next_node = NULL;
    u32 cur_slice = bsp_get_slice_value();

    osl_sem_down(&g_diag_srv_list.sem_id);

    list_for_each_entry_safe(p_curr_node, p_next_node, &g_diag_srv_list.list_header, FrameList)
    {
        if ((cur_slice > p_curr_node->timestamp) && ((cur_slice - p_curr_node->timestamp) > DIAG_PKT_TIMEOUT_LEN)) {
            if ((p_curr_node->FrameList.next != NULL) && (p_curr_node->FrameList.prev != NULL)) {
                list_del(&(p_curr_node->FrameList));
            }

            diag_crit("diag_srv_pkt_timeout_clear delete node.\n");

            if (p_curr_node->pFrame != NULL) {
                osl_free(p_curr_node->pFrame);
            }

            osl_free(p_curr_node);
        }
    }

    (void)osl_sem_up(&(g_diag_srv_list.sem_id));
}

/*
 * Function Name: diag_srv_create_pkt
 * Description: ?????????????????????????????????????????????
 * 1.         2015-03-18  Draft Enact
 */
/*lint -save -e429*/
u32 diag_srv_create_pkt(diag_frame_head_s *frame)
{
    struct list_head *me = NULL;
    diag_srv_node_s *tmp_node = NULL;
    errno_t ret;

    /* ???????????????????????????????????? */
    if (frame->msg_len + sizeof(diag_frame_head_s) > DIAG_FRAME_SUM_LEN) {
        diag_error("msg len too large, msg_len = 0x%x\n", frame->msg_len);
        return ERR_MSP_INALID_LEN_ERROR;
    }

    /* ??????????????????????????????transid???????????????????????? */
    osl_sem_down(&g_diag_srv_list.sem_id);

    list_for_each(me, &g_diag_srv_list.list_header)
    {
        tmp_node = list_entry(me, diag_srv_node_s, FrameList);

        tmp_node->stHead.index_4b = frame->srv_head.index_4b;
        tmp_node->stHead.eof_1b = frame->srv_head.eof_1b;
        tmp_node->stHead.ff_1b = frame->srv_head.ff_1b;
        if (memcmp(&tmp_node->stHead, &frame->srv_head, sizeof(diag_service_head_s)) == 0) {
            osl_sem_up(&g_diag_srv_list.sem_id);
            diag_error("there is a uniform packet in list.\n");
            return ERR_MSP_FAILURE;
        }
    }
    osl_sem_up(&g_diag_srv_list.sem_id);

    /* ??????????????????????????? */
    tmp_node = (diag_srv_node_s *)osl_malloc(sizeof(diag_srv_node_s));
    if (tmp_node == NULL) {
        diag_error("malloc pTempNode fail\n");
        return ERR_MSP_DIAG_ALLOC_MALLOC_FAIL;
    }

    tmp_node->ulFrameDataLen = frame->msg_len + sizeof(diag_frame_head_s);
    tmp_node->ulFrameOffset = 0;
    tmp_node->pFrame = (diag_frame_head_s *)osl_malloc(tmp_node->ulFrameDataLen);
    if (tmp_node->pFrame == NULL) {
        diag_error("malloc pFrame fail\n");
        osl_free(tmp_node);
        return ERR_MSP_DIAG_ALLOC_MALLOC_FAIL;
    }

    osl_sem_down(&g_diag_srv_list.sem_id);

    ret = memcpy_s(&tmp_node->stHead, (u32)sizeof(tmp_node->stHead), &frame->srv_head, sizeof(frame->srv_head));
    if (ret != EOK) {
        diag_error("memory copy fail %x\n", ret);
    }

    list_add_tail(&tmp_node->FrameList, &g_diag_srv_list.list_header);

    tmp_node->timestamp = mdrv_timer_get_normal_timestamp();

    osl_sem_up(&g_diag_srv_list.sem_id);

    return ERR_MSP_SUCCESS;
}
/*lint -restore +e429*/
/*
 * Function Name: diag_srv_save_pkt
 * Description: ????????????????????????????????????????????????
 * History:
 * 1.         2015-03-18  Draft Enact
 */
diag_frame_head_s *diag_srv_save_pkt(diag_frame_head_s *frame, u32 len)
{
    struct list_head *me = NULL;
    diag_srv_node_s *tmp_node = NULL;
    u32 ops_len;
    u32 offset;
    u32 local_len;
    errno_t ret;

    osl_sem_down(&g_diag_srv_list.sem_id);

    list_for_each(me, &g_diag_srv_list.list_header)
    {
        tmp_node = list_entry(me, diag_srv_node_s, FrameList);

        /* ????????????stService???4G ???5G????????? */
        tmp_node->stHead.index_4b = frame->srv_head.index_4b;
        tmp_node->stHead.eof_1b = frame->srv_head.eof_1b;
        tmp_node->stHead.ff_1b = frame->srv_head.ff_1b;
        if (memcmp(&tmp_node->stHead, &frame->srv_head, sizeof(diag_service_head_s)) == 0) {
            if (frame->srv_head.index_4b == 0) { /* ???0??? */
                /* ???0???????????????header, cmd_id, meglen and data */
                ret = memcpy_s(tmp_node->pFrame, tmp_node->ulFrameDataLen, frame, len);
                if (ret != EOK) {
                    diag_error("memory copy fail %x\n", ret);
                }
                tmp_node->ulFrameOffset = len;
            } else if (frame->srv_head.eof_1b) { /* ??????1??? */
                /* ????????????????????????????????????????????? */
                ops_len = tmp_node->ulFrameOffset - sizeof(diag_frame_head_s);
                local_len = len - sizeof(diag_service_head_s);

                if ((tmp_node->pFrame == NULL) || (tmp_node->ulFrameOffset + local_len > DIAG_FRAME_SUM_LEN) ||
                    (tmp_node->pFrame->msg_len != ops_len + local_len) ||
                    (tmp_node->ulFrameDataLen < tmp_node->ulFrameOffset)) {
                    diag_error("rev data len error, length:0x%x ulLocalLen:0x%x\n", ops_len, local_len);
                    osl_sem_up(&g_diag_srv_list.sem_id);
                    return NULL;
                }

                /* ???????????????????????? */
                ops_len = tmp_node->pFrame->msg_len - ops_len;

                /* ???????????????????????? */
                offset = tmp_node->ulFrameOffset;

                /* ?????????????????????????????????data */
                ret = memcpy_s(((u8 *)tmp_node->pFrame) + offset, tmp_node->ulFrameDataLen - tmp_node->ulFrameOffset,
                    ((u8 *)frame) + sizeof(diag_service_head_s), ops_len);
                if (ret != EOK) {
                    diag_error("memory copy fail %x\n", ret);
                }
                tmp_node->ulFrameOffset += ops_len;
            } else {
                /* ???????????????????????? */
                offset = tmp_node->ulFrameOffset;
                local_len = len - sizeof(diag_service_head_s);

                if ((tmp_node->pFrame == NULL) || (offset + local_len > DIAG_FRAME_SUM_LEN) ||
                    (tmp_node->pFrame->msg_len < (offset - sizeof(diag_frame_head_s) + local_len)) ||
                    (tmp_node->ulFrameDataLen < offset)) {
                    diag_error("msg len error, uloffset:0x%x ulLocallen:0x%x\n", offset, local_len);
                    osl_sem_up(&g_diag_srv_list.sem_id);
                    return NULL;
                }

                /* ?????????????????????cmdid???????????????????????????data */
                ret = memcpy_s(((u8 *)tmp_node->pFrame) + offset, (u32)(tmp_node->ulFrameDataLen - offset),
                    ((u8 *)frame) + sizeof(diag_service_head_s), local_len);
                if (ret != EOK) {
                    diag_error("memory copy fail %x\n", ret);
                }
                tmp_node->ulFrameOffset += local_len;
            }

            osl_sem_up(&g_diag_srv_list.sem_id);
            return tmp_node->pFrame;
        }
    }

    osl_sem_up(&g_diag_srv_list.sem_id);

    return (diag_frame_head_s *)NULL;
}

/*
 * Function Name: diag_srv_destroy_pkt
 * Description: ?????????????????????
 */
void diag_srv_destroy_pkt(diag_frame_head_s *frame)
{
    struct list_head *me = NULL;
    diag_srv_node_s *tmp_node = NULL;

    osl_sem_down(&g_diag_srv_list.sem_id);

    list_for_each(me, &g_diag_srv_list.list_header)
    {
        tmp_node = list_entry(me, diag_srv_node_s, FrameList);
        if (tmp_node == NULL) {
            osl_sem_up(&g_diag_srv_list.sem_id);
            return;
        }

        /* ????????????stService???4G ???5G????????? */
        tmp_node->stHead.index_4b = frame->srv_head.index_4b;
        tmp_node->stHead.eof_1b = frame->srv_head.eof_1b;
        tmp_node->stHead.ff_1b = frame->srv_head.ff_1b;
        if (memcmp(&tmp_node->stHead, &frame->srv_head, sizeof(diag_service_head_s)) == 0) {
            /* ???????????? */
            if ((tmp_node->FrameList.next != NULL) && (tmp_node->FrameList.prev != NULL)) {
                list_del(&tmp_node->FrameList);
            }

            if (tmp_node->pFrame != NULL) {
                osl_free(tmp_node->pFrame);
            }

            osl_free(tmp_node);
            break;
        }
    }

    osl_sem_up(&g_diag_srv_list.sem_id);

    return;
}

u32 diag_srv_get_data(diag_frame_head_s *rcv_head, u32 len, diag_frame_head_s **new_head)
{
    diag_frame_head_s *tmp_frame = NULL;

    if (rcv_head->srv_head.ff_1b) {
        /* ???????????????????????????????????????????????? */
        diag_srv_pkt_timeout_clear();

        /* index4b??????????????????16, ?????????????????????????????????16,??????index???????????????16 */
        if (rcv_head->srv_head.index_4b == 0) {
            diag_srv_create_pkt(rcv_head);
            (void)diag_srv_save_pkt(rcv_head, len);
            return ERR_MSP_SUCCESS;
        } else if (rcv_head->srv_head.eof_1b) {
            tmp_frame = diag_srv_save_pkt(rcv_head, len);
            if (tmp_frame == NULL) {
                return ERR_MSP_FAILURE;
            }
            /* 5G?????????????????????????????????5G??????,4G????????????????????????4G????????? */
            len = tmp_frame->msg_len + sizeof(diag_frame_head_s);

            *new_head = tmp_frame;
            return SERVICE_MSG_PROC;
        } else {
            (void)diag_srv_save_pkt(rcv_head, len);
            return ERR_MSP_SUCCESS;
        }
    } else {
#ifdef DIAG_SYSTEM_5G
        if ((rcv_head->srv_head.ver_4b == DIAG_FRAME_MSP_VER_5G) &&
            (len < rcv_head->msg_len + sizeof(diag_frame_head_s))) {
            diag_error("rev tools data len error, rev:0x%x except:0x%x\n", len,
                rcv_head->msg_len + (u32)sizeof(diag_frame_head_s));
            return ERR_MSP_INVALID_PARAMETER;
        }
#else
        if (len < rcv_head->msg_len + sizeof(diag_frame_head_s)) {
            diag_error("rev tools data len error, rev:0x%x except:0x%x\n", len,
                rcv_head->msg_len + (u32)sizeof(diag_frame_head_s));
            return ERR_MSP_INVALID_PARAMETER;
        }
#endif
        *new_head = rcv_head;

        return SERVICE_MSG_PROC;
    }
}

/*
 * Function Name: diag_srv_proc
 * Description: DIAG service ????????????
 */
u32 diag_srv_proc(void *data, u32 len)
{
    u32 ret;
    diag_frame_head_s *rcv_head = NULL;
    diag_frame_head_s *new_head = NULL;

    if ((data == NULL) || (len < sizeof(diag_service_head_s))) {
        diag_ptr(DIAG_PTR_SERVICE_IN, 1, (u32)(uintptr_t)data, len);
        diag_error("para error:data_len=0x%x\n", len);
        return BSP_ERROR;
    }

    rcv_head = (diag_frame_head_s *)data;

    diag_ptr(DIAG_PTR_DIAG_SERVICE_ERR, 1, rcv_head->cmd_id, 0);

    /* ?????????DIAG?????? */
    if (SERVICE_HEAD_SID(data) == DIAG_FRAME_MSP_SID_DIAG_SERVICE) {
        /* ?????????????????????????????? */
        __pm_stay_awake(&g_diag_srv_ctrl.stWakelock);

        ret = diag_srv_get_data(rcv_head, len, &new_head);
        if (ret != SERVICE_MSG_PROC) {
            diag_ptr(DIAG_PTR_SERVICE_WAIT, 1, rcv_head->cmd_id, ret);
            __pm_relax(&g_diag_srv_ctrl.stWakelock);
            return ret;
        }

        if (g_diag_srv_fn && (new_head != NULL)) {
            diag_ptr(DIAG_PTR_SERVICE_OK, 1, rcv_head->cmd_id, rcv_head->msg_len);
            ret = g_diag_srv_fn((void *)new_head);
        } else {
            ret = ERR_MSP_NO_INITILIZATION;
            diag_ptr(DIAG_PTR_DIAG_SERVICE_ERR, 1, (u32)(uintptr_t)new_head, ret);
        }

        if (rcv_head->srv_head.ff_1b) {
            diag_srv_destroy_pkt(rcv_head);
        }

        /* ??????????????????????????? */
        __pm_relax(&g_diag_srv_ctrl.stWakelock);
    } else {
        diag_ptr(DIAG_PTR_DIAG_SERVICE_ERR, 1, rcv_head->cmd_id, SERVICE_HEAD_SID(data));
        ret = ERR_MSP_INVALID_PARAMETER;
    }

    return ret;
}

/*
 * Function Name: diag_srv_proc_func_reg
 * Description: DIAG service??????????????????
 * History:
 * 1.         2014-11-18  Draft Enact
 */
void diag_srv_proc_func_reg(diag_srv_proc_cb srv_fn)
{
    g_diag_srv_fn = srv_fn;
}

void diag_srv_init(void)
{
    spin_lock_init(&g_ind_src_buf_spin_lock);
    spin_lock_init(&g_cnf_src_buf_spin_lock);

    wakeup_source_init(&g_diag_srv_ctrl.stWakelock, "diag_srv_lock");

    /* ??????????????????????????? */
    osl_sem_init(1, &g_diag_srv_list.sem_id);

    /* ????????????????????? */
    INIT_LIST_HEAD(&g_diag_srv_list.list_header);

    msp_srv_proc_func_reg(DIAG_FRAME_MSP_SID_DIAG_SERVICE, diag_srv_proc);

    return;
}

/*
 * ??? ??? ???:diag_srv_write_data
 * ????????????: ??????????????????????????????
 * ????????????: id????????????????????????
 * ????????????: ???
 * ??? ??? ???: ??????????????????
 */
void diag_srv_write_data(socp_buffer_rw_s *rw_buffer, const void *payload, u32 len)
{
    u32 size;
    u32 rb_size;
    errno_t ret;

    if (len == 0) {
        return;
    }

    if (rw_buffer->size > len) {
        ret = memcpy_s(((u8 *)rw_buffer->buffer), rw_buffer->size, payload, len);
        if (ret != EOK) {
            diag_error("memory copy fail %x\n", ret);
        }
        rw_buffer->buffer = rw_buffer->buffer + len;
        rw_buffer->size = rw_buffer->size - len;
    } else {
        size = rw_buffer->size;
        ret = memcpy_s(((u8 *)rw_buffer->buffer), rw_buffer->size, payload, size);
        if (ret != EOK) {
            diag_error("memory copy fail %x\n", ret);
        }

        rb_size = len - rw_buffer->size;
        if (rb_size && rw_buffer->rb_buffer != NULL) {
            ret = memcpy_s(rw_buffer->rb_buffer, rw_buffer->rb_size, ((u8 *)payload + size), rb_size);
            if (ret != EOK) {
                diag_error("memory copy fail %x\n", ret);
            }
        }

        rw_buffer->buffer = rw_buffer->rb_buffer + rb_size;
        rw_buffer->size = rw_buffer->rb_size - rb_size;
        rw_buffer->rb_buffer = NULL;
        rw_buffer->rb_size = 0;
    }

    return;
}

/*
 * Function Name: diag_SrvcPackFirst
 * Description: ?????????????????????????????????????????????????????????
 */
u32 diag_srv_ind_send(diag_msg_report_head_s *msg)
{
    socp_buffer_rw_s socp_buf = { NULL, NULL, 0, 0 };
    diag_socp_head_s *socp_header;
    u32 tmp_len;
    u32 ret;
    unsigned long lock;

    tmp_len = ALIGN_DDR_WITH_8BYTE(msg->header_size + msg->data_size);

    socp_header = &(((diag_srv_head_s *)(msg->header))->socp_header);
    socp_header->data_len = msg->header_size + msg->data_size - sizeof(diag_socp_head_s);

    spin_lock_irqsave(&g_ind_src_buf_spin_lock, lock);

    if (mdrv_socp_get_write_buff(g_diag_srv_ctrl.ind_chan_id, &socp_buf)) {
        /* no print because it is a noraml way */
        spin_unlock_irqrestore(&g_ind_src_buf_spin_lock, lock);
        return ERR_MSP_GET_WRITE_BUFF_FAIL; /* ???????????? */
    }

    diag_ps_buffer_overflow_record(socp_buf.size + socp_buf.rb_size);

    /* ?????????????????? */
    if ((socp_buf.size + socp_buf.rb_size) >= (tmp_len)) {
        socp_buf.buffer = (char *)scm_ind_src_phy_to_virt((u8 *)socp_buf.buffer);
        socp_buf.rb_buffer = (char *)scm_ind_src_phy_to_virt((u8 *)socp_buf.rb_buffer);
    } else {
        spin_unlock_irqrestore(&g_ind_src_buf_spin_lock, lock);
        diag_ps_ind_lost_record(tmp_len);
        return ERR_MSP_NOT_FREEE_SPACE;
    }

    if (socp_buf.buffer == NULL) {
        spin_unlock_irqrestore(&g_ind_src_buf_spin_lock, lock);
        diag_error("get socp buffer is null\n");
        return ERR_MSP_GET_VIRT_ADDR_FAIL;
    }

    diag_srv_write_data(&socp_buf, msg->header, msg->header_size);
    diag_srv_write_data(&socp_buf, msg->data, msg->data_size);

    ret = (u32)mdrv_socp_write_done(g_diag_srv_ctrl.ind_chan_id, (tmp_len));
    spin_unlock_irqrestore(&g_ind_src_buf_spin_lock, lock);
    if (ret) {
        diag_error("write done fail,ret:0x%x\n", ret);
        return ERR_MSP_WRITE_DONE_FAIL;
    }

    return ERR_MSP_SUCCESS;
}

/*
 * Function Name: diag_srv_pack_ind_data
 * Description: DIAG service???????????????????????????
 * History:
 * 1.         2014-11-18  Draft Enact
 * 2.         2015-03-14  ????????????????????????
 * ????????????????????????????????????????????????:
 * A. ????????????ulCmdId???ulMsgLen???????????????????????????
 * B. ?????????????????????????????????????????????????????????????????????
 * C. transid???timestamp?????????????????????????????????
 */
u32 diag_srv_pack_ind_data(diag_msg_report_head_s *msg)
{
    diag_msg_report_head_s report_info;
    diag_frame_head_s *frame_header = NULL;
    u32 ret;
    s32 data_len; /* ??????????????? */
    u32 cur_len;  /* ?????????????????????????????? */
    u32 pack_index = 0;
    u32 frame_header_size;
    u32 send_len = 0;

    frame_header_size = msg->header_size - sizeof(diag_socp_head_s);

    /* ?????????????????????????????? */
    data_len = (s32)(frame_header_size + DIAG_SERVCIE_GET_REAL_LEN(msg->data_size));
    if (data_len > (s32)(DIAG_FRAME_SUM_LEN - 15 * sizeof(diag_frame_head_s))) {
        diag_error("diag report length is %d.\n", data_len);
        return ERR_MSP_FAILURE;
    }

    diag_ps_package_record(data_len);

    frame_header = &(((diag_srv_head_s *)(msg->header))->frame_header);

    cur_len = (u32)data_len;
    if (data_len > DIAG_FRAME_MAX_LEN) {
        cur_len = DIAG_FRAME_MAX_LEN;
        frame_header->srv_head.ff_1b = 1;
    } else {
        frame_header->srv_head.ff_1b = 0;
    }

    report_info.header = msg->header;
    report_info.header_size = msg->header_size;
    report_info.data = msg->data;
    report_info.data_size = cur_len - frame_header_size;
    send_len += report_info.data_size;

    /* ??????????????????????????????cmdid???????????????????????????????????????????????????????????? */
    ret = diag_srv_ind_send(&report_info);
    if (ret) {
        return ret;
    }

    /* ???????????? */
    if (frame_header->srv_head.ff_1b) {
        /* ??????????????????????????????????????? */
        data_len = data_len - (s32)cur_len;

        while (data_len > 0) {
            frame_header->srv_head.index_4b = ++pack_index;
            if ((data_len + sizeof(diag_service_head_s)) > DIAG_FRAME_MAX_LEN) {
                cur_len = DIAG_FRAME_MAX_LEN - sizeof(diag_service_head_s);
            } else {
                cur_len = (u32)data_len;
                frame_header->srv_head.eof_1b = 1; /* ???????????????????????? */
            }
            report_info.header = msg->header;
            report_info.header_size = sizeof(diag_socp_head_s) + sizeof(diag_service_head_s);
            report_info.data = (u8 *)(msg->data) + send_len;
            report_info.data_size = cur_len;
            send_len += report_info.data_size;

            ret = diag_srv_ind_send(&report_info);
            if (ret) {
                return ret;
            }

            data_len -= (s32)cur_len;
        }
    }

    return ERR_MSP_SUCCESS;
}

/*
 * Function Name: diag_srv_cnf_send
 * Description: diag_service???cnf????????????
 */
u32 diag_srv_cnf_send(diag_msg_report_head_s *msg)
{
    socp_buffer_rw_s socp_buf = { NULL, NULL, 0, 0 };
    diag_socp_head_s *socp_header;
    u32 tmp_len;
    u32 ret;
    unsigned long lock;

    tmp_len = ALIGN_DDR_WITH_8BYTE((msg->header_size + msg->data_size));

    socp_header = &(((diag_srv_head_s *)(msg->header))->socp_header);
    socp_header->data_len = msg->header_size + msg->data_size - sizeof(diag_socp_head_s);

    spin_lock_irqsave(&g_cnf_src_buf_spin_lock, lock);

    if (mdrv_socp_get_write_buff(g_diag_srv_ctrl.cnf_chan_id, &socp_buf)) {
        spin_unlock_irqrestore(&g_cnf_src_buf_spin_lock, lock);
        return ERR_MSP_GET_WRITE_BUFF_FAIL; /* ???????????? */
    }
    /* ?????????????????? */
    if ((socp_buf.size + socp_buf.rb_size) >= (tmp_len)) {
        socp_buf.buffer = (char *)scm_cnf_src_phy_to_virt((u8 *)socp_buf.buffer);
        socp_buf.rb_buffer = (char *)scm_cnf_src_phy_to_virt((u8 *)socp_buf.rb_buffer);
    } else {
        spin_unlock_irqrestore(&g_cnf_src_buf_spin_lock, lock);
        return ERR_MSP_NOT_FREEE_SPACE;
    }

    if (socp_buf.buffer == NULL) {
        spin_unlock_irqrestore(&g_cnf_src_buf_spin_lock, lock);
        diag_error("get socp buffer is null\n");
        return ERR_MSP_GET_VIRT_ADDR_FAIL;
    }

    diag_srv_write_data(&socp_buf, msg->header, msg->header_size);
    diag_srv_write_data(&socp_buf, msg->data, msg->data_size);

    ret = (u32)mdrv_socp_write_done(g_diag_srv_ctrl.cnf_chan_id, (tmp_len));
    spin_unlock_irqrestore(&g_cnf_src_buf_spin_lock, lock);
    if (ret) {
        diag_error("write done fail,ret:0x%x\n", ret);
        return ERR_MSP_WRITE_DONE_FAIL;
    }

    return ERR_MSP_SUCCESS;
}

/*
 * Function Name: diag_srv_pack_cnf_data
 * Description: DIAG service???????????????????????????
 * History:
 * 1.         2014-11-18  Draft Enact
 * 2.         2015-03-14  ????????????????????????
 * ????????????????????????????????????????????????:
 * A. ????????????ulCmdId???ulMsgLen???????????????????????????
 * B. ?????????????????????????????????????????????????????????????????????
 * C. transid???timestamp?????????????????????????????????
 */
u32 diag_srv_pack_cnf_data(diag_msg_report_head_s *msg)
{
    diag_msg_report_head_s report_info;
    diag_frame_head_s *frame_header;
    u32 ret;
    s32 data_len; /* ??????????????? */
    u32 cur_len;  /* ?????????????????????????????? */
    u32 pack_index = 0;
    u32 frame_header_size;
    u32 send_len = 0;

    frame_header_size = msg->header_size - sizeof(diag_socp_head_s);
    frame_header = &(((diag_srv_head_s *)(msg->header))->frame_header);

    diag_ptr(DIAG_PTR_SRV_PACK_IN, 1, frame_header->cmd_id, 0);

    /* ?????????????????????????????? */
    data_len = (s32)(frame_header_size + DIAG_SERVCIE_GET_REAL_LEN(msg->data_size));
    if (data_len > (s32)(DIAG_FRAME_SUM_LEN - 15 * sizeof(diag_frame_head_s))) {
        diag_error("diag report length is %d.\n", data_len);
        return ERR_MSP_FAILURE;
    }

    /* ??????????????? */
    frame_header = &(((diag_srv_head_s *)(msg->header))->frame_header);

    cur_len = (u32)data_len;
    if (data_len > DIAG_FRAME_MAX_LEN) {
        cur_len = DIAG_FRAME_MAX_LEN;
        frame_header->srv_head.ff_1b = 1;
    } else {
        frame_header->srv_head.ff_1b = 0;
    }

    report_info.header = msg->header;
    report_info.header_size = msg->header_size;
    report_info.data = msg->data;
    report_info.data_size = cur_len - frame_header_size;
    send_len += report_info.data_size;

    /* ??????????????????????????????cmdid???????????????????????????????????????????????????????????? */
    ret = diag_srv_cnf_send(&report_info);
    if (ret) {
        diag_ptr(DIAG_PTR_SRV_PACK_ERR, 1, frame_header->cmd_id, cur_len);
        return ret;
    }

    /* ???????????? */
    if (frame_header->srv_head.ff_1b) {
        /* ??????????????????????????????????????? */
        data_len = data_len - (s32)cur_len;

        while (data_len > 0) {
            frame_header->srv_head.index_4b = ++pack_index;
            if ((data_len + sizeof(diag_service_head_s)) > DIAG_FRAME_MAX_LEN) {
                cur_len = DIAG_FRAME_MAX_LEN - sizeof(diag_service_head_s);
            } else {
                cur_len = (u32)data_len;
                frame_header->srv_head.eof_1b = 1; /* ???????????????????????? */
            }
            report_info.header = msg->header;
            report_info.header_size = sizeof(diag_socp_head_s) + sizeof(diag_service_head_s);
            report_info.data = (u8 *)(msg->data) + send_len;
            report_info.data_size = cur_len;
            send_len += report_info.data_size;

            ret = diag_srv_cnf_send(&report_info);
            if (ret) {
                diag_ptr(DIAG_PTR_SRV_PACK_ERR, 1, frame_header->cmd_id, cur_len);
                return ret;
            }

            data_len -= (s32)cur_len;
        }
    }

    diag_ptr(DIAG_PTR_SRV_PACK_OK, 1, frame_header->cmd_id, data_len);
    return ERR_MSP_SUCCESS;
}

u32 diag_srv_pack_reset_data(diag_msg_report_head_s *msg)
{
    diag_msg_report_head_s report_info;
    diag_frame_head_s *frame_header;
    u32 ret;
    s32 data_len; /* ??????????????? */
    u32 cur_len;  /* ?????????????????????????????? */

    frame_header = &(((diag_srv_head_s *)(msg->header))->frame_header);

    /* ?????????????????????????????? */
    data_len = (s32)(msg->header_size + msg->data_size);
    if (data_len - sizeof(diag_socp_head_s) > (s32)(DIAG_FRAME_SUM_LEN - 15 * sizeof(diag_frame_head_s))) {
        diag_error("report length error(0x%x).\n", data_len);
        return ERR_MSP_FAILURE;
    }

    cur_len = (u32)data_len;
    if (data_len > DIAG_FRAME_MAX_LEN) {
        return ERR_MSP_INALID_LEN_ERROR;
    }

    /* ??????????????? */
    frame_header = &(((diag_srv_head_s *)(msg->header))->frame_header);
    frame_header->srv_head.mt_2b = DIAG_FRAME_MT_IND;
    frame_header->srv_head.ff_1b = 0;
    frame_header->msg_len = (u32)data_len - sizeof(diag_srv_head_s);

    report_info.header = msg->header;
    report_info.header_size = msg->header_size;
    report_info.data = msg->data;
    report_info.data_size = cur_len - msg->header_size;

    /* ??????????????????????????????cmdid???????????????????????????????????????????????????????????? */
    ret = diag_srv_cnf_send(&report_info);
    if (ret) {
        return ret;
    }

    return ERR_MSP_SUCCESS;
}

