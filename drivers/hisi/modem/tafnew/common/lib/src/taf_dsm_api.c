/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#include "ps_common_def.h"
#include "taf_ps_api.h"
#include "securec.h"
#include "mn_comm_api.h"

#if (OSA_CPU_CCPU == VOS_OSA_CPU)
#include "taf_dsm_mntn.h"
#include "taf_dsm_profile_com_func.h"
#include "taf_dsm_conn_com_func.h"
#endif


/*
 * ??????????????????????.C??????????
 */
#define THIS_FILE_ID PS_FILE_ID_TAF_DSM_API_C

/* Log Print Module Define */
#ifndef THIS_MODU
#define THIS_MODU ps_nas
#endif

VOS_UINT32 TAF_PS_SndDsmMsg(VOS_UINT16 modemId, VOS_UINT32 msgId, const VOS_VOID *data, VOS_UINT32 length)
{
    TAF_PS_Msg *msg = VOS_NULL_PTR;
    VOS_UINT32  sendPid;
    VOS_UINT32  rcvPid;
    VOS_UINT32  result;
    errno_t     memResult;

    switch (modemId) {
        case MODEM_ID_0:
            sendPid = I0_WUEPS_PID_TAF;
            rcvPid  = I0_UEPS_PID_DSM;
            break;

        case MODEM_ID_1:
            sendPid = I1_WUEPS_PID_TAF;
            rcvPid  = I1_UEPS_PID_DSM;
            break;

        case MODEM_ID_2:
            sendPid = I2_WUEPS_PID_TAF;
            rcvPid  = I2_UEPS_PID_DSM;
            break;

        default:
            PS_PRINTF_WARNING("<TAF_PS_SndDsmMsg> ModemId is error!\n");
            return VOS_ERR;
    }

#if (OSA_CPU_ACPU == VOS_OSA_CPU)
    sendPid = ACPU_PID_TAF;
#endif

    /* ???????? */
    msg = (TAF_PS_Msg *)TAF_AllocMsgWithHeaderLen(sendPid, sizeof(MSG_Header) + length);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    TAF_CfgMsgHdr((MsgBlock *)msg, sendPid, rcvPid, (sizeof(MSG_Header) + length - VOS_MSG_HEAD_LENGTH));

    msg->header.msgName = msgId;

    /* ???????????? */
    if (length > 0) {
        memResult = memcpy_s(msg->content, length, data, length);
        TAF_MEM_CHK_RTN_VAL(memResult, length, length);
    }

    /* ???????? */
    result = TAF_TraceAndSendMsg(sendPid, msg);

    if (result != VOS_OK) {
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 TAF_PS_SetPrimPdpContextInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                        TAF_PDP_PrimContextExt *pdpContextInfo)
{
    VOS_UINT32                      result = VOS_OK;
    TAF_PS_SetPrimPdpContextInfoReq setPdpCtxInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setPdpCtxInfoReq, sizeof(setPdpCtxInfoReq), 0x00, sizeof(TAF_PS_SetPrimPdpContextInfoReq));

    /* ????ID_MSG_TAF_PS_SET_PRIM_PDP_CONTEXT_INFO_REQ???? */
    setPdpCtxInfoReq.ctrl.moduleId  = moduleId;
    setPdpCtxInfoReq.ctrl.clientId  = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setPdpCtxInfoReq.ctrl.opId      = opId;
    setPdpCtxInfoReq.pdpContextInfo = *pdpContextInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId),
                              ID_MSG_TAF_PS_SET_PRIM_PDP_CONTEXT_INFO_REQ, &setPdpCtxInfoReq,
                              sizeof(TAF_PS_SetPrimPdpContextInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetPrimPdpContextInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32                      result = VOS_OK;
    TAF_PS_GetPrimPdpContextInfoReq getPdpCtxInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getPdpCtxInfoReq, sizeof(getPdpCtxInfoReq), 0x00, sizeof(TAF_PS_GetPrimPdpContextInfoReq));

    /* ????ID_MSG_TAF_PS_GET_PRIM_PDP_CONTEXT_INFO_REQ???? */
    getPdpCtxInfoReq.ctrl.moduleId = moduleId;
    getPdpCtxInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getPdpCtxInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId),
                              ID_MSG_TAF_PS_GET_PRIM_PDP_CONTEXT_INFO_REQ, &getPdpCtxInfoReq,
                              sizeof(TAF_PS_GetPrimPdpContextInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetPdpContextInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32                  result = VOS_OK;
    TAF_PS_GetPdpContextInfoReq getPdpCtxInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getPdpCtxInfoReq, sizeof(getPdpCtxInfoReq), 0x00, sizeof(TAF_PS_GetPdpContextInfoReq));

    /* ????ID_MSG_TAF_PS_GET_PRIM_PDP_CONTEXT_INFO_REQ???? */
    getPdpCtxInfoReq.ctrl.moduleId = moduleId;
    getPdpCtxInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getPdpCtxInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_PDPCONT_INFO_REQ,
                              &getPdpCtxInfoReq, sizeof(TAF_PS_GetPdpContextInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_SetSecPdpContextInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                       TAF_PDP_SecContextExt *pdpContextInfo)
{
    VOS_UINT32                     result = VOS_OK;
    TAF_PS_SetSecPdpContextInfoReq setPdpCtxInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setPdpCtxInfoReq, sizeof(setPdpCtxInfoReq), 0x00, sizeof(TAF_PS_SetSecPdpContextInfoReq));

    /* ????ID_MSG_TAF_PS_SET_SEC_PDP_CONTEXT_INFO_REQ???? */
    setPdpCtxInfoReq.ctrl.moduleId  = moduleId;
    setPdpCtxInfoReq.ctrl.clientId  = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setPdpCtxInfoReq.ctrl.opId      = opId;
    setPdpCtxInfoReq.pdpContextInfo = *pdpContextInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId),
                              ID_MSG_TAF_PS_SET_SEC_PDP_CONTEXT_INFO_REQ, &setPdpCtxInfoReq,
                              sizeof(TAF_PS_SetSecPdpContextInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetSecPdpContextInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32                     result = VOS_OK;
    TAF_PS_GetSecPdpContextInfoReq getPdpCtxInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getPdpCtxInfoReq, sizeof(getPdpCtxInfoReq), 0x00, sizeof(TAF_PS_GetSecPdpContextInfoReq));

    /* ????ID_MSG_TAF_PS_GET_SEC_PDP_CONTEXT_INFO_REQ???? */
    getPdpCtxInfoReq.ctrl.moduleId = moduleId;
    getPdpCtxInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getPdpCtxInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId),
                              ID_MSG_TAF_PS_GET_SEC_PDP_CONTEXT_INFO_REQ, &getPdpCtxInfoReq,
                              sizeof(TAF_PS_GetSecPdpContextInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_SetTftInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, TAF_TFT_Ext *tftInfo)
{
    VOS_UINT32           result = VOS_OK;
    TAF_PS_SetTftInfoReq setTftInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setTftInfoReq, sizeof(setTftInfoReq), 0x00, sizeof(TAF_PS_SetTftInfoReq));

    /* ????ID_MSG_TAF_PS_SET_TFT_INFO_REQ???? */
    setTftInfoReq.ctrl.moduleId = moduleId;
    setTftInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setTftInfoReq.ctrl.opId     = opId;
    setTftInfoReq.tftInfo       = *tftInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_TFT_INFO_REQ,
                              &setTftInfoReq, sizeof(TAF_PS_SetTftInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetTftInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32           result = VOS_OK;
    TAF_PS_GetTftInfoReq getTftInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getTftInfoReq, sizeof(getTftInfoReq), 0x00, sizeof(TAF_PS_GetTftInfoReq));

    /* ????ID_MSG_TAF_PS_GET_SEC_PDP_CONTEXT_INFO_REQ???? */
    getTftInfoReq.ctrl.moduleId = moduleId;
    getTftInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getTftInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_TFT_INFO_REQ,
                              &getTftInfoReq, sizeof(TAF_PS_GetTftInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_SetUmtsQosInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                 TAF_UMTS_QosExt *umtsQosInfo)
{
    VOS_UINT32               result = VOS_OK;
    TAF_PS_SetUmtsQosInfoReq setUmtsQosInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setUmtsQosInfoReq, sizeof(setUmtsQosInfoReq), 0x00, sizeof(TAF_PS_SetUmtsQosInfoReq));

    /* ????ID_MSG_TAF_PS_SET_UMTS_QOS_INFO_REQ???? */
    setUmtsQosInfoReq.ctrl.moduleId = moduleId;
    setUmtsQosInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setUmtsQosInfoReq.ctrl.opId     = opId;
    setUmtsQosInfoReq.umtsQosInfo   = *umtsQosInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_UMTS_QOS_INFO_REQ,
                              &setUmtsQosInfoReq, sizeof(TAF_PS_SetUmtsQosInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetUmtsQosInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32               result = VOS_OK;
    TAF_PS_GetUmtsQosInfoReq getUmtsQosInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getUmtsQosInfoReq, sizeof(getUmtsQosInfoReq), 0x00, sizeof(TAF_PS_GetUmtsQosInfoReq));

    /* ????ID_MSG_TAF_PS_GET_UMTS_QOS_INFO_REQ???? */
    getUmtsQosInfoReq.ctrl.moduleId = moduleId;
    getUmtsQosInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getUmtsQosInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_UMTS_QOS_INFO_REQ,
                              &getUmtsQosInfoReq, sizeof(TAF_PS_GetUmtsQosInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_SetUmtsQosMinInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                    TAF_UMTS_QosExt *umtsQosMinInfo)
{
    VOS_UINT32                  result = VOS_OK;
    TAF_PS_SetUmtsQosMinInfoReq setUmtsQosMinInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setUmtsQosMinInfoReq, sizeof(setUmtsQosMinInfoReq), 0x00, sizeof(TAF_PS_SetUmtsQosMinInfoReq));

    /* ????ID_MSG_TAF_PS_SET_UMTS_QOS_MIN_INFO_REQ???? */
    setUmtsQosMinInfoReq.ctrl.moduleId  = moduleId;
    setUmtsQosMinInfoReq.ctrl.clientId  = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setUmtsQosMinInfoReq.ctrl.opId      = opId;
    setUmtsQosMinInfoReq.umtsQosMinInfo = *umtsQosMinInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_UMTS_QOS_MIN_INFO_REQ,
                              &setUmtsQosMinInfoReq, sizeof(TAF_PS_SetUmtsQosMinInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetUmtsQosMinInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32                  result = VOS_OK;
    TAF_PS_GetUmtsQosMinInfoReq getUmtsQosMinInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getUmtsQosMinInfoReq, sizeof(getUmtsQosMinInfoReq), 0x00, sizeof(TAF_PS_GetUmtsQosMinInfoReq));

    /* ????ID_MSG_TAF_PS_GET_UMTS_QOS_MIN_INFO_REQ???? */
    getUmtsQosMinInfoReq.ctrl.moduleId = moduleId;
    getUmtsQosMinInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getUmtsQosMinInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_UMTS_QOS_MIN_INFO_REQ,
                              &getUmtsQosMinInfoReq, sizeof(TAF_PS_GetUmtsQosMinInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetDynamicUmtsQosInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                        TAF_CID_List *cidListInfo)
{
    VOS_UINT32                      result = VOS_OK;
    TAF_PS_GetDynamicUmtsQosInfoReq getDynamicUmtsQosInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getDynamicUmtsQosInfoReq, sizeof(getDynamicUmtsQosInfoReq), 0x00,
                       sizeof(TAF_PS_GetDynamicUmtsQosInfoReq));

    /* ????ID_MSG_TAF_PS_GET_DYNAMIC_UMTS_QOS_INFO_REQ???? */
    getDynamicUmtsQosInfoReq.ctrl.moduleId = moduleId;
    getDynamicUmtsQosInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getDynamicUmtsQosInfoReq.ctrl.opId     = opId;
    getDynamicUmtsQosInfoReq.cidListInfo   = *cidListInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId),
                              ID_MSG_TAF_PS_GET_DYNAMIC_UMTS_QOS_INFO_REQ, &getDynamicUmtsQosInfoReq,
                              sizeof(TAF_PS_GetDynamicUmtsQosInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_SetPdpContextState(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                     TAF_CID_ListState *cidListStateInfo)
{
    VOS_UINT32            result = VOS_OK;
    TAF_PS_SetPdpStateReq setPdpStateReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setPdpStateReq, sizeof(setPdpStateReq), 0x00, sizeof(TAF_PS_SetPdpStateReq));

    /* ????ID_MSG_TAF_PS_SET_PDP_STATE_REQ???? */
    setPdpStateReq.ctrl.moduleId    = moduleId;
    setPdpStateReq.ctrl.clientId    = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setPdpStateReq.ctrl.opId        = opId;
    setPdpStateReq.cidListStateInfo = *cidListStateInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ,
                              &setPdpStateReq, sizeof(TAF_PS_SetPdpStateReq));

    return result;
}

