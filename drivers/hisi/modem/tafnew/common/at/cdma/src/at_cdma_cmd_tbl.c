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

#include "at_cdma_cmd_tbl.h"
#include "at_cdma_qry_cmd_proc.h"
#include "at_cdma_set_cmd_proc.h"
#include "AtParse.h"
#include "at_cmd_proc.h"
#include "at_test_para_cmd.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CDMA_CMD_TBL_C

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
static const AT_ParCmdElement g_atCdmaCmdTbl[] = {
    { AT_CMD_CFSH,
      AT_SetCfshPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CFSH", (VOS_UINT8 *)"(@number)" },

    { AT_CMD_CBURSTDTMF,
      AT_SetCBurstDTMFPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_TestCBurstDTMFPara, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CBURSTDTMF", (VOS_UINT8 *)"(1-7),(@key),(95,150,200,250,300,350),(60,100,150,200)" },

    { AT_CMD_CCONTDTMF,
      AT_SetCContinuousDTMFPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_TestCContinuousDTMFPara, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CCONTDTMF", (VOS_UINT8 *)"(1-7),(0,1),(@key)" },

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { AT_CMD_CSIDLIST,
      AT_SetCSidList, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
      (VOS_UINT8 *)"^CSIDLIST", (VOS_UINT8 *)"(0-5),(0-65535),(0-65535),(@number),(0-65535),(0-65535),(@number),(0-65535),(0-65535),(@number),(0-65535),(0-65535),(@number),(0-65535),(0-65535),(@number)" },

    { AT_CMD_CFREQLOCK,
      AT_SetCFreqLockPara, AT_SET_PARA_TIME, AT_QryCFreqLockInfo, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CFREQLOCK", (VOS_UINT8 *)"(0,1),(0-32767),(0-65535),(0),(1-1800),(0-511,65535),(0),(1-1800),(0-511,65535)" },

    { AT_CMD_1XCHAN,
      AT_Set1xChanPara, AT_SET_PARA_TIME, AT_Qry1xChanPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^1XCHAN", (VOS_UINT8 *)"(1-1800)" },
#endif


    { AT_CMD_CDMACSQ,
      AT_SetCdmaCsqPara, AT_SET_PARA_TIME, AT_QryCdmaCsqPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CDMACSQ", (VOS_UINT8 *)"(0,1),(0-20),(1-10),(1-10)" },

    { AT_CMD_HDRCSQ,
      AT_SetHdrCsqPara, AT_SET_PARA_TIME, AT_QryHdrCsqPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^HDRCSQ", (VOS_UINT8 *)"(0,1),(0-20),(1-10),(1-10),(1-10)" },

    { AT_CMD_CLOCINFO,
      AT_SetClocinfoEnable, AT_SET_PARA_TIME, AT_QryCLocInfo, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CLOCINFO", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_CSID,
      AT_SetCSidEnable, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CSID", (VOS_UINT8 *)"(0,1)" },

    /* 移出CDMA编译开关 */

    { AT_CMD_CSNID,
      VOS_NULL_PTR, AT_SET_PARA_TIME, AT_QryCurrSidNid, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CSNID", VOS_NULL_PTR },

#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON)
    { AT_CMD_ECCALL,
      AT_SetEncryptCall, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^ECCALL", (VOS_UINT8 *)"(0-3),(@NUM)" },

    { AT_CMD_ECCTRL,
      AT_SetEncryptCallRemoteCtrl, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^ECCTRL", (VOS_UINT8 *)"(0-1),(0-1)" },

    { AT_CMD_ECCAP,
      AT_SetEncryptCallCap, AT_SET_PARA_TIME, AT_QryEncryptCallCap, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^ECCAP", (VOS_UINT8 *)"(0-2),(0-1)" },

#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)
    { AT_CMD_ECKMC,
      AT_SetEncryptCallKmc, AT_SET_PARA_TIME, AT_QryEncryptCallKmc, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^ECKMC", (VOS_UINT8 *)"(0-255),(@Kmc)" },

    { AT_CMD_ECRANDOM,
      VOS_NULL_PTR, AT_SET_PARA_TIME, AT_QryEncryptCallRandom, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^ECRANDOM", (VOS_UINT8 *)"(1-32),(@RANDOM)" },

    { AT_CMD_ECCTEST,
      AT_SetEccTestMode, AT_SET_PARA_TIME, AT_QryEccTestMode, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^ECCTEST", (VOS_UINT8 *)"(0-1)" },
#endif
#endif

    { AT_CMD_CTROAMINFO,
      AT_SetCtRoamInfo, AT_SET_PARA_TIME, AT_QryCtRoamInfo, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CTROAMINFO", (VOS_UINT8 *)"(0-1)" },

    { AT_CMD_CTOOSCOUNT,
      AT_SetCtOosCount, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CTOOSCOUNT", (VOS_UINT8 *)"(0-65535),(0-65535)" },

    { AT_CMD_CPMP,
      AT_SetPrivacyModePreferred, AT_SET_PARA_TIME, AT_QryPrivacyModePreferred, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CPMP", (VOS_UINT8 *)"(0-1)" },

    { AT_CMD_PRLID,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryPRLID, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^PRLID", VOS_NULL_PTR },

    { AT_CMD_CNOCARDMODE,
      AT_SetNoCardMode, AT_SET_PARA_TIME, AT_QryNoCardMode, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CNOCARDMODE", (VOS_UINT8 *)"(0-1)" },

    { AT_CMD_RATCOMBINEDMODE,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryRatCombinedMode, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^RATCOMBINEDMODE", (VOS_UINT8 *)"(0-1)" },

    { AT_CMD_CCLPR,
      AT_SetCclprPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, AT_TestCclprPara, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CCLPR", (VOS_UINT8 *)"(1-7)" },

    { AT_CMD_CCMGS,
      VOS_NULL_PTR, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
      (VOS_UINT8 *)"^CCMGS", (TAF_UINT8 *)"(712),(cmd)" },

    { AT_CMD_CCMGW,
      VOS_NULL_PTR, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
      (VOS_UINT8 *)"^CCMGW", (TAF_UINT8 *)"(712),(0-3),(cmd)" },

    { AT_CMD_CCMGD,
      AT_SetCcmgdPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CCMGD", (VOS_UINT8 *)"(0-254)" },

    { AT_CMD_CCSASM,
      AT_SetCdmaMemStatusPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (TAF_UINT8 *)"^CCSASM", (TAF_UINT8 *)"(0,1)" },

    { AT_CMD_CCIMI,
      At_SetCCimiPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_E5_IS_LOCKED,
      (VOS_UINT8 *)"^CCIMI", VOS_NULL_PTR },

    { AT_CMD_CDORMTIMER,
      AT_SetCdmaDormantTimer, AT_SET_PARA_TIME, AT_QryCdmaDormTimerVal, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (TAF_UINT8 *)"^CDORMTIMER", (TAF_UINT8 *)"(1-255)" },

    { AT_CMD_CVER,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryProRevInUse, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CVER", VOS_NULL_PTR },

    { AT_CMD_GETSTA,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryCasState, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^GETSTA", VOS_NULL_PTR },

    { AT_CMD_GETESN,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryProGetEsn, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^GETESN", VOS_NULL_PTR },

    { AT_CMD_GETMEID,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryProGetMeid, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^GETMEID", VOS_NULL_PTR },

    { AT_CMD_CHIGHVER,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryHighVer, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CHIGHVER", VOS_NULL_PTR },

    { AT_CMD_MEID,
      AT_SetMeidPara, AT_SET_PARA_TIME, AT_QryMeidPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^MEID", (VOS_UINT8 *)"(@number)" },

    { AT_CMD_DOSYSEVENT,
      AT_SetEvdoSysEvent, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^DOSYSEVENT", (VOS_UINT8 *)"(0-4294967295)" },

    { AT_CMD_DOSIGMASK,
      AT_SetDoSigMask, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^DOSIGMASK", (VOS_UINT8 *)"(0-4294967295)" },

    { AT_CMD_CUSTOMDIAL,
      At_SetCustomDial, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CUSTOMDIAL", (VOS_UINT8 *)"(@number),(2,9,32,55)" },

    { AT_CMD_CDMAMODEMSWITCH,
      At_SetCdmaModemSwitch, AT_TEST_PLATFORM_WRITE_TIME, At_QryCdmaModemSwitch, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CDMAMODEMSWITCH", (VOS_UINT8 *)"(0-2)" },

    { AT_CMD_CDMACAPRESUME,
      At_SetCdmaCapResume, AT_TEST_PLATFORM_WRITE_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CDMACAPRESUME", VOS_NULL_PTR },

    { AT_CMD_FACTORYCDMACAP,
      VOS_NULL_PTR, AT_SET_PARA_TIME, At_QryFactoryCdmaCap, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^FACTORYCDMACAP", VOS_NULL_PTR },

    { AT_CMD_CLDBDOMAINSTATUS,
      AT_SetClDbDomainStatusPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, AT_TestClDbDomainStatusPara, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_NO_LIMITED,
      (TAF_UINT8 *)"^CLDBDOMAINSTATUS", (VOS_UINT8 *)"(0-1)" },

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { AT_CMD_DIVERSITYSWITCH,
      AT_SetCdmaAttDiversitySwitch, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^DIVERSITYSWITCH", (VOS_UINT8 *)"(0,1)" },
#endif

#if ((FEATURE_AGPS == FEATURE_ON) && (FEATURE_XPDS == FEATURE_ON))
    { AT_CMD_CAGPSCFGPOSMODE,
      AT_SetCagpsCfgPosMode, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSCFGPOSMODE", (TAF_UINT8 *)"(1-255),(@data)" },

    { AT_CMD_CAGPSSTART,
      AT_SetCagpsStart, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSSTART", (TAF_UINT8 *)"(1-255),(@data)" },

    { AT_CMD_CAGPSSTOP,
      AT_SetCagpsStop, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSSTOP", VOS_NULL_PTR },

    { AT_CMD_CAGPSCFGMPCADDR,
      AT_SetCagpsCfgMpcAddr, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSCFGMPCADDR", (TAF_UINT8 *)"(1-255),(@data)" },

    { AT_CMD_CAGPSCFGPDEADDR,
      AT_SetCagpsCfgPdeAddr, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSCFGPDEADDR", (TAF_UINT8 *)"(1-65535),(@data)" },

    { AT_CMD_CAGPSQRYREFLOC,
      AT_SetCagpsQryRefloc, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSQRYREFLOC", VOS_NULL_PTR },

    { AT_CMD_CAGPSQRYTIME, /* ^CAGPSTIMEINFO */
      AT_SetCagpsQryTime, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSQRYTIME", (TAF_UINT8 *)"(1-255),(@data)" },

    { AT_CMD_CAGPSPRMINFO,
      AT_SetCagpsPrmInfo, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSPRMINFO", (TAF_UINT8 *)"(1-65535),(@data)" },

    { AT_CMD_CAGPSREPLYNIREQ, /* ^CAGPSREFLOCINFO */
      AT_SetCagpsReplyNiReq, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSREPLYNIREQ", (TAF_UINT8 *)"(1-255),(@data)" },

    { AT_CMD_CAGPSDATACALLSTATUS, /* ^CAGPSDATACALLSTATUS */
      At_SetAgpsDataCallStatus, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSDATACALLSTATUS", (TAF_UINT8 *)"(0,1)" },

    { AT_CMD_CUPBINDSTATUS, /* ^CAGPSUPBINDSTATUS */
      At_SetAgpsUpBindStatus, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CUPBINDSTATUS", (TAF_UINT8 *)"(0-2),(0,1)" },

    { AT_CMD_CAGPSFORWARDDATA, /* ^CAGPSFORWARDDATA */
      At_SetAgpsForwardData, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSFORWARDDATA", (TAF_UINT8 *)"(0-2),(1-255),(1-255),(1-500),(@data)" },

    { AT_CMD_CAGPSPOSINFO, /* ^CAGPSPOSINFO */
      At_SetCagpsPosInfo, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CAGPSPOSINFO", (TAF_UINT8 *)"(504),(@data)" },

    { AT_CMD_CGPSCONTROLSTART,
      At_SetCgpsControlStart, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CGPSCONTROLSTART", (TAF_UINT8 *)"(1-255),(@data)" },

    { AT_CMD_CGPSCONTROLSTOP,
      At_SetCgpsControlStop, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CGPSCONTROLSTOP", (TAF_UINT8 *)"(1-255),(@data)" },
#endif
};
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
/* 注册CDMA AT命令表 */
VOS_UINT32 AT_RegisterCdmaCmdTable(VOS_VOID)
{
    return AT_RegisterCmdTable(g_atCdmaCmdTbl, sizeof(g_atCdmaCmdTbl) / sizeof(g_atCdmaCmdTbl[0]));
}
#endif

