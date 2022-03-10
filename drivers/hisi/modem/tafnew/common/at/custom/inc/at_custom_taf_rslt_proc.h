/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
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
#ifndef _AT_CUSTOM_TAF_RSLT_PROC_H_
#define _AT_CUSTOM_TAF_RSLT_PROC_H_

#include "vos.h"
#include "ps_type_def.h"
#include "taf_api.h"
#include "mn_call_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)
VOS_UINT32 AT_RcvMmaExchangeModemInfoCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaQryXcposrRptCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaQryXcposrCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaMipiInfoCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaQryBestFreqCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
extern VOS_UINT32 AT_RcvMtaQryCrrconnCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvTafPsEvtConfigVTFlowRptCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtSetDataSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtGetDataSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtSetDataRoamSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtGetDataRoamSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
#if (FEATURE_LTE == FEATURE_ON)
/*
 * 功能描述: ID_MSG_TAF_PS_GET_LTE_ATTACH_INFO_CNF事件处理函数
 */
VOS_UINT32 AT_RcvTafGetLteAttachInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_RcvMtaLendcQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvTafPsEvtSetUePolicyRptCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_ChkTafPsEvtGetUePolicyCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetUePolicyCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
#if (FEATURE_LADN == FEATURE_ON)
VOS_UINT32 AT_RcvMtaLadnInfoQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_RcvTafPsEvtSetCgContExCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
#endif
VOS_UINT32 AT_RcvMtaCsqlvlQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvDrvAgentVertimeQryRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaRrcStatQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaHfreqinfoQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvTafPsEvtSetImsPdpCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_VOID AT_ShowCccRst(NAS_CC_StateInfo *ccState, VOS_UINT16 *length);
VOS_UINT32 At_RcvAtCcMsgStateQryCnfProc(struct MsgCB *msg);

#if (FEATURE_LTE == FEATURE_ON)
#if (FEATURE_LTE_MBMS == FEATURE_ON)
VOS_UINT32 AT_RcvMtaMBMSSib16NetworkTimeQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaMBMSBssiSignalLevelQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaMBMSNetworkInfoQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaMBMSModemStatusQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaMBMSCmdQryCnf(VOS_VOID *msg, VOS_UINT8 indexNum);
VOS_VOID   AT_ReportMBMSCmdQryCnf(MTA_AT_MbmsAvlServiceListQryCnf *lrrcCnf, VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaMBMSServiceEventInd(VOS_VOID *msg, VOS_UINT8 indexNum);
#endif
#endif

VOS_UINT32 AT_RcvDrvAgentAuthVerQryRsp(struct MsgCB *msg);

VOS_UINT32 atQryCellInfoCnfProc(struct MsgCB *msgBlock);
#if (FEATURE_MBB_CUST == FEATURE_ON)
VOS_UINT32 AT_MbbQryCellInfoCnfProc(struct MsgCB *msgBlock);
#endif
VOS_UINT32 AT_RcvMtaNvRefreshSetCnf(VOS_VOID *msg, VOS_UINT8 indexNum);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