VOS_UINT32 TAF_PS_GetPdpContextState(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32            result = VOS_OK;
    TAF_PS_GetPdpStateReq getPdpStateReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getPdpStateReq, sizeof(getPdpStateReq), 0x00, sizeof(TAF_PS_GetPdpStateReq));

    /* ????ID_MSG_TAF_PS_GET_PDP_STATE_REQ???? */
    getPdpStateReq.ctrl.moduleId = moduleId;
    getPdpStateReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getPdpStateReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_PDP_CONTEXT_STATE_REQ,
                              &getPdpStateReq, sizeof(TAF_PS_GetPdpStateReq));

    return result;
}

VOS_UINT32 TAF_PS_CallModify(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, TAF_CID_List *cidListInfo)
{
    VOS_UINT32           result = VOS_OK;
    TAF_PS_CallModifyReq callModifyReq;

    /* ?????? */
    (VOS_VOID)memset_s(&callModifyReq, sizeof(callModifyReq), 0x00, sizeof(TAF_PS_CallModifyReq));

    /* ????ID_MSG_TAF_PS_CALL_MODIFY_REQ???? */
    callModifyReq.ctrl.moduleId = moduleId;
    callModifyReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    callModifyReq.ctrl.opId     = opId;
    callModifyReq.cidListInfo   = *cidListInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_CALL_MODIFY_REQ,
                              &callModifyReq, sizeof(TAF_PS_CallModifyReq));

    return result;
}

VOS_UINT32 TAF_PS_CallAnswer(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, TAF_PS_Answer *ansInfo)
{
    VOS_UINT32           result = VOS_OK;
    TAF_PS_CallAnswerReq callAnswerReq;

    /* ?????? */
    (VOS_VOID)memset_s(&callAnswerReq, sizeof(callAnswerReq), 0x00, sizeof(TAF_PS_CallAnswerReq));

    /* ????ID_MSG_TAF_PS_CALL_ANSWER_REQ???? */
    callAnswerReq.ctrl.moduleId = moduleId;
    callAnswerReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    callAnswerReq.ctrl.opId     = opId;
    callAnswerReq.ansInfo       = *ansInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_CALL_ANSWER_REQ,
                              &callAnswerReq, sizeof(TAF_PS_CallAnswerReq));

    return result;
}

VOS_UINT32 TAF_PS_CallHangup(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32           result = VOS_OK;
    TAF_PS_CallHangupReq callHangupReq;

    /* ?????? */
    (VOS_VOID)memset_s(&callHangupReq, sizeof(callHangupReq), 0x00, sizeof(TAF_PS_CallHangupReq));

    /* ????ID_MSG_TAF_PS_CALL_HANGUP_REQ???? */
    callHangupReq.ctrl.moduleId = moduleId;
    callHangupReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    callHangupReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_CALL_HANGUP_REQ,
                              &callHangupReq, sizeof(TAF_PS_CallHangupReq));

    return result;
}

MODULE_EXPORTED VOS_UINT32 TAF_PS_CallOrig(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                           TAF_PS_DialPara *dialParaInfo)
{
    errno_t            memResult;
    VOS_UINT32         result = VOS_OK;
    TAF_PS_CallOrigReq callOrigReq;

    /* ?????? */
    (VOS_VOID)memset_s(&callOrigReq, sizeof(callOrigReq), 0x00, sizeof(TAF_PS_CallOrigReq));

    /* ????ID_MSG_TAF_PS_CALL_ORIG_REQ???? */
    callOrigReq.ctrl.moduleId = moduleId;
    callOrigReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    callOrigReq.ctrl.opId     = opId;

    memResult = memcpy_s(&callOrigReq.dialParaInfo, sizeof(callOrigReq.dialParaInfo), dialParaInfo,
                         sizeof(TAF_PS_DialPara));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(callOrigReq.dialParaInfo), sizeof(TAF_PS_DialPara));

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_CALL_ORIG_REQ, &callOrigReq,
                              sizeof(TAF_PS_CallOrigReq));

    return result;
}

MODULE_EXPORTED VOS_UINT32 TAF_PS_CallEnd(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 cid)
{
    return TAF_PS_CallEndEx(moduleId, exClientId, opId, cid, TAF_PS_CALL_END_CAUSE_NORMAL);
}

MODULE_EXPORTED VOS_UINT32 TAF_PS_CallEndEx(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 cid,
                                            TAF_PS_CallEndCauseUint8 cause)
{
    VOS_UINT32        result = VOS_OK;
    TAF_PS_CallEndReq callEndReq;

    /* ?????? */
    (VOS_VOID)memset_s(&callEndReq, sizeof(callEndReq), 0x00, sizeof(TAF_PS_CallEndReq));

    /* ????ID_MSG_TAF_PS_CALL_END_REQ???? */
    callEndReq.ctrl.moduleId = moduleId;
    callEndReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    callEndReq.ctrl.opId     = opId;
    callEndReq.cid           = cid;
    callEndReq.cause         = cause;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_CALL_END_REQ, &callEndReq,
                              sizeof(TAF_PS_CallEndReq));

    return result;
}

VOS_UINT32 TAF_PS_GetPdpIpAddrInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                   TAF_CID_List *cidListInfo)
{
    VOS_UINT32                 result = VOS_OK;
    TAF_PS_GetPdpIpAddrInfoReq getPdpIpAddrReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getPdpIpAddrReq, sizeof(getPdpIpAddrReq), 0x00, sizeof(TAF_PS_GetPdpIpAddrInfoReq));

    /* ????ID_MSG_TAF_PS_GET_PDP_IP_ADDR_INFO_REQ???? */
    getPdpIpAddrReq.ctrl.moduleId = moduleId;
    getPdpIpAddrReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getPdpIpAddrReq.ctrl.opId     = opId;
    getPdpIpAddrReq.cidListInfo   = *cidListInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_PDP_IP_ADDR_INFO_REQ,
                              &getPdpIpAddrReq, sizeof(TAF_PS_GetPdpIpAddrInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_SetAnsModeInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT32 ansMode)
{
    VOS_UINT32                  result = VOS_OK;
    TAF_PS_SetAnswerModeInfoReq setAnsModeReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setAnsModeReq, sizeof(setAnsModeReq), 0x00, sizeof(TAF_PS_SetAnswerModeInfoReq));

    /* ????ID_MSG_TAF_PS_SET_ANSWER_MODE_INFO_REQ???? */
    setAnsModeReq.ctrl.moduleId = moduleId;
    setAnsModeReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setAnsModeReq.ctrl.opId     = opId;
    setAnsModeReq.ansMode       = ansMode;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_ANSWER_MODE_INFO_REQ,
                              &setAnsModeReq, sizeof(TAF_PS_SetAnswerModeInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetAnsModeInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32                  result = VOS_OK;
    TAF_PS_GetAnswerModeInfoReq getAnsModeReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getAnsModeReq, sizeof(getAnsModeReq), 0x00, sizeof(TAF_PS_GetAnswerModeInfoReq));

    /* ????ID_MSG_TAF_PS_SET_ANSWER_MODE_INFO_REQ???? */
    getAnsModeReq.ctrl.moduleId = moduleId;
    getAnsModeReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getAnsModeReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_ANSWER_MODE_INFO_REQ,
                              &getAnsModeReq, sizeof(TAF_PS_GetAnswerModeInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetDynamicPrimPdpContextInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                               VOS_UINT8 cid)
{
    VOS_UINT32                             result = VOS_OK;
    TAF_PS_GetDynamicPrimPdpContextInfoReq getDynamicPrimPdpCtxInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getDynamicPrimPdpCtxInfoReq, sizeof(getDynamicPrimPdpCtxInfoReq), 0x00,
                       sizeof(TAF_PS_GetDynamicPrimPdpContextInfoReq));

    /* ????ID_MSG_TAF_PS_GET_DYNAMIC_PRIM_PDP_CONTEXT_INFO_REQ???? */
    getDynamicPrimPdpCtxInfoReq.ctrl.moduleId = moduleId;
    getDynamicPrimPdpCtxInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getDynamicPrimPdpCtxInfoReq.ctrl.opId     = opId;
    getDynamicPrimPdpCtxInfoReq.cid           = cid;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId),
                              ID_MSG_TAF_PS_GET_DYNAMIC_PRIM_PDP_CONTEXT_INFO_REQ, &getDynamicPrimPdpCtxInfoReq,
                              sizeof(TAF_PS_GetDynamicPrimPdpContextInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetDynamicSecPdpContextInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 cid)
{
    VOS_UINT32                            result = VOS_OK;
    TAF_PS_GetDynamicSecPdpContextInfoReq getDynamicSecPdpCtxInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getDynamicSecPdpCtxInfoReq, sizeof(getDynamicSecPdpCtxInfoReq), 0x00,
                       sizeof(TAF_PS_GetDynamicSecPdpContextInfoReq));

    /* ????ID_MSG_TAF_PS_GET_DYNAMIC_SEC_PDP_CONTEXT_INFO_REQ???? */
    getDynamicSecPdpCtxInfoReq.ctrl.moduleId = moduleId;
    getDynamicSecPdpCtxInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getDynamicSecPdpCtxInfoReq.ctrl.opId     = opId;
    getDynamicSecPdpCtxInfoReq.cid           = cid;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId),
                              ID_MSG_TAF_PS_GET_DYNAMIC_SEC_PDP_CONTEXT_INFO_REQ, &getDynamicSecPdpCtxInfoReq,
                              sizeof(TAF_PS_GetDynamicSecPdpContextInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetDynamicTftInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 cid)
{
    VOS_UINT32                  result = VOS_OK;
    TAF_PS_GetDynamicTftInfoReq getDynamicTftInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getDynamicTftInfoReq, sizeof(getDynamicTftInfoReq), 0x00, sizeof(TAF_PS_GetDynamicTftInfoReq));

    /* ????ID_MSG_TAF_PS_GET_DYNAMIC_TFT_INFO_REQ???? */
    getDynamicTftInfoReq.ctrl.moduleId = moduleId;
    getDynamicTftInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getDynamicTftInfoReq.ctrl.opId     = opId;
    getDynamicTftInfoReq.cid           = cid;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_DYNAMIC_TFT_INFO_REQ,
                              &getDynamicTftInfoReq, sizeof(TAF_PS_GetDynamicTftInfoReq));

    return result;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 TAF_PS_Set5QosInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, TAF_5G_QosExt *pst5gQosInfo)
{
    VOS_UINT32             result = VOS_OK;
    TAF_PS_Set5GQosInfoReq set5QosInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&set5QosInfoReq, sizeof(set5QosInfoReq), 0x00, sizeof(TAF_PS_Set5GQosInfoReq));

    /* ????ID_MSG_TAF_PS_SET_5G_QOS_INFO_REQ???? */
    set5QosInfoReq.ctrl.moduleId = moduleId;
    set5QosInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    set5QosInfoReq.ctrl.opId     = opId;
    set5QosInfoReq.st5QosInfo    = *pst5gQosInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_5G_QOS_INFO_REQ,
                              &set5QosInfoReq, sizeof(TAF_PS_Set5GQosInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_Get5gQosInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32             result = VOS_OK;
    TAF_PS_Get5gQosInfoReq get5gQosInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&get5gQosInfoReq, sizeof(get5gQosInfoReq), 0x00, sizeof(TAF_PS_Get5gQosInfoReq));

    /* ????ID_MSG_TAF_PS_GET_SEC_PDP_CONTEXT_INFO_REQ???? */
    get5gQosInfoReq.ctrl.moduleId = moduleId;
    get5gQosInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    get5gQosInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_5G_QOS_INFO_REQ,
                              &get5gQosInfoReq, sizeof(TAF_PS_Get5gQosInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetDynamic5gQosInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 cid)
{
    VOS_UINT32                    result = VOS_OK;
    TAF_PS_GetDynamic5GQosInfoReq getDynamic5gQosInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getDynamic5gQosInfoReq, sizeof(getDynamic5gQosInfoReq), 0x00, sizeof(TAF_PS_GetDynamic5GQosInfoReq));

    /* ????ID_MSG_TAF_PS_GET_DYNAMIC_EPS_QOS_INFO_REQ???? */
    getDynamic5gQosInfoReq.ctrl.moduleId = moduleId;
    getDynamic5gQosInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getDynamic5gQosInfoReq.ctrl.opId     = opId;
    getDynamic5gQosInfoReq.cid           = cid;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_DYNAMIC_5G_QOS_INFO_REQ,
                              &getDynamic5gQosInfoReq, sizeof(TAF_PS_GetDynamic5GQosInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_SetUePolicyRpt(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 enable)
{
    VOS_UINT32               result;
    TAF_PS_SetUePolicyRptReq setUePolicyReq;

    (VOS_VOID)memset_s(&setUePolicyReq, sizeof(setUePolicyReq), 0x00, sizeof(TAF_PS_SetUePolicyRptReq));

    /* ?????? */
    result = VOS_OK;

    /* ????ID_MSG_TAF_PS_GET_UE_POLICY_REQ???? */
    setUePolicyReq.ctrl.moduleId = moduleId;
    setUePolicyReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setUePolicyReq.ctrl.opId     = opId;
    setUePolicyReq.enable        = enable;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_UE_POLICY_RPT_REQ,
                              &setUePolicyReq, sizeof(TAF_PS_SetUePolicyRptReq));

    return result;
}

VOS_UINT32 TAF_PS_GetUePolicyInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 indexNum)
{
    VOS_UINT32            result = VOS_OK;
    TAF_PS_GetUePolicyReq getUePolicyReq;

    /* ?????? */

    (VOS_VOID)memset_s(&getUePolicyReq, sizeof(getUePolicyReq), 0x00, sizeof(TAF_PS_GetUePolicyReq));

    /* ????ID_MSG_TAF_PS_GET_UE_POLICY_REQ???? */
    getUePolicyReq.ctrl.moduleId = moduleId;
    getUePolicyReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getUePolicyReq.ctrl.opId     = opId;
    getUePolicyReq.index         = indexNum;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_UE_POLICY_REQ,
                              &getUePolicyReq, sizeof(TAF_PS_GetUePolicyReq));

    return result;
}

VOS_UINT32 TAF_PS_SetCsUePolicy(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                TAF_PS_5GUePolicyInfo *pst5gUePolicy)
{
    VOS_UINT32            result = VOS_OK;
    TAF_PS_5GUePolicyRsp *uePolicyRsp = VOS_NULL_PTR;
    VOS_UINT32            allocLen;
    errno_t               memResult;

    /* ?????? */
    allocLen = sizeof(TAF_PS_5GUePolicyRsp) + pst5gUePolicy->infoLen;

    uePolicyRsp = (TAF_PS_5GUePolicyRsp *)PS_MEM_ALLOC(moduleId, allocLen);

    /* ???????????????? */
    if (uePolicyRsp == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(uePolicyRsp, allocLen, 0x00, allocLen);

    /* ????ID_MSG_TAF_PS_5G_UE_POLICY_INFO_RSP???? */
    uePolicyRsp->ctrl.moduleId = moduleId;
    uePolicyRsp->ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    uePolicyRsp->ctrl.opId     = opId;

    /* ????protocalver & pti & MsgType */
    uePolicyRsp->uePolicyInfo.protocalVer     = pst5gUePolicy->protocalVer;
    uePolicyRsp->uePolicyInfo.pti             = pst5gUePolicy->pti;
    uePolicyRsp->uePolicyInfo.uePolicyMsgType = pst5gUePolicy->uePolicyMsgType;

    /* ????class mark info */
    uePolicyRsp->uePolicyInfo.classMarkLen = pst5gUePolicy->classMarkLen;

    memResult = memcpy_s(uePolicyRsp->uePolicyInfo.classMark, sizeof(uePolicyRsp->uePolicyInfo.classMark),
                         pst5gUePolicy->classMark, sizeof(pst5gUePolicy->classMark));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(uePolicyRsp->uePolicyInfo.classMark), sizeof(pst5gUePolicy->classMark));

    uePolicyRsp->uePolicyInfo.ueOsIdLen = pst5gUePolicy->ueOsIdLen;

    memResult = memcpy_s(uePolicyRsp->uePolicyInfo.ueOsIdInfo, sizeof(uePolicyRsp->uePolicyInfo.ueOsIdInfo),
                         pst5gUePolicy->ueOsIdInfo, sizeof(pst5gUePolicy->ueOsIdInfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(uePolicyRsp->uePolicyInfo.ueOsIdInfo), sizeof(pst5gUePolicy->ueOsIdInfo));

    if (pst5gUePolicy->infoLen != 0) {
        uePolicyRsp->uePolicyInfo.infoLen = pst5gUePolicy->infoLen;

        memResult = memcpy_s(uePolicyRsp->uePolicyInfo.content, pst5gUePolicy->infoLen, pst5gUePolicy->content,
                             pst5gUePolicy->infoLen);
        TAF_MEM_CHK_RTN_VAL(memResult, pst5gUePolicy->infoLen, pst5gUePolicy->infoLen);
    }

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_5G_UE_POLICY_INFO_RSP,
                              uePolicyRsp, (sizeof(TAF_PS_5GUePolicyRsp) + uePolicyRsp->uePolicyInfo.infoLen));

    PS_MEM_FREE(moduleId, uePolicyRsp);

    return result;
}

VOS_UINT32 TAF_PS_Get5gUrspRsd(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, PS_URSP_Td *qryPara)
{
    VOS_UINT32 result;

    TAF_PS_UrspRsdQryReq urspRsdQryReq;
    (VOS_VOID)memset_s(&urspRsdQryReq, sizeof(urspRsdQryReq), 0, sizeof(urspRsdQryReq));

    /* ????ID_MSG_TAF_PS_GET_URSP_RSD_INFO_REQ???? */
    urspRsdQryReq.ctrl.moduleId = moduleId;
    urspRsdQryReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    urspRsdQryReq.ctrl.opId = opId;
    urspRsdQryReq.tdInfo = *qryPara;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId),
        ID_MSG_TAF_PS_GET_URSP_RSD_INFO_REQ, &urspRsdQryReq, sizeof(TAF_PS_UrspRsdQryReq));

    return result;
}

VOS_UINT32 TAF_PS_SetCgContEx(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
    TAf_PS_CgContExInfo *cgContexInfo)
{
    TAF_PS_SetCgContExReq setCgContExReq;
    (VOS_VOID)memset_s(&setCgContExReq, sizeof(setCgContExReq), 0x00, sizeof(setCgContExReq));

    setCgContExReq.ctrl.moduleId = moduleId;
    setCgContExReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setCgContExReq.ctrl.opId     = opId;
    setCgContExReq.cgContexInfo  = *cgContexInfo;

    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_CGCONTEX_REQ,
        &setCgContExReq, sizeof(TAF_PS_SetCgContExReq));
}
#endif

VOS_UINT32 TAF_PS_SetEpsQosInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, TAF_EPS_QosExt *epsQosInfo)
{
    VOS_UINT32              result = VOS_OK;
    TAF_PS_SetEpsQosInfoReq setEpsQosInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setEpsQosInfoReq, sizeof(setEpsQosInfoReq), 0x00, sizeof(TAF_PS_SetEpsQosInfoReq));

    /* ????ID_MSG_TAF_PS_SET_EPS_QOS_INFO_REQ???? */
    setEpsQosInfoReq.ctrl.moduleId = moduleId;
    setEpsQosInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setEpsQosInfoReq.ctrl.opId     = opId;
    setEpsQosInfoReq.epsQosInfo    = *epsQosInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_EPS_QOS_INFO_REQ,
                              &setEpsQosInfoReq, sizeof(TAF_PS_SetEpsQosInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetEpsQosInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32              result = VOS_OK;
    TAF_PS_GetEpsQosInfoReq getEpsQosInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getEpsQosInfoReq, sizeof(getEpsQosInfoReq), 0x00, sizeof(TAF_PS_GetEpsQosInfoReq));

    /* ????ID_MSG_TAF_PS_GET_EPS_QOS_INFO_REQ???? */
    getEpsQosInfoReq.ctrl.moduleId = moduleId;
    getEpsQosInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getEpsQosInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_EPS_QOS_INFO_REQ,
                              &getEpsQosInfoReq, sizeof(TAF_PS_GetEpsQosInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetDynamicEpsQosInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 cid)
{
    VOS_UINT32                     result = VOS_OK;
    TAF_PS_GetDynamicEpsQosInfoReq getDynamicEpsQosInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getDynamicEpsQosInfoReq, sizeof(getDynamicEpsQosInfoReq), 0x00, sizeof(TAF_PS_GetDynamicEpsQosInfoReq));

    /* ????ID_MSG_TAF_PS_GET_DYNAMIC_EPS_QOS_INFO_REQ???? */
    getDynamicEpsQosInfoReq.ctrl.moduleId = moduleId;
    getDynamicEpsQosInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getDynamicEpsQosInfoReq.ctrl.opId     = opId;
    getDynamicEpsQosInfoReq.cid           = cid;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId),
                              ID_MSG_TAF_PS_GET_DYNAMIC_EPS_QOS_INFO_REQ, &getDynamicEpsQosInfoReq,
                              sizeof(TAF_PS_GetDynamicEpsQosInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_SetPdpDnsInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, TAF_PDP_DnsExt *pdpDnsInfo)
{
    VOS_UINT32              result = VOS_OK;
    TAF_PS_SetPdpDnsInfoReq setPdpDnsInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setPdpDnsInfoReq, sizeof(setPdpDnsInfoReq), 0x00, sizeof(TAF_PS_SetPdpDnsInfoReq));

    /* ????ID_MSG_TAF_PS_SET_PDP_DNS_INFO_REQ???? */
    setPdpDnsInfoReq.ctrl.moduleId = moduleId;
    setPdpDnsInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setPdpDnsInfoReq.ctrl.opId     = opId;
    setPdpDnsInfoReq.pdpDnsInfo    = *pdpDnsInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_PDP_DNS_INFO_REQ,
                              &setPdpDnsInfoReq, sizeof(TAF_PS_SetPdpDnsInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetPdpDnsInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32              result = VOS_OK;
    TAF_PS_GetPdpDnsInfoReq getPdpDnsInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getPdpDnsInfoReq, sizeof(getPdpDnsInfoReq), 0x00, sizeof(TAF_PS_GetPdpDnsInfoReq));

    /* ????ID_MSG_TAF_PS_GET_PDP_DNS_INFO_REQ???? */
    getPdpDnsInfoReq.ctrl.moduleId = moduleId;
    getPdpDnsInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getPdpDnsInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_PDP_DNS_INFO_REQ,
                              &getPdpDnsInfoReq, sizeof(TAF_PS_GetPdpDnsInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_ConfigNbnsFunction(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT32 enabled)
{
    VOS_UINT32                   result = VOS_OK;
    TAF_PS_ConfigNbnsFunctionReq configNbnsFunReq;

    /* ?????? */
    (VOS_VOID)memset_s(&configNbnsFunReq, sizeof(configNbnsFunReq), 0x00, sizeof(TAF_PS_ConfigNbnsFunctionReq));

    /* ????ID_MSG_TAF_PS_CONFIG_NBNS_FUNCTION_REQ???? */
    configNbnsFunReq.ctrl.moduleId = moduleId;
    configNbnsFunReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    configNbnsFunReq.ctrl.opId     = opId;
    configNbnsFunReq.enabled       = enabled;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_CONFIG_NBNS_FUNCTION_REQ,
                              &configNbnsFunReq, sizeof(TAF_PS_ConfigNbnsFunctionReq));

    return result;
}

VOS_UINT32 TAF_PS_SetAuthDataInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                  TAF_AUTHDATA_Ext *authDataInfo)
{
    VOS_UINT32                result = VOS_OK;
    TAF_PS_SetAuthdataInfoReq setAuthDataInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setAuthDataInfoReq, sizeof(setAuthDataInfoReq), 0x00, sizeof(TAF_PS_SetAuthdataInfoReq));

    /* ????ID_MSG_TAF_PS_SET_AUTHDATA_INFO_REQ???? */
    setAuthDataInfoReq.ctrl.moduleId = moduleId;
    setAuthDataInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setAuthDataInfoReq.ctrl.opId     = opId;
    setAuthDataInfoReq.authDataInfo  = *authDataInfo;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_AUTHDATA_INFO_REQ,
                              &setAuthDataInfoReq, sizeof(TAF_PS_SetAuthdataInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetAuthDataInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    VOS_UINT32                result = VOS_OK;
    TAF_PS_GetAuthdataInfoReq getAuthDataInfoReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getAuthDataInfoReq, sizeof(getAuthDataInfoReq), 0x00, sizeof(TAF_PS_GetAuthdataInfoReq));

    /* ????ID_MSG_TAF_PS_GET_AUTHDATA_INFO_REQ???? */
    getAuthDataInfoReq.ctrl.moduleId = moduleId;
    getAuthDataInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getAuthDataInfoReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_AUTHDATA_INFO_REQ,
                              &getAuthDataInfoReq, sizeof(TAF_PS_GetAuthdataInfoReq));

    return result;
}

VOS_UINT32 TAF_PS_GetGprsActiveType(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, TAF_ATD_Para *atdPara)
{
    VOS_UINT32                   result = VOS_OK;
    TAF_PS_GetDGprsActiveTypeReq getGprsActiveTypeReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getGprsActiveTypeReq, sizeof(getGprsActiveTypeReq), 0x00, sizeof(TAF_PS_GetDGprsActiveTypeReq));

    /* ????ID_MSG_TAF_PS_GET_AUTHDATA_INFO_REQ???? */
    getGprsActiveTypeReq.ctrl.moduleId = moduleId;
    getGprsActiveTypeReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getGprsActiveTypeReq.ctrl.opId     = opId;
    getGprsActiveTypeReq.atdPara       = *atdPara;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_D_GPRS_ACTIVE_TYPE_REQ,
                              &getGprsActiveTypeReq, sizeof(TAF_PS_GetDGprsActiveTypeReq));

    return result;
}

VOS_UINT32 TAF_PS_PppDialOrig(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 cid,
                              TAF_PPP_ReqConfigInfo *pppReqConfigInfo)
{
    errno_t               memResult;
    VOS_UINT32            result = VOS_OK;
    TAF_PS_PppDialOrigReq pppDialOrigReq;

    /* ?????? */
    (VOS_VOID)memset_s(&pppDialOrigReq, sizeof(pppDialOrigReq), 0x00, sizeof(TAF_PS_PppDialOrigReq));

    /* ????ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ???? */
    pppDialOrigReq.ctrl.moduleId = moduleId;
    pppDialOrigReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    pppDialOrigReq.ctrl.opId     = opId;

    pppDialOrigReq.pppDialParaInfo.cid = cid;

    pppDialOrigReq.pppDialParaInfo.opPppConfigInfo = VOS_TRUE;
    memResult = memcpy_s(&(pppDialOrigReq.pppDialParaInfo.pppReqConfigInfo),
                         sizeof(pppDialOrigReq.pppDialParaInfo.pppReqConfigInfo), pppReqConfigInfo,
                         sizeof(TAF_PPP_ReqConfigInfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pppDialOrigReq.pppDialParaInfo.pppReqConfigInfo),
                        sizeof(TAF_PPP_ReqConfigInfo));

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ,
                              &pppDialOrigReq, sizeof(TAF_PS_PppDialOrigReq));

    return result;
}

VOS_UINT32 TAF_PS_GetDynamicDnsInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 cid)
{
    VOS_UINT32 result;

    TAF_PS_GetNegotiationDnsReq negoDns;

    /* ?????? */
    (VOS_VOID)memset_s(&negoDns, sizeof(negoDns), 0x00, sizeof(TAF_PS_GetNegotiationDnsReq));

    /* ???????????? */
    negoDns.ctrl.moduleId = moduleId;
    negoDns.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    negoDns.ctrl.opId     = opId;
    negoDns.cid           = cid;

    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_NEGOTIATION_DNS_REQ,
                              &negoDns, sizeof(TAF_PS_GetNegotiationDnsReq));

    return result;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 TAF_PS_SetCtaInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT8 timeLen)
{
    TAF_PS_SetCtaInfoReq setPktCdataInactivityTmrlenReq;
    VOS_UINT32           result;

    (VOS_VOID)memset_s(&setPktCdataInactivityTmrlenReq, sizeof(TAF_PS_SetCtaInfoReq), 0x00, sizeof(TAF_PS_SetCtaInfoReq));

    /* ????ID_MSG_TAF_PS_GET_DSFLOW_WRITE_NV_CFG_REQ???? */
    setPktCdataInactivityTmrlenReq.ctrl.moduleId = moduleId;
    setPktCdataInactivityTmrlenReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setPktCdataInactivityTmrlenReq.ctrl.opId     = opId;

    setPktCdataInactivityTmrlenReq.pktCdataInactivityTmrLen = timeLen;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_CTA_INFO_REQ,
                              &setPktCdataInactivityTmrlenReq, sizeof(setPktCdataInactivityTmrlenReq));

    return result;
}

VOS_UINT32 TAF_PS_GetCtaInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    TAF_PS_GetCtaInfoReq getPktCdataInactivityTmrlenReq;
    VOS_UINT32           result;

    (VOS_VOID)memset_s(&getPktCdataInactivityTmrlenReq, sizeof(TAF_PS_GetCtaInfoReq), 0x00, sizeof(TAF_PS_GetCtaInfoReq));

    getPktCdataInactivityTmrlenReq.ctrl.moduleId = moduleId;
    getPktCdataInactivityTmrlenReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getPktCdataInactivityTmrlenReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_CTA_INFO_REQ,
                              &getPktCdataInactivityTmrlenReq, sizeof(getPktCdataInactivityTmrlenReq));

    return result;
}

#endif

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 TAF_PS_SetImsPdpCfg(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, TAF_IMS_PdpCfg *imsPdpCfg)
{
    VOS_UINT32             result = VOS_OK;
    TAF_PS_SetImsPdpCfgReq setImsPdpCfgReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setImsPdpCfgReq, sizeof(TAF_PS_SetImsPdpCfgReq), 0x00, sizeof(TAF_PS_SetImsPdpCfgReq));

    /* ????ID_MSG_TAF_PS_SET_IMS_PDP_CFG_REQ???? */
    setImsPdpCfgReq.ctrl.moduleId = moduleId;
    setImsPdpCfgReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setImsPdpCfgReq.ctrl.opId     = opId;
    setImsPdpCfgReq.imsPdpCfg     = *imsPdpCfg;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_IMS_PDP_CFG_REQ,
                              &setImsPdpCfgReq, sizeof(TAF_PS_SetImsPdpCfgReq));

    return result;
}
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 TAF_PS_SetCdmaDormantTimer(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                      VOS_UINT8 dormantTimer)
{
    VOS_UINT32               result = VOS_OK;
    TAF_PS_Set1XDormTimerReq st1xDormTimerReq;

    /* ?????? */
    (VOS_VOID)memset_s(&st1xDormTimerReq, sizeof(TAF_PS_Set1XDormTimerReq), 0x00, sizeof(TAF_PS_Set1XDormTimerReq));

    /* ????ID_MSG_TAF_PS_SET_1X_DORMANT_TIMER_REQ???? */
    st1xDormTimerReq.ctrl.moduleId = moduleId;
    st1xDormTimerReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    st1xDormTimerReq.ctrl.opId     = opId;
    st1xDormTimerReq.dormantTimer  = dormantTimer;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_1X_DORM_TIMER_REQ,
                              &st1xDormTimerReq, sizeof(TAF_PS_Set1XDormTimerReq));

    return result;
}

VOS_UINT32 TAF_PS_ProcCdmaDormTimerQryReq(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    TAF_PS_Get1XDormTimerReq get1xDormTimerReq;
    VOS_UINT32               result;

    (VOS_VOID)memset_s(&get1xDormTimerReq, sizeof(TAF_PS_Get1XDormTimerReq), 0x00, sizeof(TAF_PS_Get1XDormTimerReq));

    get1xDormTimerReq.ctrl.moduleId = moduleId;
    get1xDormTimerReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    get1xDormTimerReq.ctrl.opId     = opId;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_1X_DORM_TIEMR_REQ,
                              &get1xDormTimerReq, sizeof(get1xDormTimerReq));

    return result;
}

#endif

#if (OSA_CPU_CCPU == VOS_OSA_CPU)

VOS_UINT32 TAF_PS_GetModuleIdByCid(VOS_UINT8 cid, ModemIdUint16 modemId)
{
    VOS_UINT8 bearerInst = 0;
    VOS_UINT8 pdnInst    = 0;

    if (TAF_DSM_GetPdnInstAndBearerInstByCid(cid, VOS_FALSE, &pdnInst, &bearerInst) == VOS_FALSE) {
        return 0;
    }

    if (TAF_DSM_GetDsmCtxAddrByModemId(modemId)->dsmConnCtx.pdnEntityCtx[pdnInst]->pdnClientType.bitImsaClient ==
        VOS_TRUE) {
        return PS_PID_IMSA;
    }

    if (TAF_DSM_GetDsmCtxAddrByModemId(modemId)->dsmConnCtx.pdnEntityCtx[pdnInst]->pdnClientType.bitAppClient ==
        VOS_TRUE) {
        return WUEPS_PID_AT;
    }

    return 0;
}

#endif

MODULE_EXPORTED VOS_UINT32 TAF_PS_GetCidSdfParaInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                                    TAF_SDF_ParaQueryInfo *sdfQueryInfo)
{
#if (OSA_CPU_CCPU == VOS_OSA_CPU)
    VOS_UINT8  num;
    VOS_UINT16 modemId;
#endif
    VOS_UINT32        result;
    TAF_PS_SdfInfoReq sdfInfoReq;

    result = VOS_ERR;
    (VOS_VOID)memset_s(&sdfInfoReq, sizeof(sdfInfoReq), 0x00, sizeof(TAF_PS_SdfInfoReq));

#if (OSA_CPU_CCPU == VOS_OSA_CPU)
    modemId = TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId);
#endif

    if (sdfQueryInfo == VOS_NULL_PTR) {
        /* ???????????????????? */
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(sdfQueryInfo, sizeof(TAF_SDF_ParaQueryInfo), 0x00, sizeof(TAF_SDF_ParaQueryInfo));

/* ??????????????????C?? */
#if (OSA_CPU_CCPU == VOS_OSA_CPU)
    for (num = 0; num <= TAF_MAX_CID_NV; num++) {
        if (TAF_DSM_GetSdfParaInfoByModemId(modemId, num, &(sdfQueryInfo->sdfPara[sdfQueryInfo->sdfNum])) == VOS_OK) {
            sdfQueryInfo->sdfNum++;
        }
    }
    result = VOS_OK;

    /* ???????????? */
    TAF_DSM_TraceSyncMsgByModemId(modemId, ID_MSG_TAF_PS_GET_CID_SDF_REQ, (VOS_UINT8 *)sdfQueryInfo,
                                  sizeof(TAF_SDF_ParaQueryInfo));
#endif

    return result;
}

MODULE_EXPORTED VOS_UINT32 TAF_PS_GetUnusedCid(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                               VOS_UINT8 *puCid)
{
    VOS_UINT32    result;
    ModemIdUint16 modemId;

    modemId = TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId);

    if (modemId >= MODEM_ID_BUTT) {
        PS_PRINTF_WARNING("<TAF_PS_GetUnusedCid> ModemId is error!");

        return VOS_ERR;
    }

    result = VOS_OK;

    if (puCid == VOS_NULL_PTR) {
        /* ???????????????????? */
        return VOS_ERR;
    }

/* ??????????????????C?? */
#if (OSA_CPU_CCPU == VOS_OSA_CPU)
    /* ????????????????CID */
    *puCid = TAF_DSM_FindCidForDialByModemId(modemId, moduleId);

    if (*puCid == TAF_INVALID_CID) {
        result = VOS_ERR;
    }

    /* ???????????? */
    TAF_DSM_TraceSyncMsgByModemId(modemId, ID_MSG_TAF_PS_GET_UNUSED_CID_REQ, puCid, sizeof(VOS_UINT8));
#endif

    return result;
}

VOS_UINT32 TAF_PS_GetLteAttchInfo(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    TAF_PS_GetLteAttachInfoReq getLteAttchInfoReq;

    /* ?????? */
   (VOS_VOID)memset_s(&getLteAttchInfoReq, sizeof(getLteAttchInfoReq), 0x00, sizeof(TAF_PS_GetLteAttachInfoReq));

    /* ????ID_MSG_TAF_PS_GET_LTE_ATTACH_ESM_INFO_REQ???? */
    getLteAttchInfoReq.ctrl.moduleId = moduleId;
    getLteAttchInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getLteAttchInfoReq.ctrl.opId     = opId;

    /* ???????? */
    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_LTE_ATTACH_INFO_REQ,
                            &getLteAttchInfoReq, sizeof(TAF_PS_GetLteAttachInfoReq));
}

VOS_UINT32 TAF_PS_EpdgCtrlMsg(const TAF_Ctrl *ctrl, const TAF_PS_EpdgCtrl *epdgCtrl)
{
    errno_t            memResult;
    VOS_UINT32         result = VOS_OK;
    TAF_PS_EpdgCtrlNtf epdgCtrlNtf;

    /* ?????? */
    (VOS_VOID)memset_s(&epdgCtrlNtf, sizeof(epdgCtrlNtf), 0x00, sizeof(TAF_PS_EpdgCtrlNtf));

    /* ????ID_MSG_TAF_IFACE_UP_REQ???? */
    epdgCtrlNtf.ctrl.moduleId = ctrl->moduleId;
    epdgCtrlNtf.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(ctrl->clientId);
    epdgCtrlNtf.ctrl.opId     = ctrl->opId;

    memResult = memcpy_s(&epdgCtrlNtf.epdgCtrl, sizeof(epdgCtrlNtf.epdgCtrl), epdgCtrl, sizeof(TAF_PS_EpdgCtrl));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(epdgCtrlNtf.epdgCtrl), sizeof(TAF_PS_EpdgCtrl));

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(ctrl->clientId), ID_MSG_TAF_PS_EPDG_CTRL_NTF,
                              &epdgCtrlNtf, sizeof(TAF_PS_EpdgCtrlNtf));

    return result;
}

VOS_UINT32 TAF_PS_SetRoamPdpType(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
                                 const TAF_PS_RoamingPdpTypeInfo *pdpTypeInfo)
{
    TAF_PS_SetRoamingPdpTypeReq setPdpType;
    VOS_UINT32                  result;
    errno_t                     memResult;

    (VOS_VOID)memset_s(&setPdpType, sizeof(setPdpType), 0x00, sizeof(TAF_PS_SetRoamingPdpTypeReq));

    /* ???????????? */
    setPdpType.ctrl.moduleId = moduleId;
    setPdpType.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setPdpType.ctrl.opId     = opId;

    memResult = memcpy_s(&(setPdpType.pdpTypeInfo), sizeof(TAF_PS_RoamingPdpTypeInfo), pdpTypeInfo,
                         sizeof(TAF_PS_RoamingPdpTypeInfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TAF_PS_RoamingPdpTypeInfo), sizeof(TAF_PS_RoamingPdpTypeInfo));

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_ROAMING_PDP_TYPE_REQ,
                              &setPdpType, sizeof(TAF_PS_SetRoamingPdpTypeReq));

    return result;
}

VOS_UINT32 TAF_PS_GetSinglePdnSwitch(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    TAF_PS_GetSinglePdnSwitchReq getSinglePdnSwitchReq;

    /* ?????? */
    (VOS_VOID)memset_s(&getSinglePdnSwitchReq, sizeof(getSinglePdnSwitchReq),
        0x00, sizeof(TAF_PS_GetSinglePdnSwitchReq));

    /* ????ID_MSG_TAF_PS_GET_SINGLE_PDN_SWITCH_REQ???? */
    getSinglePdnSwitchReq.ctrl.moduleId = moduleId;
    getSinglePdnSwitchReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    getSinglePdnSwitchReq.ctrl.opId     = opId;

    /* ???????? */
    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_GET_SINGLE_PDN_SWITCH_REQ,
                            &getSinglePdnSwitchReq, sizeof(TAF_PS_GetSinglePdnSwitchReq));
}

VOS_UINT32 TAF_PS_SetSinglePdnSwitch(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId, VOS_UINT32 enable)
{
    TAF_PS_SetSinglePdnSwitchReq setSinglePdnSwitchReq;

    /* ?????? */
    (VOS_VOID)memset_s(&setSinglePdnSwitchReq, sizeof(setSinglePdnSwitchReq),
        0x00, sizeof(TAF_PS_SetSinglePdnSwitchReq));

    /* ????ID_MSG_TAF_PS_SET_SINGLE_PDN_SWITCH_REQ???? */
    setSinglePdnSwitchReq.ctrl.moduleId = moduleId;
    setSinglePdnSwitchReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setSinglePdnSwitchReq.ctrl.opId     = opId;

    setSinglePdnSwitchReq.enable = enable;

    /* ???????? */
    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_SINGLE_PDN_SWITCH_REQ,
                            &setSinglePdnSwitchReq, sizeof(TAF_PS_SetSinglePdnSwitchReq));
}

#if (FEATURE_MBB_CUST == FEATURE_ON)
VOS_UINT32 TAF_PS_SendDsambrQryReq(VOS_UINT32 moduleId, VOS_UINT16 exclientId, VOS_UINT8 opId, VOS_UINT8 cid)
{
    TAF_PS_DsambrQryRequest getDsambrInfoReq;
    VOS_UINT32 result;

    /* ?????? */
    (VOS_VOID)memset_s(&getDsambrInfoReq, sizeof(getDsambrInfoReq), 0x00, sizeof(getDsambrInfoReq));

    /* ????ID_MSG_TAF_PS_GET_DSAMBR_INFO_REQ???? */
    getDsambrInfoReq.ctrlInfo.moduleId = moduleId;
    getDsambrInfoReq.ctrlInfo.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exclientId);
    getDsambrInfoReq.ctrlInfo.opId = opId;
    getDsambrInfoReq.cid = cid;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exclientId), ID_MSG_TAF_PS_GET_DSAMBR_INFO_REQ,
        &getDsambrInfoReq, sizeof(getDsambrInfoReq));

    return result;
}

/*
 * ????????: ????APN LIST????????????????????????APN????
 * ????????: moduleId: AT Module ID
 *           clientId: AT Client ID
 *           opId: ????ID
 *           custAttachApn: ??????????APN????
 * ????????: VOS_OK: ????????
 *           VOS_ERR: ????????
 */
VOS_UINT32 TAF_PS_SetCustomAttachProfile(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
    TAF_PS_CustomerAttachApnInfo *custAttachApn)
{
    TAF_PS_SetCustAttachProfileInfoReq customAttachApnInfoReq;

    (VOS_VOID)memset_s(&customAttachApnInfoReq, sizeof(customAttachApnInfoReq), 0x00, sizeof(customAttachApnInfoReq));

    /* ????ID_MSG_TAF_PS_SET_CUST_ATTACH_PROFILE_REQ???? */
    customAttachApnInfoReq.ctrl.moduleId = moduleId;
    customAttachApnInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(clientId);
    customAttachApnInfoReq.ctrl.opId = opId;

    (VOS_VOID)memcpy_s(&(customAttachApnInfoReq.custAttachApn), sizeof(customAttachApnInfoReq.custAttachApn),
        custAttachApn, sizeof(customAttachApnInfoReq.custAttachApn));

    /* ???????? */
    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(clientId), ID_MSG_TAF_PS_SET_CUST_ATTACH_PROFILE_REQ,
        &customAttachApnInfoReq, sizeof(TAF_PS_SetCustAttachProfileInfoReq));
}

/*
 * ????????: ????APN LIST????????????????????????APN????
 * ????????: moduleId: AT Module ID
 *           clientId: AT Client ID
 *           opId: ????ID
 *           customGetInfo: ??????????APN????
 * ????????: VOS_OK: ????????
 *           VOS_ERR: ????????
 */
VOS_UINT32 TAF_PS_GetCustomAttachProfile(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
    TAF_PS_CustomerAttachApnInfoExt *customGetInfo)
{
    TAF_PS_GetCustAttachProfileInfoReq  customAttachApnInfoReq;

    (VOS_VOID)memset_s(&customAttachApnInfoReq, sizeof(customAttachApnInfoReq), 0x00, sizeof(customAttachApnInfoReq));

    /* ????ID_MSG_TAF_PS_GET_CUST_ATTACH_PROFILE_REQ???? */
    customAttachApnInfoReq.ctrl.moduleId = moduleId;
    customAttachApnInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(clientId);
    customAttachApnInfoReq.ctrl.opId = opId;

    (VOS_VOID)memcpy_s(&(customAttachApnInfoReq.exInfo), sizeof(customAttachApnInfoReq.exInfo),
        customGetInfo, sizeof(customAttachApnInfoReq.exInfo));

    /* ???????? */
    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(clientId), ID_MSG_TAF_PS_GET_CUST_ATTACH_PROFILE_REQ,
        &customAttachApnInfoReq, sizeof(TAF_PS_GetCustAttachProfileInfoReq));
}

/*
 * ????????: ????APN LIST??????????????????????????Profile
 * ????????: moduleId: AT Module ID
 *           clientId: AT Client ID
 *           opId: ????ID
 * ????????: VOS_OK: ????????
 *           VOS_ERR: ????????
 */
VOS_UINT32 TAF_PS_GetCustomAttachProfileCount(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId)
{
    TAF_PS_GetCustAttachProfileCountReq customAttachApnInfoReq;

    (VOS_VOID)memset_s(&customAttachApnInfoReq, sizeof(customAttachApnInfoReq), 0x00, sizeof(customAttachApnInfoReq));

    /* ????ID_MSG_TAF_PS_GET_CUSTOM_ATTACH_APN_REQ???? */
    customAttachApnInfoReq.ctrl.moduleId = moduleId;
    customAttachApnInfoReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(clientId);
    customAttachApnInfoReq.ctrl.opId = opId;

    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(clientId),
        ID_MSG_TAF_PS_GET_CUST_ATTACH_PROFILE_COUNT_REQ, &customAttachApnInfoReq,
        sizeof(TAF_PS_GetCustAttachProfileCountReq));
}

/*
 * ????????: LTEAPNATTACH????????????????????????????TAF????
 * ????????: moduleId: AT Module ID
 *           clientId: AT Client ID
 *           opId: ????ID
 *           switchFlag: Profile????????
 * ????????: AT_OK: ????????
 *           AT_ERROR: ????????
 */
VOS_UINT32 TAF_PS_SetAttachProfileSwitchStatus(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
    VOS_UINT32 switchFlag)
{
    TAF_PS_SetAttachProfileSwitchStatusReq apnAttachSwitchReq;

    (VOS_VOID)memset_s(&apnAttachSwitchReq, sizeof(apnAttachSwitchReq), 0x00, sizeof(apnAttachSwitchReq));

    /* ????ID_MSG_TAF_PS_SET_APN_ATTACH_SWITCH_REQ???? */
    apnAttachSwitchReq.ctrl.moduleId = moduleId;
    apnAttachSwitchReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(clientId);
    apnAttachSwitchReq.ctrl.opId = opId;
    apnAttachSwitchReq.switchFlag = switchFlag;

    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(clientId),
        ID_MSG_TAF_PS_SET_ATTACH_PROFILE_SWITCH_STATUS_REQ, &apnAttachSwitchReq,
        sizeof(TAF_PS_SetAttachProfileSwitchStatusReq));
}

/*
 * ????????: LTEAPNATTACH????????????????????????????TAF????
 * ????????: moduleId: AT Module ID
 *           clientId: AT Client ID
 *           opId: ????ID
 * ????????: AT_OK: ????????
 *           AT_ERROR: ????????
 */
VOS_UINT32 TAF_PS_GetAttachProfileSwitchStatus(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId)
{
    TAF_PS_GetAttachProfileSwitchStatusReq apnAttachStatusReq;

    (VOS_VOID)memset_s(&apnAttachStatusReq, sizeof(apnAttachStatusReq), 0x00, sizeof(apnAttachStatusReq));

    /* ????ID_MSG_TAF_PS_GET_APN_ATTACH_STATE_REQ???? */
    apnAttachStatusReq.ctrl.moduleId = moduleId;
    apnAttachStatusReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(clientId);
    apnAttachStatusReq.ctrl.opId = opId;

    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(clientId),
        ID_MSG_TAF_PS_GET_ATTACH_PROFILE_SWITCH_STATUS_REQ, &apnAttachStatusReq,
        sizeof(TAF_PS_GetAttachProfileSwitchStatusReq));
}
#endif

VOS_UINT32 TAF_PS_SetPsCallRptCmd(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
    const TAF_PS_CallRptCmdCfgInfo *cmdCfg)
{
    TAF_PS_SetPsCallRptCmdReq setCmdCfgReq;
    VOS_UINT32 result;

    (VOS_VOID)memset_s(&setCmdCfgReq, sizeof(setCmdCfgReq), 0x00, sizeof(setCmdCfgReq));

    /* ???????????? */
    setCmdCfgReq.ctrl.moduleId = moduleId;
    setCmdCfgReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    setCmdCfgReq.ctrl.opId = opId;
    setCmdCfgReq.cfgInfo = *cmdCfg;

    /* ???????? */
    result = TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_SET_PS_CALL_RPT_CMD_REQ,
                              &setCmdCfgReq, sizeof(setCmdCfgReq));

    return result;
}

MODULE_EXPORTED VOS_UINT32 TAF_PS_TrigSrvAcq(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId,
    TAF_PS_SrvAcqInfo *srvAcqInfo)
{
    TAF_PS_TrigSrvAcqReq trigSrvAcqReq;
    (VOS_VOID)memset_s(&trigSrvAcqReq, sizeof(trigSrvAcqReq), 0x00, sizeof(trigSrvAcqReq));

    /* ???????????? */
    trigSrvAcqReq.ctrl.moduleId = moduleId;
    trigSrvAcqReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    trigSrvAcqReq.ctrl.opId = opId;
    if (srvAcqInfo != VOS_NULL_PTR) {
        trigSrvAcqReq.srvAcqInfo = *srvAcqInfo;
    }

    /* ???????? */
    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_TRIG_SRV_ACQ_REQ,
                            &trigSrvAcqReq, sizeof(trigSrvAcqReq));

}

MODULE_EXPORTED VOS_UINT32 TAF_PS_AbortSrvAcq(VOS_UINT32 moduleId, VOS_UINT16 exClientId, VOS_UINT8 opId)
{
    TAF_PS_AbortSrvAcqReq abortSrvAcqReq;
    (VOS_VOID)memset_s(&abortSrvAcqReq, sizeof(abortSrvAcqReq), 0x00, sizeof(abortSrvAcqReq));

    /* ???????????? */
    abortSrvAcqReq.ctrl.moduleId = moduleId;
    abortSrvAcqReq.ctrl.clientId = TAF_PS_GET_CLIENTID_FROM_EXCLIENTID(exClientId);
    abortSrvAcqReq.ctrl.opId = opId;

    /* ???????? */
    return TAF_PS_SndDsmMsg(TAF_PS_GET_MODEMID_FROM_EXCLIENTID(exClientId), ID_MSG_TAF_PS_ABORT_SRV_ACQ_REQ,
                            &abortSrvAcqReq, sizeof(abortSrvAcqReq));

}

