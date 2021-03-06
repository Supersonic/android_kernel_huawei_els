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
#include "at_device_taf_set_cmd_proc.h"
#include "securec.h"
#include "at_set_para_cmd.h"
#include "at_cmd_proc.h"
#include "taf_type_def.h"
#include "taf_drv_agent.h"
#include "at_mta_interface.h"
#include "osm.h"
#include "at_check_func.h"
#include "at_init.h"
#include "nv_stru_gucnas.h"
#include "at_device_cmd.h"
#include "at_mdrv_interface.h"
#include "at_device_comm.h"
#include "at_lte_common.h"
#if (FEATURE_MBB_CUST == FEATURE_ON)
#include "mbb_cust_api.h"
#endif


#define THIS_FILE_ID PS_FILE_ID_AT_DEVICE_TAF_SET_CMD_PROC_C

#define AT_NVREAD_STATUS_1 1
#define AT_NVREAD_STATUS_2 2
#define AT_NVREAD_STATUS_3 3
#define AT_NVREAD_STATUS_4 4
#define AT_NVREAD_STATUS_5 5
#define AT_NVREAD_STATUS_6 6
#define AT_NVREAD_STATUS_7 7
#define AT_NVWR_STATUS_2 2
#define AT_NVWR_STATUS_3 3
#define AT_NVWR_STATUS_4 4
#define AT_NVWR_STATUS_5 5
#define AT_NVWR_STATUS_6 6
#define AT_NVWR_STATUS_7 7
#define AT_NVWR_STATUS_8 8
#define AT_NVWR_STATUS_9 9
#define AT_NVWR_STATUS_10 10
#define AT_NVWR_STATUS_11 11
#define AT_NV_NUM_PER_MAX_NUM 128
#define AT_NVWR_PARA_NVID 0
#define AT_NVWR_PARA_NV_TOTAL_LEN 1
#define AT_NVWR_PARA_NV_LEN 2
#define AT_NVWR_PARA_DATA 3
#define AT_NVRDEX_PARA_NUM 3
#define AT_NVRDEX_NVID 0
#define AT_NVRDEX_OFFSET 1
#define AT_NVRDEX_RDLEN 2
#define AT_NVWREX_NVID 0
#define AT_NVWREX_OFFFSET 1
#define AT_NVWREX_LENGTH 2
#define AT_NVWREX_DATA 3
#define AT_PARA_LIST_OFFSET_NUM_2 2
#define AT_PARA_LIST_OFFSET_NUM_3 3
#define AT_DATA_MAX_LENGTH 128
#define AT_FACINFO_VALUE_INFO 1
#define AT_FACINFO_PARA_VALID_NUM 2
#define AT_PHYNUM_PARA_MIN_VALID_NUM 2
#define AT_PHYNUM_PARA_MAX_VALID_NUM 3
#define AT_WIWEP_PARA_WIFI_KEY_INDEX 0
#define AT_WIWEP_PARA_INDEX_VAL_0 0
#define AT_WIWEP_PARA_INDEX_VAL_1 1
#define AT_WIWEP_PARA_INDEX_VAL_2 2
#define AT_WIWEP_PARA_WIFI_KEY_CONTENT 1
#define AT_WIWEP_SSID_GROUP_INDEX 2
#define AT_CMDLEN_PARA_NUM 2
#define AT_PC_EXPECT_TX_MSG_LEN 0
#define AT_PC_EXPECT_RX_MSG_LEN 1
#define AT_PHYNUM_TIMES 2
#define AT_RSFW_PARA_MAX_NUM 6
#define AT_RSFW_ITEM_NAME 0
#define AT_RSFW_ITME 5
#define AT_CHAR_AT_LEN 2
#define AT_SIMLOCK_INDEX_A 0
#define AT_SIMLOCK_INDEX_T 1
#define AT_SIMLOCK_INDEX_EQUAL_SIGN 10
#define AT_SIMLOCK_INDEX_1 11
#define AT_SIMLOCK_INDEX_COMMA 12
#define AT_SIMLOCK_UNLOCK_PASSWORD_LEN 8
#define AT_SIMLOCK_MIN_LEN 13
#define AT_MULTIUPGMODE_PARA_NUM      1
#define AT_MULTIUPGMODE_PARA_RESERVE2 0 /* MULTIUPGMODE????????????RESERVE2 */
#define AT_FPA_LEVEL 0
#define AT_WUPWD_PARA_MAX_NUM 2
#define AT_SIMLOCKDATAWRITEEX_LAYER_FOUR_VALID_VALUE 4
#define AT_SIMLOCKDATAWRITEEX_LAYER_MAX_VALID_VALUE 255
#define AT_SIMLOCKDATAWRITEEX_ULINDEX_MAX_VALID_VALUE 255
#define AT_SIMLOCKDATAWRITEEX_ULTOTAL_MAX_VALID_VALUE 255
#define AT_SIMLOCKDATAWRITEEX_STR_MIN_LEN 6
#define AT_SIMLOCKOTADATAWRITE_STR_MIN_LEN 6
#define AT_COMMPOS_LENGTH 4
#define AT_FIRST_COMMPOS 0
#define AT_SECOND_COMMPOS 1
#define AT_THIRD_COMMPOS 2
#define AT_FOURTH_COMMPOS 3
#define AT_PHONEPHYNUM_PARA_NUM 3
#define AT_PHONEPHYNUM_PARA_TYPE_NUM 0
#define AT_PHONEPHYNUM_PARA_RSA_NUM 1
#define AT_SPECIAL_CMD_MAX_LEN 23
#define AT_SIMLOCK_DATA_LEN 4
#define AT_BASE_64_CODING_CHAR_MAX_NUM 64
#define AT_LETTERS_NUM 26
#define AT_BASE64_DECODE_ARRAY_MAX_LEN 256

#define AT_RAT_MODE_GSM_BASE_FREQ 66700
#define AT_RAT_MODE_WCDMA_BASE_FREQ 480000
#define AT_RAT_MODE_CDMA_BASE_FREQ 153600
#define AT_RAT_MODE_LTE_BASE_FREQ 480000
#define AT_RAT_MODE_NR_BASE_FREQ 480000
#define AT_RAT_MODE_DEAFAULT_BASE_FREQ 480000

VOS_UINT32 g_nvRead  = 0;
VOS_UINT32 g_nvWrite = 0;

VOS_UINT32 g_nvReadTl  = 0;
VOS_UINT32 g_nvWriteTl = 0;

static const AT_SpecialCmdFuncTbl g_ApModemSpecialCmdFucTab[] = {
    /* AT^IDENTIFYEND= ???????? */
    { "AT^IDENTIFYEND=", AT_HandleIdentifyEndCmd },
    /* AT^IDENTIFYOTAEND= ???????? */
    { "AT^IDENTIFYOTAEND=", AT_HandleIdentifyEndCmd },
    /* AT^PHONEPHYNUM= ???????? */
    { "AT^PHONEPHYNUM=", AT_HandlePhonePhynumCmd },
};

VOS_UINT32 AT_NVWRGetItemValue(VOS_UINT8 *para, VOS_UINT16 paraMaxLen, VOS_UINT8 *value, VOS_UINT8 **paraDst)
{
    VOS_UINT32 tmp   = 0;
    VOS_UINT32 i     = 0;
    VOS_UINT8 *start = para;
    VOS_UINT16 len   = 0;
    VOS_UINT8 *end   = VOS_NULL_PTR;
    VOS_UINT32 ret;

    /* ??????????????pucPara,pucValue,pucParaDst????NULL */

    /* ??????????????' '?????? */
    while ((i < paraMaxLen) && (*start == ' ')) {
        start++;
        i++;
    }

    /*
     * at^nvwr??NV??????????????IE????????????????????????????????????????
     * 1??????????IE????????????????????????2??????????????????????????
     */
    if ((i + 2 < paraMaxLen) && (*(start + 1) == ' ')) {
        len = 1;         /* 1??????????IE?????????????????????? */
        end = start + 2; /* 2?????????????????????????? */
    /* 2??????????IE????????????????????????3?????????????????????????? */
    } else if ((i + 3 < paraMaxLen ) && (*(start + 2) == ' ')) {
        len = 2;         /* 2??????????IE?????????????????????? */
        end = start + 3; /* 3?????????????????????????? */
    } else {
        return VOS_ERR;
    }

    ret = AT_String2Hex(start, len, &tmp);
    if ((ret != VOS_OK) || (tmp > 0xff)) {
        return VOS_ERR;
    }

    *value   = (VOS_UINT8)tmp;
    *paraDst = end;

    return VOS_OK;
}

VOS_UINT32 AT_NVWRGetParaInfo(AT_ParseParaType *para, VOS_UINT8 *pu8Data, VOS_UINT32 bufLen, VOS_UINT32 *len)
{
    VOS_UINT32 num;
    VOS_UINT8 *pu8Start   = VOS_NULL_PTR;
    VOS_UINT8 *pu8ParaTmp = VOS_NULL_PTR;
    VOS_UINT16 lenTemp;
    VOS_UINT32 ret;
    VOS_UINT16 i = 0;

    /* ??????????????pstPara,pu8Data????NULL */

    pu8Start = para->para;
    lenTemp  = para->paraLen;

    if (lenTemp < AT_PARA_MAX_LEN) {
        para->para[lenTemp] = ' ';
    } else {
        return VOS_ERR;
    }

    num = 0;
    for (i = 0; i < lenTemp;) {
        /* ??????????????????????????????????????????????para????????????+2 */
        ret = AT_NVWRGetItemValue(pu8Start, lenTemp + 2, (pu8Data + num), &pu8ParaTmp);
        if (ret != ERR_MSP_SUCCESS) {
            return ret;
        }

        num++;

        /* ????128?? */
        /* MAX_NV_NUM_PER_PARA */
        if ((num == bufLen) || (num == AT_NV_NUM_PER_MAX_NUM)) {
            break;
        }

        if (pu8ParaTmp >= (pu8Start + lenTemp)) {
            break;
        }

        i += (VOS_UINT16)(pu8ParaTmp - pu8Start);
        pu8Start = pu8ParaTmp;
    }

    *len = num;
    return VOS_OK;
}

VOS_VOID AT_GetNvRdDebug(VOS_VOID)
{
    PS_PRINTF_INFO("g_nvRead=0x%x \n", g_nvRead);
    PS_PRINTF_INFO("g_nvWrite=0x%x \n", g_nvWrite);
}

/*
 * ????????: ????AT????????
 */
LOCAL VOS_UINT32 AT_CheckNVReadPara(VOS_UINT16 *nvId)
{
    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        g_nvRead = AT_NVREAD_STATUS_1;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ???????? */
    if (g_atParaIndex > 1) {
        g_nvRead = AT_NVREAD_STATUS_2;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ???????? */
    if (g_atParaList[0].paraLen == 0) {
        g_nvRead = AT_NVREAD_STATUS_3;
        return AT_CME_INCORRECT_PARAMETERS;
    } else {
        *nvId = (VOS_UINT16)g_atParaList[0].paraValue;
    }

#if (FEATURE_MBB_CUST == FEATURE_ON)
    if (AT_JudgeNvOperationValid() != VOS_TRUE) {
        return AT_ERROR;
    }
#endif

    return AT_SUCCESS;
}

#if (FEATURE_MBB_CUST == FEATURE_ON)
VOS_BOOL AT_JudgeNvOperationValid(VOS_VOID)
{
    if (TAF_SDC_GetMbbCustFunTabAddr()->TAF_JudgeNvOperationValid == VOS_NULL_PTR) {
        return VOS_TRUE;
    }
    return TAF_SDC_GetMbbCustFunTabAddr()->TAF_JudgeNvOperationValid();
}
#endif

VOS_UINT32 AT_SetNVReadPara(VOS_UINT8 indexNum)
{
    VOS_UINT16    nvId    = 0;
    VOS_UINT32    nvLen   = 0;
    VOS_UINT8    *data    = VOS_NULL_PTR;
    VOS_UINT32    i       = 0;
    ModemIdUint16 modemId = MODEM_ID_0;
    VOS_UINT32    ret;

    AT_PR_LOGI("Rcv Msg");

    ret = AT_CheckNVReadPara(&nvId);

    if (ret != AT_SUCCESS) {
        return ret;
    }

    ret = TAF_ACORE_NV_GET_LENGTH(nvId, &nvLen);

    if (ret != VOS_OK) {
        g_nvRead = AT_NVREAD_STATUS_4;
        return AT_ERROR;
    }

    if (nvLen == 0) {
        AT_WARN_LOG("data cannot malloc space !");
        return AT_ERROR;
    }
    /* ????????128????????128?????? */
    /*lint -save -e516 */
    data = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, nvLen);
    /*lint -restore */
    if (data == VOS_NULL_PTR) {
        g_nvRead = AT_NVREAD_STATUS_5;
        return AT_ERROR;
    }
    memset_s(data, nvLen, 0x00, nvLen);

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_SetNVReadPara:Get modem id fail");
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, data);
        /*lint -restore */
        g_nvWrite = AT_NVWR_STATUS_8;
        return AT_ERROR;
    }

    ret = TAF_ACORE_NV_READ(modemId, nvId, data, nvLen);

    if (ret != VOS_OK) {
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, data);
        /*lint -restore */
        g_nvRead = AT_NVREAD_STATUS_6;
        return AT_ERROR;
    }
    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^NVRD: %d,", nvLen);

    for (i = 0; i < nvLen; i++) {
        if (i == 0) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%02X",
                data[i]);
        } else {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, " %02X",
                data[i]);
        }
    }
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, data);
    /*lint -restore */
    g_nvRead = AT_NVREAD_STATUS_7;
    return AT_OK;
}


// *****************************************************************************
// o????y??3?: atSetNVRDLenPara
// 1|?????????: ???NV3?????Y
// 2???y??????:
//   ulIndex [in] ???????????y
// ???? ?? ???:
//    TODO: ...
// ?????????a???: TODO: ...
// ????????????y: TODO: ...
// ????    ??: ???????1?/00193980 [2013-02-24]
// *****************************************************************************
VOS_UINT32 atSetNVRDLenPara(VOS_UINT8 clientId)
{
    VOS_UINT32 ret;
    VOS_UINT16 nvid  = 0;
    VOS_UINT32 nVLen = 0;

    /* PCLINT */
    if (clientId == 0) {
    }

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        g_nvReadTl = 1;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex > 1) {
        g_nvReadTl = 2;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        g_nvReadTl = 3;
        return AT_CME_INCORRECT_PARAMETERS;
    } else {
        nvid = (VOS_UINT16)g_atParaList[0].paraValue;
    }
    ret = TAF_ACORE_NV_GET_LENGTH(nvid, &nVLen);

    if (ret != ERR_MSP_SUCCESS) {
        g_nvReadTl = 4;
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^NVRDLEN: %d", nVLen);
    g_nvReadTl              = 7;
    return AT_OK;
}

LOCAL VOS_UINT32 AT_SetNvRdExPreCheck(VOS_VOID)
{
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        g_nvReadTl = 1;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != AT_NVRDEX_PARA_NUM) {
        g_nvReadTl = 2;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_NVRDEX_NVID].paraLen == 0 || g_atParaList[AT_NVRDEX_OFFSET].paraLen == 0 ||
        g_atParaList[AT_NVRDEX_RDLEN].paraLen == 0) {
        g_nvReadTl = 3;
        return AT_CME_INCORRECT_PARAMETERS;
    }
    return AT_SUCCESS;
}

VOS_UINT32 atSetNVRDExPara(VOS_UINT8 clientId)
{
    VOS_UINT32 ret;
    VOS_UINT16 nvid;
    VOS_UINT32 nVLen = 0;
    VOS_UINT16 offset;
    VOS_UINT16 rdLen;
    VOS_UINT8 *pu8Data = NULL;
    VOS_UINT32 i       = 0;

    AT_PR_LOGI("Rcv Msg");

    /* PCLINT */
    if (clientId == 0) {
    }

    ret = AT_SetNvRdExPreCheck();
    if (ret != AT_SUCCESS) {
        return ret;
    }

    nvid   = (VOS_UINT16)g_atParaList[AT_NVRDEX_NVID].paraValue;
    offset = (VOS_UINT16)g_atParaList[AT_NVRDEX_OFFSET].paraValue;
    rdLen  = (VOS_UINT16)g_atParaList[AT_NVRDEX_RDLEN].paraValue;

#if (FEATURE_MBB_CUST == FEATURE_ON)
    if (AT_JudgeNvOperationValid() != VOS_TRUE) {
        return AT_ERROR;
    }
#endif

    ret = TAF_ACORE_NV_GET_LENGTH(nvid, &nVLen);

    AT_PR_LOGI("Call interface success!");

    if (ret != ERR_MSP_SUCCESS) {
        g_nvReadTl = 4;
        return AT_ERROR;
    }

    if ((((VOS_UINT32)offset + 1) > nVLen) || (((VOS_UINT32)offset + rdLen) > nVLen)) {
        g_nvReadTl = 5;
        return AT_ERROR;
    }

    pu8Data = VOS_MemAlloc(WUEPS_PID_AT, (DYNAMIC_MEM_PT), nVLen);
    if (pu8Data == NULL) {
        g_nvReadTl = 6;
        return AT_ERROR;
    }
    memset_s(pu8Data, nVLen, 0x00, nVLen);

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, (VOS_UINT32)nvid, pu8Data, nVLen);

    if (ret != ERR_MSP_SUCCESS) {
        VOS_MemFree(WUEPS_PID_AT, pu8Data);
        g_nvReadTl = 7;
        return AT_ERROR;
    }
    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^NVRDEX: %d,%d,%d,", nvid, offset, rdLen);

    for (i = offset; i < ((VOS_UINT32)offset + rdLen); i++) {
        if (offset == i) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%02X",
                pu8Data[i]);
        } else {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, " %02X",
                pu8Data[i]);
        }
    }

    VOS_MemFree(WUEPS_PID_AT, pu8Data);
    g_nvReadTl = 8;
    return AT_OK;
}


TAF_UINT32 At_CheckNumCharString(TAF_UINT8 *data, TAF_UINT16 len)
{
    TAF_UINT32 length = 0;
    TAF_UINT8 *pcTmp  = data;
    TAF_UINT32 checkDigitRslt;
    TAF_UINT32 checkCharRslt;

    if ((data == TAF_NULL_PTR) || (len == 0)) {
        return AT_FAILURE;
    }

    pcTmp = data;

    while (length++ < len) {
        checkDigitRslt = At_CheckDigit(*pcTmp);
        checkCharRslt  = At_CheckAlpha(*pcTmp);

        if ((checkDigitRslt == AT_SUCCESS) || (checkCharRslt == AT_SUCCESS)) {
            pcTmp++;
        } else {
            return AT_FAILURE;
        }
    }
    return AT_SUCCESS;
}

TAF_UINT32 At_SetBsn(TAF_UINT8 indexNum)
{
    TAF_PH_SerialNum serialNum;
    errno_t          memResult;

    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* ???????? */
    if (g_atParaIndex > 1) {
        return AT_ERROR;
    }

    /* ????????????<imei>??????16?????????????? */
    if (g_atParaList[0].paraLen != TAF_SERIAL_NUM_LEN) {
        return AT_ERROR;
    }

    /* ????<imei>????????????????,?????????????????? */
    if (At_CheckNumCharString(g_atParaList[0].para, g_atParaList[0].paraLen) == AT_FAILURE) {
        return AT_ERROR;
    }

    if (g_atParaList[0].paraLen > 0) {
        memResult = memcpy_s(serialNum.serialNum, sizeof(serialNum.serialNum), g_atParaList[0].para,
                             g_atParaList[0].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(serialNum.serialNum), g_atParaList[0].paraLen);
    }
    (VOS_VOID)memset_s(serialNum.serialNum + TAF_SERIAL_NUM_LEN,
                (VOS_SIZE_T)((TAF_SERIAL_NUM_NV_LEN - TAF_SERIAL_NUM_LEN) * sizeof(serialNum.serialNum[0])),
                0x00,
                (VOS_SIZE_T)((TAF_SERIAL_NUM_NV_LEN - TAF_SERIAL_NUM_LEN) * sizeof(serialNum.serialNum[0])));

    if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_MANUFACTURE_INFO, serialNum.serialNum, TAF_SERIAL_NUM_NV_LEN) != NV_OK) {
        return AT_ERROR;
    } else {
        return AT_OK;
    }
}

VOS_UINT32 At_WriteSnPara(VOS_UINT8 indexNum)
{
    TAF_PH_SerialNum serialNum;
    errno_t          memResult;

    /* ????????????1 */
    if (g_atParaIndex != 1) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ??????????????????16?????????????? */
    if (g_atParaList[0].paraLen != TAF_SERIAL_NUM_LEN) {
        return AT_SN_LENGTH_ERROR;
    }

    /* ????????????????????????????????,?????????????????? */
    if (At_CheckNumCharString(g_atParaList[0].para, g_atParaList[0].paraLen) == AT_FAILURE) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ??????????16??SN????????????????stSerialNum.aucSerialNum?? */
    memResult = memcpy_s(serialNum.serialNum, sizeof(serialNum.serialNum), g_atParaList[0].para, TAF_SERIAL_NUM_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(serialNum.serialNum), TAF_SERIAL_NUM_LEN);

    /* ??stSerialNum.aucSerialNum????????????0xFF */
    (VOS_VOID)memset_s((serialNum.serialNum + TAF_SERIAL_NUM_LEN), TAF_SERIAL_NUM_NV_LEN - TAF_SERIAL_NUM_LEN, 0xFF,
                       (TAF_SERIAL_NUM_NV_LEN - TAF_SERIAL_NUM_LEN));

    if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_SERIAL_NUM, serialNum.serialNum, TAF_SERIAL_NUM_NV_LEN) != NV_OK) {
        AT_WARN_LOG("At_WriteSnPara:WARNING:TAF_ACORE_NV_WRITE NV_ITEM_SERIAL_NUM failed!");
        return AT_DEVICE_OTHER_ERROR;
    }

    return AT_OK;
}

TAF_UINT32 At_SetSnPara(TAF_UINT8 indexNum)
{
    TAF_PH_SerialNum serialNum;
    TAF_UINT16       length;

    /* ???????? */
    if (g_atParseCmd.cmdOptType == AT_CMD_OPT_SET_PARA_CMD) {
        return At_WriteSnPara(indexNum);
    }
    /* ??NV?????? Serial Num,?????????????? */

    (VOS_VOID)memset_s(&serialNum, sizeof(serialNum), 0x00, sizeof(TAF_PH_SerialNum));

    length = TAF_SERIAL_NUM_NV_LEN;
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_SERIAL_NUM, serialNum.serialNum, length) != NV_OK) {
        AT_WARN_LOG("At_SetSnPara:WARNING:NVIM Read NV_ITEM_SERIAL_NUM falied!");
        return AT_ERROR;
    } else {
        (VOS_VOID)memset_s((serialNum.serialNum + TAF_SERIAL_NUM_LEN),
                           (VOS_SIZE_T)((TAF_SERIAL_NUM_NV_LEN - TAF_SERIAL_NUM_LEN) * sizeof(serialNum.serialNum[0])),
                           0x00, (VOS_SIZE_T)((TAF_SERIAL_NUM_NV_LEN - TAF_SERIAL_NUM_LEN) * sizeof(serialNum.serialNum[0])));
        length = 0;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", serialNum.serialNum);
    }
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

VOS_UINT32 AT_CheckTmodePara(VOS_VOID)
{
    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return VOS_FALSE;
    }

    /* ???????? */
    if (g_atParaIndex > 1) {
        return VOS_FALSE;
    }

    /* ????TMODE */
    if (g_atParaList[0].paraValue >= AT_TMODE_BUTT) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

VOS_UINT32 AT_SetTmodeNormal(TAF_PH_TmodeUint8 *currentTMode, TAF_UINT8 indexNum)
{
    TAF_MMA_PhoneModePara phoneModePara;

    (VOS_VOID)memset_s(&phoneModePara, sizeof(phoneModePara), 0x00, sizeof(phoneModePara));

    phoneModePara.phMode = TAF_PH_MODE_FULL;

    if (TAF_MMA_PhoneModeSetReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &phoneModePara) ==
        VOS_TRUE) {
        /* ???????????????? */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_TMODE;

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
        AT_InitDevCmdCtrl();
#else
        AT_InitMTInfo();
#endif

        *currentTMode = AT_TMODE_NORMAL;

        return AT_WAIT_ASYNC_RETURN; /* ???????????????????? */
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_SetTmodeFtm(TAF_PH_TmodeUint8 *currentTMode, TAF_UINT8 indexNum)
{
    TAF_MMA_PhoneModePara phoneModePara;

    (VOS_VOID)memset_s(&phoneModePara, sizeof(phoneModePara), 0x00, sizeof(phoneModePara));

    if (*currentTMode == AT_TMODE_UPGRADE) {
        return AT_ERROR;
    }

    phoneModePara.phMode = TAF_PH_MODE_FT;

    if (TAF_MMA_PhoneModeSetReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &phoneModePara) ==
        VOS_TRUE) {
        /* ???????????????? */
        *currentTMode                         = AT_TMODE_FTM;
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_TMODE;
        return AT_WAIT_ASYNC_RETURN; /* ???????????????????? */
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_SetTmodeUpgrade(TAF_PH_TmodeUint8 *currentTMode, TAF_UINT8 indexNum)
{
    VOS_UINT8             updateFlag;

    if (*currentTMode == AT_TMODE_FTM) {
        return AT_ERROR;
    }
    /*
     * ???????? C ??????????????????????????????????????????????DRV_AGENT_TMODE_SET_REQ
     * ????????AT_OK????????????????????
     */
    updateFlag = VOS_TRUE;

    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                               DRV_AGENT_TMODE_SET_REQ, &updateFlag, sizeof(updateFlag),
                               I0_WUEPS_PID_DRV_AGENT) != TAF_SUCCESS) {
        AT_ERR_LOG("AT_SetTmodeUpgrade: AT_FillAndSndAppReqMsg fail.");
    }

    *currentTMode = AT_TMODE_UPGRADE;
    return AT_OK;
}

VOS_UINT32 AT_SetTmodeReset(TAF_PH_TmodeUint8 *currentTMode, TAF_UINT8 indexNum)
{
    VOS_UINT8             updateFlag;

    *currentTMode = AT_TMODE_RESET;

    /* ???????? */
    updateFlag = VOS_FALSE;

    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                               DRV_AGENT_TMODE_SET_REQ, &updateFlag, sizeof(updateFlag),
                               I0_WUEPS_PID_DRV_AGENT) != TAF_SUCCESS) {
        AT_ERR_LOG("AT_SetPstandbyPara: AT_FillAndSndAppReqMsg fail.");
    }

    return AT_OK;
}

VOS_UINT32 AT_SetTmodeAutoPowerOff(TAF_PH_TmodeUint8 *currentTMode, VOS_UINT8 indexNum)
{
    TAF_MMA_PhoneModePara phModeSet;

    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    memset_s(&phModeSet, sizeof(phModeSet), 0x00, sizeof(TAF_MMA_PhoneModePara));

    systemAppConfig = AT_GetSystemAppConfigAddr();

    if (*systemAppConfig == SYSTEM_APP_WEBUI) {
        /* ????????C???????????????? */
        /*
         * ??????????AT??????????????????????????????????????????????????????????
         * ????????????????????????????????????????
         */

        phModeSet.phMode = TAF_PH_MODE_POWEROFF;

        if (TAF_MMA_PhoneModeSetReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &phModeSet) == VOS_TRUE) {
            return AT_OK;
        } else {
            return AT_ERROR;
        }
    }

    return AT_ERROR;
}

VOS_UINT32 AT_SetTmodeSignaling(TAF_PH_TmodeUint8 *currentTMode, TAF_UINT8 indexNum)
{
    TAF_MMA_PhoneModePara phoneModePara;

    (VOS_VOID)memset_s(&phoneModePara, sizeof(phoneModePara), 0x00, sizeof(phoneModePara));

    if (*currentTMode == AT_TMODE_UPGRADE) {
        return AT_ERROR;
    }

    phoneModePara.phMode = TAF_PH_MODE_FT;

    if (TAF_MMA_PhoneModeSetReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &phoneModePara) ==
        VOS_TRUE) {
        /* ???????????????? */
        *currentTMode                         = AT_TMODE_SIGNALING;
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_TMODE;
        return AT_WAIT_ASYNC_RETURN; /* ???????????????????? */
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_SetTmodeOffline(TAF_PH_TmodeUint8 *currentTMode, TAF_UINT8 indexNum)
{
    TAF_MMA_PhoneModePara phoneModePara;

    (VOS_VOID)memset_s(&phoneModePara, sizeof(phoneModePara), 0x00, sizeof(phoneModePara));

    phoneModePara.phMode = TAF_PH_MODE_FULL;

    if (TAF_MMA_PhoneModeSetReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &phoneModePara) ==
        VOS_TRUE) {
        /* ???????????????? */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_TMODE;

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
        AT_InitDevCmdCtrl();
#else
        AT_InitMTInfo();
#endif

        *currentTMode = AT_TMODE_OFFLINE;

        return AT_WAIT_ASYNC_RETURN; /* ???????????????????? */
    } else {
        return AT_ERROR;
    }
}
#if ((FEATURE_LTE == FEATURE_ON) || (FEATURE_UE_MODE_TDS == FEATURE_ON))

VOS_UINT32 AT_SetTmodeOthers(TAF_PH_TmodeUint8 *currentTMode, TAF_UINT8 indexNum)
{
    if (atSetTmodePara(indexNum, g_atParaList[0].paraValue) == AT_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SET_TMODE;
        *currentTMode                         = (VOS_UINT8)g_atParaList[0].paraValue;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif

TAF_UINT32 At_SetTModePara(TAF_UINT8 indexNum)
{
    TAF_PH_TmodeUint8 *currentTMode = VOS_NULL_PTR;
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
#else
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
#endif
    TAF_MMA_PhoneModePara phoneModePara;

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    atDevCmdCtrl = AT_GetDevCmdCtrl();
    currentTMode = &atDevCmdCtrl->currentTMode;
#else
    atMtInfoCtx = AT_GetMtInfoCtx();
    currentTMode = &atMtInfoCtx->currentTMode;
#endif

    (VOS_VOID)memset_s(&phoneModePara, sizeof(phoneModePara), 0x00, sizeof(phoneModePara));

    /* ???????? */
    if (AT_CheckTmodePara() != VOS_TRUE) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    switch (g_atParaList[0].paraValue) {
        case AT_TMODE_NORMAL:
            return AT_SetTmodeNormal(currentTMode, indexNum);
        case AT_TMODE_FTM:
            return AT_SetTmodeFtm(currentTMode, indexNum);
        case AT_TMODE_UPGRADE:
            return AT_SetTmodeUpgrade(currentTMode, indexNum);
        case AT_TMODE_RESET:
            return AT_SetTmodeReset(currentTMode, indexNum);
        case AT_TMODE_POWEROFF:
            return AT_SetTmodeAutoPowerOff(currentTMode, indexNum);

            /* ??????????????4->11??????1???????????? */
        case AT_TMODE_SIGNALING:
            return AT_SetTmodeSignaling(currentTMode, indexNum);

            /* ??????????????5->12??????0???????????? */
        case AT_TMODE_OFFLINE:
            return AT_SetTmodeOffline(currentTMode, indexNum);

#if ((FEATURE_LTE == FEATURE_ON) || (FEATURE_UE_MODE_TDS == FEATURE_ON))
        case AT_TMODE_F_NONESIGNAL:
        case AT_TMODE_SYN_NONESIGNAL:
        case AT_TMODE_SET_SECONDARY:
        case AT_TMODE_GU_BT:
        case AT_TMODE_COMM_CT:
            return AT_SetTmodeOthers(currentTMode, indexNum);
#endif

        default:
            break;
    }

    return AT_ERROR;
}

VOS_UINT32 AT_SetTmmiPara(VOS_UINT8 indexNum)
{
    errno_t    memResult;
    VOS_UINT32 result;
    VOS_UINT8  facInfo[AT_FACTORY_INFO_LEN];

    /* ???????? */
    if (g_atParaIndex > 1) {
        return AT_TOO_MANY_PARA;
    }

    /* ???????????? */
    if (g_atParaList[0].paraLen != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    (VOS_VOID)memset_s(facInfo, sizeof(facInfo), 0x00, AT_FACTORY_INFO_LEN);

    /* ????en_NV_Item_Factory_Info????????24????????????4???????????????????????? */
    result = TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_Factory_Info, facInfo, AT_FACTORY_INFO_LEN);

    if (result != NV_OK) {
        AT_ERR_LOG("AT_SetTmmiPara: NV Read Fail!");
        return AT_ERROR;
    }

    /* ???? ????"ST P" */
    if (g_atParaList[0].paraValue == 1) {
        memResult = memcpy_s(&facInfo[AT_MMI_TEST_FLAG_OFFSET], AT_FACTORY_INFO_LEN - AT_MMI_TEST_FLAG_OFFSET,
                             AT_MMI_TEST_FLAG_SUCC, AT_MMI_TEST_FLAG_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_FACTORY_INFO_LEN - AT_MMI_TEST_FLAG_OFFSET, AT_MMI_TEST_FLAG_LEN);
    }
    /* ???? ???? */
    else {
        memResult = memcpy_s(&facInfo[AT_MMI_TEST_FLAG_OFFSET], AT_FACTORY_INFO_LEN - AT_MMI_TEST_FLAG_OFFSET, "0000",
                             AT_MMI_TEST_FLAG_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_FACTORY_INFO_LEN - AT_MMI_TEST_FLAG_OFFSET, AT_MMI_TEST_FLAG_LEN);
    }

    result = TAF_ACORE_NV_WRITE(MODEM_ID_0, en_NV_Item_Factory_Info, facInfo, AT_FACTORY_INFO_LEN);

    if (result != NV_OK) {
        return AT_ERROR;
    } else {
        return AT_OK;
    }
}

VOS_UINT32 AT_SetTestScreenPara(VOS_UINT8 indexNum)
{
    return AT_OK;
}

VOS_UINT32 AT_SetProdNamePara(VOS_UINT8 indexNum)
{
    TAF_AT_ProductId productId;
    VOS_UINT32       productIdLen;
    VOS_UINT32       ret;
    errno_t          memResult;

    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* ???????? */
    if (g_atParaIndex > 1) {
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&productId, sizeof(productId), 0x00, sizeof(TAF_AT_ProductId));

    /* ??NV50048?????????????? */
    /* ????NV??NV_ITEM_PRODUCT_ID */
    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_PRODUCT_ID, &productId,
                            sizeof(productId.nvStatus) + sizeof(productId.productId));

    if (ret != NV_OK) {
        AT_WARN_LOG("AT_SetProdNamePara: Fail to read NV_ITEM_PRODUCT_ID");
        return AT_ERROR;
    }

    /* ????????????NV??????????????AT_PRODUCT_NAME_MAX_NUM 30?????????? */
    productIdLen = (g_atParaList[0].paraLen > AT_PRODUCT_NAME_MAX_NUM) ? AT_PRODUCT_NAME_MAX_NUM :
                                                                         g_atParaList[0].paraLen;

    (VOS_VOID)memset_s(productId.productId, sizeof(productId.productId), 0x00, sizeof(productId.productId));
    if (productIdLen > 0) {
        memResult = memcpy_s(productId.productId, sizeof(productId.productId), g_atParaList[0].para,
                             (VOS_UINT16)productIdLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(productId.productId), (VOS_UINT16)productIdLen);
    }

    productId.nvStatus = VOS_TRUE;

    /* ????NV */
    if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_PRODUCT_ID, (VOS_UINT8 *)&productId,
                           sizeof(productId.nvStatus) + sizeof(productId.productId)) != NV_OK) {
        AT_ERR_LOG("AT_SetProdNamePara:Write NV fail");
        return AT_ERROR;
    } else {
        return AT_OK;
    }
}

VOS_UINT32 At_SetDataLock(VOS_UINT8 indexNum)
{
    DRV_AGENT_DatalockSetReq datalockInfo;
    errno_t                  memResult;

    /* ?????????????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* ???????? */
    if (g_atParaIndex > 1) {
        return AT_ERROR;
    }

    /* <unlock_code>????????????8???????????? */
    if (g_atParaList[0].paraLen != TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX) {
        return AT_ERROR;
    }

    if (At_CheckNumString(g_atParaList[0].para, TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX) == AT_FAILURE) {
        return AT_ERROR;
    }

    if (g_atDataLocked == VOS_TRUE) {
        /* ?????????????? */
        datalockInfo.pwd[TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX] = '\0';
        memResult = memcpy_s(datalockInfo.pwd, sizeof(datalockInfo.pwd), g_atParaList[0].para,
                             TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(datalockInfo.pwd), TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX);

        /* ???????? C ?? ???? DATALOCK ???????? */
        if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                   DRV_AGENT_DATALOCK_SET_REQ, (VOS_UINT8 *)&datalockInfo, sizeof(datalockInfo),
                                   I0_WUEPS_PID_DRV_AGENT) == TAF_SUCCESS) {
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DATALOCK_SET; /* ???????????????? */
            (VOS_VOID)memset_s(&datalockInfo, sizeof(datalockInfo), 0x00, sizeof(datalockInfo));
            return AT_WAIT_ASYNC_RETURN; /* ???????????????? */
        } else {
            (VOS_VOID)memset_s(&datalockInfo, sizeof(datalockInfo), 0x00, sizeof(datalockInfo));
            return AT_ERROR;
        }
    }

    return AT_OK;
}

VOS_UINT32 At_SetMaxLockTimes(VOS_UINT8 indexNum)
{
    TAF_CUSTOM_SimLockMaxTimes simLockMaxTimes;

    /* ???????????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ???????????? */
    if (g_atParaIndex != 1) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ????????????????????,???????????????????? */
    if (g_atDataLocked == VOS_TRUE) {
        return AT_DATA_UNLOCK_ERROR;
    }

    simLockMaxTimes.status       = NV_ITEM_ACTIVE;
    simLockMaxTimes.lockMaxTimes = g_atParaList[0].paraValue;

    /* ??C??????????????SIMLOCK?????????? */
    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, DRV_AGENT_MAX_LOCK_TIMES_SET_REQ,
                               (VOS_UINT8 *)&simLockMaxTimes, sizeof(simLockMaxTimes),
                               I0_WUEPS_PID_DRV_AGENT) != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetMaxLockTimes():DRV_AGENT_SIMLOCK_NV_SET_REQ NV Write Fail!");
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ???????????????? */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MAXLCKTMS_SET;
    return AT_WAIT_ASYNC_RETURN; /* ???????????????????? */
}

VOS_UINT32 At_SetCallSrvPara(VOS_UINT8 indexNum)
{
    NAS_NVIM_CustomizeService custSrv;

    /* ???????????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ???????????? */
    if (g_atParaIndex != 1) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ????????????????????,???????????????????? */
    if (g_atDataLocked == VOS_TRUE) {
        return AT_DATA_UNLOCK_ERROR;
    }

    custSrv.customizeService = g_atParaList[0].paraValue;
    custSrv.status           = NV_ITEM_ACTIVE; /* NV???????????? */

    if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_CUSTOMIZE_SERVICE, (VOS_UINT8 *)&custSrv, sizeof(custSrv)) != NV_OK) {
        AT_WARN_LOG("At_SetCallSrvPara():NV_ITEM_CUSTOMIZE_SERVICE NV Write Fail!");
        return AT_DEVICE_OTHER_ERROR;
    }

    return AT_OK;
}

VOS_UINT32 AT_SetMDatePara(VOS_UINT8 indexNum)
{
    errno_t memResult;
    VOS_UINT32                 ret;
    AT_DATE                    date;
    TAF_AT_NvimManufactureDate manufactrueDate;

    if (g_atParaIndex != 1) {
        AT_NORM_LOG("AT_SetMDatePara: the number of parameters is error.");
        return AT_ERROR;
    }

    /* ????????1:?????????????? */
    ret = AT_CheckDateFormat(g_atParaList[0].para, g_atParaList[0].paraLen);
    if (ret != AT_OK) {
        AT_NORM_LOG("AT_SetMDatePara: the format of parameters is error.");
        return AT_ERROR;
    }

    /* ????????2:?????????????????????????????????? */
    ret = AT_GetDate(g_atParaList[0].para, g_atParaList[0].paraLen, &date);
    if (ret != AT_OK) {
        AT_NORM_LOG("AT_SetMDatePara: Fail to get the date.");
        return AT_ERROR;
    }

    ret = AT_CheckDate(&date);
    if (ret != AT_OK) {
        AT_NORM_LOG("AT_SetMDatePara: the value of parameters is error.");
        return AT_ERROR;
    }

    /* ????????????NV */
    memResult = memcpy_s(manufactrueDate.mDate, sizeof(manufactrueDate.mDate), g_atParaList[0].para,
                         (AT_MDATE_STRING_LENGTH - 1));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(manufactrueDate.mDate), (AT_MDATE_STRING_LENGTH - 1));
    manufactrueDate.mDate[AT_MDATE_STRING_LENGTH - 1] = '\0';
    ret = TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_AT_MANUFACTURE_DATE, manufactrueDate.mDate, AT_MDATE_STRING_LENGTH);
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_SetMDatePara: Fail to write nv.");
        return AT_ERROR;
    }

    return AT_OK;
}

VOS_UINT32 AT_SetFacInfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;
    VOS_UINT8 *facInfo = TAF_NULL_PTR;
    VOS_UINT32 offSet;
    errno_t    memResult;

    if ((g_atParaIndex != AT_FACINFO_PARA_VALID_NUM) || ((g_atParaList[AT_FACINFO_VALUE_INFO].paraLen == 0))) {
        AT_WARN_LOG("AT_SetFacInfoPara: the number of parameters is error.");
        return AT_ERROR;
    }

    /* ?????????????????????????????????????????? */
    if (g_atParaList[0].paraValue == 0) {
        if (g_atParaList[AT_FACINFO_VALUE_INFO].paraLen != AT_FACINFO_INFO1_LENGTH) {
            AT_NORM_LOG("AT_SetFacInfoPara: Length of info1 is failure.");
            return AT_ERROR;
        }

        if (At_CheckNumString(g_atParaList[AT_FACINFO_VALUE_INFO].para, AT_FACINFO_INFO1_LENGTH) == AT_FAILURE) {
            return AT_ERROR;
        }
    } else {
        if (g_atParaList[AT_FACINFO_VALUE_INFO].paraLen != AT_FACINFO_INFO2_LENGTH) {
            AT_NORM_LOG("AT_SetFacInfoPara: Length of info2 is failure.");
            return AT_ERROR;
        }

        if (At_CheckNumString(g_atParaList[AT_FACINFO_VALUE_INFO].para, AT_FACINFO_INFO2_LENGTH) == AT_FAILURE) {
            return AT_ERROR;
        }
    }

    /* ????NV???????????????????? */
    facInfo = (TAF_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, AT_FACINFO_STRING_LENGTH);
    if (facInfo == TAF_NULL_PTR) {
        AT_WARN_LOG("AT_SetFacInfoPara: fail to alloc memory.");
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(facInfo, AT_FACINFO_STRING_LENGTH, 0x00, AT_FACINFO_STRING_LENGTH);

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_AT_FACTORY_INFO, facInfo, AT_FACINFO_STRING_LENGTH);
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_SetFacInfoPara: fail to read NVIM . ");
        PS_MEM_FREE(WUEPS_PID_AT, facInfo);
        return AT_ERROR;
    }

    /*
     * ??????????NV??NV_ITEM_AT_FACTORY_INFO??????????:
     * ????????????????<INDEX>????????????,
     * <INDEX>??0????????129????????????129??????????????
     * <INDEX>??1????????129????????????129??????????????
     */
    offSet = (AT_FACINFO_INFO1_LENGTH + 1) * g_atParaList[0].paraValue;
    if ((offSet + g_atParaList[AT_FACINFO_VALUE_INFO].paraLen) >
        ((AT_FACINFO_INFO1_LENGTH + 1) + AT_FACINFO_INFO2_LENGTH)) {
        AT_WARN_LOG("AT_SetFacInfoPara: offset is error. ");
        PS_MEM_FREE(WUEPS_PID_AT, facInfo);
        return AT_ERROR;
    }

    *((facInfo + offSet) + g_atParaList[AT_FACINFO_VALUE_INFO].paraLen) = '\0';

    memResult = memcpy_s((facInfo + offSet), AT_FACINFO_STRING_LENGTH - offSet,
                         g_atParaList[AT_FACINFO_VALUE_INFO].para,
                         g_atParaList[AT_FACINFO_VALUE_INFO].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, AT_FACINFO_STRING_LENGTH - offSet, g_atParaList[AT_FACINFO_VALUE_INFO].paraLen);

    /* ??????????????????????????NV??NV_ITEM_AT_FACTORY_INFO */
    ret = TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_AT_FACTORY_INFO, facInfo, AT_FACINFO_STRING_LENGTH);
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_SetFacInfoPara: fail to write NVIM . ");
        PS_MEM_FREE(WUEPS_PID_AT, facInfo);
        return AT_ERROR;
    }

    PS_MEM_FREE(WUEPS_PID_AT, facInfo);
    return AT_OK;
}

VOS_UINT32 AT_UpdateMacPara(VOS_UINT8 aucMac[], VOS_UINT16 macLength)
{
    errno_t    memResult;
    VOS_UINT32 ret;
    VOS_UINT32 loop;
    VOS_UINT32 wifiGlobalMacOffset;
    VOS_UINT32 phyNumMacOffset;
    VOS_UINT8  wifiGlobalMac[AT_MAC_ADDR_LEN];

    /* MAC????????????: ????12?? */
    if (macLength != AT_PHYNUM_MAC_LEN) {
        return AT_PHYNUM_LENGTH_ERR;
    }

    /* MAC????????????: 7AFEE22111E4=>7A:FE:E2:21:11:E4 */
    wifiGlobalMacOffset = 0;
    phyNumMacOffset     = 0;
    for (loop = 0; loop < (AT_PHYNUM_MAC_COLON_NUM + 1); loop++) {
        memResult = memcpy_s(&wifiGlobalMac[wifiGlobalMacOffset], AT_MAC_ADDR_LEN - wifiGlobalMacOffset,
                             &aucMac[phyNumMacOffset], AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_MAC_ADDR_LEN - wifiGlobalMacOffset, AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS);
        wifiGlobalMacOffset += AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS;
        phyNumMacOffset += AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS;
        wifiGlobalMac[wifiGlobalMacOffset] = ':';
        wifiGlobalMacOffset++;
    }

    wifiGlobalMac[AT_PHYNUM_MAC_LEN + AT_PHYNUM_MAC_COLON_NUM] = '\0';

    /* ????MAC??????NV */
    ret = TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_WIFI_MAC_ADDR, wifiGlobalMac, AT_MAC_ADDR_LEN);
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_UpdateMacPara: Fail to write NV.");
        return AT_ERROR;
    }

    return AT_OK;
}

VOS_UINT32 AT_CheckPhyNumPara(AT_PhynumTypeUint32 *setType)
{
    VOS_UINT32 times;
    VOS_UINT32 ret;

    /*
     * <type>????????: BALONG??????????????????IMEI??SVN??
     * ????????????????????< err_code > ????????1????????????
     */
    /* ???????????????????? */
    if (At_UpString(g_atParaList[0].para, g_atParaList[0].paraLen) == AT_FAILURE) {
        return AT_ERROR;
    }

    if (VOS_StrCmp((VOS_CHAR *)g_atParaList[0].para, "IMEI") == 0) {
        *setType = AT_PHYNUM_TYPE_IMEI;
    } else if (VOS_StrCmp((VOS_CHAR *)g_atParaList[0].para, "SVN") == 0) {
        *setType = AT_PHYNUM_TYPE_SVN;
    } else if (VOS_StrCmp((VOS_CHAR *)g_atParaList[0].para, "MACWLAN") == 0) {
        *setType = AT_PHYNUM_TYPE_MAC;
    } else {
        AT_NORM_LOG("AT_CheckPhyNumPara: the type of physical is error.");
        return AT_PHYNUM_TYPE_ERR;
    }

    /* < times >????????: ????< times >??????0-1??????????AT_ERROR */
    if (g_atParaList[AT_PHYNUM_TIMES].paraLen != 0) {
        /* TIMES????????????0??1 */
        ret = At_Auc2ul(g_atParaList[AT_PHYNUM_TIMES].para, g_atParaList[AT_PHYNUM_TIMES].paraLen, &times);
        if ((ret != AT_SUCCESS) || (times > 1)) {
            AT_NORM_LOG("AT_CheckPhyNumPara: times error.");
            return AT_ERROR;
        }
    }

    return AT_OK;
}

VOS_UINT32 AT_UpdateSvn(ModemIdUint16 modemId, VOS_UINT8 aucSvn[], VOS_UINT16 svnLength)
{
    TAF_SVN_Data svn;

    (VOS_VOID)memset_s(&svn, sizeof(svn), 0x00, sizeof(svn));

    /* SVN ????????: ??????????SVN????????????2?? */
    if (svnLength != TAF_SVN_DATA_LENGTH) {
        AT_NORM_LOG("AT_UpdateSvn: the length of SVN is error.");
        return AT_PHYNUM_LENGTH_ERR;
    }

    /* SVN ??????????1: ????<SVN>????????????, ????ASCII????????BCD?? */
    if (At_AsciiNum2Num(svn.svn, aucSvn, svnLength) != AT_SUCCESS) {
        AT_NORM_LOG("AT_UpdateSvn: the number of SVN is error.");
        return AT_PHYNUM_NUMBER_ERR;
    }

    /*
     * SVN ??????????2: 99??SVN????????????????AT_PHYNUM_NUMBER_ERR??????????
     * ????????: 3GPP 23003 6.3 Allocation principles SVN value 99 is reserved for future use.
     */
    if ((svn.svn[0] == 9) && (svn.svn[1] == 9)) {
        AT_NORM_LOG("AT_UpdateSvn: the number of SVN is reserved.");
        return AT_PHYNUM_NUMBER_ERR;
    }

    /* ????SVN??NV?????????????????????????????????????? */
    svn.activeFlag = NV_ITEM_ACTIVE;

    /* ?????????????????? */
    svn.reserve[0] = 0;

    if (TAF_ACORE_NV_WRITE(modemId, NV_ITEM_IMEI_SVN, (VOS_UINT8 *)&svn, sizeof(svn)) != NV_OK) {
        AT_WARN_LOG("AT_UpdateSvn: Fail to write NV.");
        return AT_ERROR;
    }

    return AT_OK;
}

VOS_UINT32 AT_SetPhyNumPara(VOS_UINT8 indexNum)
{
    VOS_UINT32          ret;
    AT_PhynumTypeUint32 setType;
    VOS_BOOL            bPhyNumIsNull;
    ModemIdUint16       modemId = MODEM_ID_0;

    /*
     * ??????????????????: ????????????<type>,<number>????????
     * ??????????????< times > ????????????????????AT_ERROR
     */
    if ((g_atParaIndex != AT_PHYNUM_PARA_MIN_VALID_NUM) && (g_atParaIndex != AT_PHYNUM_PARA_MAX_VALID_NUM)) {
        AT_NORM_LOG("AT_SetPhyNumPara: the number of parameter is error.");
        return AT_ERROR;
    }

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_SetPhyNumPara: Get modem id fail.");
        return AT_ERROR;
    }

    ret = AT_CheckPhyNumPara(&setType);
    if (ret != AT_OK) {
        return ret;
    }

    /* ??????????????????????????????????????????????< err_code > ????????1?????????????? */
    if (AT_PhyNumIsNull(modemId, setType, &bPhyNumIsNull) != AT_OK) {
        return AT_ERROR;
    }

    if ((bPhyNumIsNull == VOS_FALSE) && (g_atDataLocked == VOS_TRUE)) {
        AT_NORM_LOG("AT_SetPhyNumPara: physical number is not null and data locked.");
        return AT_PHYNUM_TYPE_ERR;
    }

    /* ????????????????????????IMEI????????en_NV_Item_IMEI???????? */
    /* ????????????????????????SVN????????en_Nv_Item_Imei_Svn???????? */
    /* ????????????????????????MAC????????MAC??????????NV??(??????????MAC??????NV??)???????? */
    if (setType == AT_PHYNUM_TYPE_IMEI) {
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
        ret = AT_UpdateImei(modemId, g_atParaList[1].para, g_atParaList[1].paraLen);
#else
        ret = AT_OK;
#endif

    } else if (setType == AT_PHYNUM_TYPE_SVN) {
        ret = AT_UpdateSvn(modemId, g_atParaList[1].para, g_atParaList[1].paraLen);
    } else {
        /* ????MAC??????NV */
        ret = AT_UpdateMacPara(g_atParaList[1].para, g_atParaList[1].paraLen);
    }

    return ret;
}

VOS_UINT32 At_SetQosPara(VOS_UINT8 indexNum)
{
    AT_TrafficClassCustomize aTTrafficClass;
    ModemIdUint16            modemId = MODEM_ID_0;
    VOS_UINT32               ret;
    VOS_UINT8                qosPara;

    /* ???????????????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ?????????????? */
    if (g_atDataLocked == VOS_TRUE) {
        return AT_DATA_UNLOCK_ERROR;
    }

    /* ????????????1 */
    if (g_atParaIndex != 1) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ????????????1 */
    if (g_atParaList[0].paraLen != 1) {
        return AT_DEVICE_OTHER_ERROR;
    }

    qosPara = g_atParaList[0].para[0];

    /* ucQosPara??????????'0'~'4' */
    if ((qosPara >= '0') && (qosPara <= '4')) {
        qosPara = g_atParaList[0].para[0] - '0';
    } else {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* ??NV??NV_ITEM_TRAFFIC_CLASS_TYPE??ucStatus??????????????1 */
    aTTrafficClass.status = NV_ITEM_ACTIVE;

    /* ????????????????NV????ucTrafficClass???? */
    aTTrafficClass.trafficClass = qosPara;

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("At_SetQosPara: Get modem id fail.");
        return AT_ERROR;
    }

    if (TAF_ACORE_NV_WRITE(modemId, NV_ITEM_TRAFFIC_CLASS_TYPE, (VOS_UINT8 *)&aTTrafficClass,
                           sizeof(AT_TrafficClassCustomize)) != NV_OK) {
        AT_WARN_LOG("At_SetQosPara:WARNING:NVIM Write NV_ITEM_TRAFFIC_CLASS_TYPE failed!");
        return AT_DEVICE_OTHER_ERROR;
    }

    return AT_OK;
}

VOS_UINT32 AT_WriteWiWep(VOS_UINT32 indexNum, VOS_UINT8 aucWiWep[], VOS_UINT16 wiWepLen,
                         TAF_AT_MultiWifiSec *wifiSecInfo, VOS_UINT8 group)
{
    errno_t    memResult;
    VOS_UINT32 ret;
    VOS_UINT32 loop;
    VOS_UINT8 *wifiWepKey = VOS_NULL_PTR;

    /* ??????????en_NV_Item_WIFI_KEY????WIFI KEY????????????????????????NV???????????????????? */
    ret = TAF_ACORE_NV_READ_IN_CCORE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_KEY, (VOS_UINT8 *)wifiSecInfo,
                                     sizeof(TAF_AT_MultiWifiSec));
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_WriteWiWep: Fail to read en_NV_Item_WIFI_KEY.");
        return AT_ERROR;
    }

    /* ????index????NV???????????? */
    wifiWepKey = ((indexNum == AT_WIWEP_PARA_INDEX_VAL_0) ? wifiSecInfo->wifiWepKey1[group] :
                      ((indexNum == AT_WIWEP_PARA_INDEX_VAL_1) ? wifiSecInfo->wifiWepKey2[group] :
                          ((indexNum == AT_WIWEP_PARA_INDEX_VAL_2) ? wifiSecInfo->wifiWepKey3[group] :
                              wifiSecInfo->wifiWepKey4[group])));

    /* ????????WIFI KEY??NV???????????????? */
    for (loop = 0; loop < AT_WIFI_KEY_LEN_MAX; loop++) {
        if (wifiWepKey[loop] != aucWiWep[loop]) {
            break;
        }
    }

    /* ????????WIFI KEY??NV?????????????????????????????? */
    if (loop == AT_WIFI_KEY_LEN_MAX) {
        return AT_OK;
    }

    /* ??????????NV??en_NV_Item_WIFI_KEY */
    (VOS_VOID)memset_s(wifiWepKey, AT_WIFI_KEY_LEN_MAX, 0x00, AT_WIFI_KEY_LEN_MAX);

    if ((wiWepLen > 0) && (wiWepLen <= AT_WIFI_KEY_LEN_MAX)) {
        memResult = memcpy_s(wifiWepKey, AT_WIFI_KEY_LEN_MAX, aucWiWep, wiWepLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_WIFI_KEY_LEN_MAX, wiWepLen);
    }

    ret = TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_KEY, (VOS_UINT8 *)wifiSecInfo, sizeof(TAF_AT_MultiWifiSec));
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_WriteWiWep: Fail to write NV en_NV_Item_WIFI_KEY.");
        return AT_ERROR;
    }

    return AT_OK;
}

VOS_UINT32 AT_SetWiwepPara(VOS_UINT8 indexNum)
{
    VOS_UINT32           indexTemp;
    VOS_UINT8            group;
    VOS_UINT32           ret;
    TAF_AT_MultiWifiSec *wifiSecInfo = VOS_NULL_PTR;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ????????????: ????????< index >??< content >???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    if ((g_atParaList[AT_WIWEP_PARA_WIFI_KEY_INDEX].paraLen == 0) ||
        (g_atParaList[AT_WIWEP_PARA_WIFI_KEY_CONTENT].paraLen == 0) ||
        (g_atParaList[AT_WIWEP_SSID_GROUP_INDEX].paraLen == 0)) {
        return AT_ERROR;
    }

    /* WIFI key ??????DATALOCK???? */
    if (g_atDataLocked == VOS_TRUE) {
        return AT_ERROR;
    }

    /* < index >??????0-3????????< content >????????NV_WLKEY_LEN */
    indexTemp = g_atParaList[AT_WIWEP_PARA_WIFI_KEY_INDEX].paraValue;
    if (indexTemp > AT_WIWEP_CARD_WIFI_KEY_TOTAL) {
        return AT_ERROR;
    }

    if (g_atParaList[AT_WIWEP_PARA_WIFI_KEY_CONTENT].paraLen > AT_NV_WLKEY_LEN) {
        return AT_ERROR;
    }

    group = (VOS_UINT8)g_atParaList[AT_WIWEP_SSID_GROUP_INDEX].paraValue;

    if (group >= AT_WIFI_MAX_SSID_NUM) {
        return AT_ERROR;
    }

    /* ????WIFI KEY??NV????????????????NV??en_NV_Item_WIFI_KEY */
    /*lint -save -e830 */
    wifiSecInfo = (TAF_AT_MultiWifiSec *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(TAF_AT_MultiWifiSec));
    /*lint -restore */
    if (wifiSecInfo == VOS_NULL_PTR) {
        return AT_ERROR;
    }
    memset_s(wifiSecInfo, sizeof(TAF_AT_MultiWifiSec), 0x00, sizeof(TAF_AT_MultiWifiSec));

    ret = AT_WriteWiWep(indexTemp, g_atParaList[AT_WIWEP_PARA_WIFI_KEY_CONTENT].para,
                        g_atParaList[AT_WIWEP_PARA_WIFI_KEY_CONTENT].paraLen, wifiSecInfo, group);
    /*lint -save -e830 */
    PS_MEM_FREE(WUEPS_PID_AT, wifiSecInfo);
    /*lint -restore */
    return ret;
}

VOS_UINT32 AT_SetCmdlenPara(VOS_UINT8 indexNum)
{
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ?????????????? */
    if ((g_atParaList[AT_PC_EXPECT_TX_MSG_LEN].paraLen == 0) ||
        (g_atParaList[AT_PC_EXPECT_RX_MSG_LEN].paraLen == 0) || (g_atParaIndex != AT_CMDLEN_PARA_NUM)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_PC_EXPECT_TX_MSG_LEN].paraValue > (AT_COM_BUFF_LEN - sizeof("AT"))) {
        return AT_ERROR;
    }

    return AT_OK;
}

VOS_VOID AT_RsfwSimLockProc(VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT16 i   = 0;
    VOS_UINT32 num = 0;

    /* ????AT???????? */
    data[AT_SIMLOCK_INDEX_A]  = 'A';
    data[AT_SIMLOCK_INDEX_T]  = 'T';
    data[AT_SIMLOCK_INDEX_EQUAL_SIGN] = '='; /* ?????????????? */

    if (len < AT_SIMLOCK_MIN_LEN) {
        return;
    }

    if ((data[AT_SIMLOCK_INDEX_1] == '1') && (data[AT_SIMLOCK_INDEX_COMMA] == ',')) {
        /* ??????????????????MNC??????????0??????????2??3 */
        for (i = AT_SIMLOCK_INDEX_1; i < len; i++) {
            if (data[i] == ',') {
                num++;
                if (i + AT_SIMLOCK_UNLOCK_PASSWORD_LEN >= len) {
                    return;
                }

                /* ????????PLMN??????????????2????3 */
                if (((num % 3) == 1) && (data[i + 1] == '0') && (data[i + 2] == ',')) {
                    if (data[i + AT_SIMLOCK_UNLOCK_PASSWORD_LEN] == ',') {
                        data[i + 1] = '2';
                    } else {
                        data[i + 1] = '3';
                    }
                }
            }
        }
    }
}

VOS_UINT32 AT_SetRsfwPara(VOS_UINT8 indexNum)
{
    errno_t    memResult;
    VOS_UINT32 ret;
    VOS_UINT32 length                          = 0;
    VOS_CHAR   name[AT_RSFR_RSFW_NAME_LEN + 1] = {0};
    VOS_UINT8 *rsfw                            = NULL;

    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* ???????????????? */
    if (g_atParaIndex != AT_RSFW_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ?????????????? */
    if ((g_atParaList[AT_RSFW_ITEM_NAME].paraLen == 0) ||
        (g_atParaList[AT_RSFW_ITEM_NAME].paraLen > AT_RSFR_RSFW_NAME_LEN)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memcpy_s(name, sizeof(name), g_atParaList[AT_RSFW_ITEM_NAME].para,
                         g_atParaList[AT_RSFW_ITEM_NAME].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(name), g_atParaList[AT_RSFW_ITEM_NAME].paraLen);

    At_UpString((VOS_UINT8 *)name, g_atParaList[AT_RSFW_ITEM_NAME].paraLen);

    /* CustomInfoSetting ??????????????????<name>???????????????? */

    if (AT_STRCMP(name, "SIMLOCK") == ERR_MSP_SUCCESS) {
        if (g_atParaList[AT_RSFW_ITME].paraLen > AT_PARA_MAX_LEN) {
            PS_PRINTF_WARNING("<AT_SetRsfwPara> param 5 is too long : %d.\n", g_atParaList[AT_RSFW_ITME].paraLen);

            return AT_ERROR;
        }

        rsfw = (TAF_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, AT_RSFR_RSFW_MAX_LEN);
        if (rsfw == VOS_NULL_PTR) {
            return AT_ERROR;
        }

        (VOS_VOID)memset_s(rsfw, AT_RSFR_RSFW_MAX_LEN, 0x00, AT_RSFR_RSFW_MAX_LEN);

        /* ??????????????????AT??????????^SIMLOCK?????????????????? */
        ret = AtBase64Decode(g_atParaList[AT_RSFW_ITME].para, g_atParaList[AT_RSFW_ITME].paraLen,
                             &(rsfw[AT_CHAR_AT_LEN]), (AT_RSFR_RSFW_MAX_LEN - AT_CHAR_AT_LEN), &length);
        if (b64Result_OK != ret) {
            PS_MEM_FREE(WUEPS_PID_AT, rsfw);
            return AT_ERROR;
        }

        /* ??????CustomInfoSetting?????????????? */
        AT_RsfwSimLockProc(rsfw, (VOS_UINT16)(length + AT_CHAR_AT_LEN)); /* ????AT????????,??????????2 */

        ret = At_ProcSimLockPara(indexNum, rsfw, (VOS_UINT16)(length + AT_CHAR_AT_LEN));

        if (ret != AT_SUCCESS) {
            PS_PRINTF_WARNING("<AT_SetRsfwPara> At_ProcSimLockPara failed %d.\n", ret);
        }

        PS_MEM_FREE(WUEPS_PID_AT, rsfw);

        /* AT_ERROR AT_OK??At_ProcSimLockPara?????? */
        return AT_SUCCESS;
    } else {
        return AT_ERROR;
    }
}

VOS_VOID AT_InitTmpBase64DecodeArray(unsigned char *base64Decode, VOS_UINT32 maxBaseLen)
{
    static VOS_BOOL      initialized = VOS_FALSE;
    unsigned int         i;
    const unsigned char  decodeDataMax     =  AT_BASE_64_CODING_CHAR_MAX_NUM - 1;
    const unsigned char  decodePadding     = decodeDataMax + 2; /* ??????????????????????2??"=" */
    const unsigned char  decodeError       = decodeDataMax + 3; /* ????????2??"="?? ??????3?? */

    if (!initialized) {
        for (i = 0; i < maxBaseLen; ++i) {
            base64Decode[i] = decodeError;
        }
        for (i = 'A'; i <= 'Z'; ++i) {
            base64Decode[i] = (unsigned char)(i - 'A');
        }
        for (i = 'a'; i <= 'z'; ++i) {
            base64Decode[i] = (unsigned char)(i - 'a' + AT_LETTERS_NUM);
        }
        for (i = '0'; i <= '9'; ++i) {
            base64Decode[i] = (unsigned char)(i - '0' + AT_LETTERS_NUM * AT_DOUBLE_LENGTH);
        }
        base64Decode['+'] = AT_LETTERS_NUM * AT_DOUBLE_LENGTH + AT_DECIMAL_BASE_NUM;
        base64Decode['/'] = AT_LETTERS_NUM * AT_DOUBLE_LENGTH + AT_DECIMAL_BASE_NUM + 1;
        base64Decode['='] = decodePadding;
        initialized       = VOS_TRUE;
    }
}

VOS_UINT32 AT_CheckBase64CodeSize(const unsigned int codeSize, unsigned int *codeNode, VOS_UINT32 *b64result)
{
    /* ????base64??????????????????4???????????????????? */
    *codeNode            = codeSize / 4;
    if ((*codeNode * 4) != codeSize) {
        *b64result = b64Result_CODE_SIZE_ERROR;
        return VOS_FALSE;
    } else if (*codeNode == 0) {
        *b64result = b64Result_OK;
        return VOS_FALSE;
    } else {
        return VOS_TRUE;
    }
}

VOS_UINT32 AT_JudgeInputCodePaddingValid(const unsigned char *input, const unsigned int codeSize,
    unsigned int *outputSize, const unsigned int dataSize, VOS_UINT32 *b64result)
{
    /* ??????????????????????????????????"=" */
    if (input[codeSize - 2] == AT_BASE64_PADDING) {
        if (input[codeSize - 1] != AT_BASE64_PADDING) {
            *b64result = b64Result_CODE_ERROR;
            return VOS_FALSE;
        }
        /* ?????????????????????? */
        (*outputSize) -= 2;
    } else if (input[codeSize - 1] == AT_BASE64_PADDING) {
        --(*outputSize);
    } else {
    }

    if (*outputSize > dataSize) {
        *b64result = b64Result_DATA_SIZE_SMALLNESS;
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*
 * ????????: ????base64????????3????????????????
 */
VOS_UINT32 AtBase64DecodeBordWidthProc(unsigned int bordWidth, const unsigned char *input,
    const unsigned char *base64Decode, unsigned char *output)
{
    const unsigned char  decodeDataMax = AT_BASE_64_CODING_CHAR_MAX_NUM - 1;
    unsigned int         code0;
    unsigned int         code1;
    unsigned int         code2;

    if (bordWidth == AT_BORD_MIN_WIDTH) {
        code0 = base64Decode[input[AT_INPUT_INDEX_0]];
        code1 = base64Decode[input[AT_INPUT_INDEX_1]];
        if ((code0 | code1) <= decodeDataMax) {
            output[AT_OUTPUT_INDEX_0] = (unsigned char)((code0 << 2) + (code1 >> 4));
        } else {
            return VOS_FALSE;
        }
    } else if (bordWidth == AT_BORD_MAX_WIDTH) {
        code0 = base64Decode[input[AT_INPUT_INDEX_0]];
        code1 = base64Decode[input[AT_INPUT_INDEX_1]];
        code2 = base64Decode[input[AT_INPUT_INDEX_2]];
        if ((code0 | code1 | code2) <= decodeDataMax) {
            output[AT_OUTPUT_INDEX_0] = (unsigned char)((code0 << 2) + (code1 >> 4));
            output[AT_OUTPUT_INDEX_1] = (unsigned char)((code1 << 4) + (code2 >> 2));
        } else {
            return VOS_FALSE;
        }
    } else {
    }

    return VOS_TRUE;
}

unsigned int AtBase64Decode(const VOS_UINT8 *pcode, const unsigned int codeSize, VOS_UINT8 *outPdata,
                            const unsigned int dataSize, unsigned int *outPwritedDataSize)
{
    const unsigned char *inputLastFastNode = VOS_NULL_PTR;
    unsigned char       *output            = VOS_NULL_PTR;
    const unsigned char *input             = VOS_NULL_PTR;
    const unsigned char  decodeDataMax     =  AT_BASE_64_CODING_CHAR_MAX_NUM - 1;
    static unsigned char base64Decode[AT_BASE64_DECODE_ARRAY_MAX_LEN];
    unsigned int         code0;
    unsigned int         code1;
    unsigned int         code2;
    unsigned int         code3;
    unsigned int         bordWidth;
    unsigned int         outputSize;
    unsigned int         codeNode = 0;
    VOS_UINT32           b64result = b64Result_OK;

    AT_InitTmpBase64DecodeArray(&base64Decode[0], AT_BASE64_DECODE_ARRAY_MAX_LEN);

    *outPwritedDataSize = 0;

    if (AT_CheckBase64CodeSize(codeSize, &codeNode, &b64result) != VOS_TRUE) {
        return b64result;
    }

    /* code_node>0 */
    input      = (const unsigned char *)pcode;
    output     = (unsigned char *)outPdata;
    /* ????base64????????3???????????? */
    outputSize = codeNode * 3;

    if (AT_JudgeInputCodePaddingValid(input, codeSize, &outputSize, dataSize, &b64result) != VOS_TRUE) {
        return b64result;
    }

    inputLastFastNode = &input[outputSize / 3UL * 4UL];

    /* input??????????????output????????4?? */
    for (; input < inputLastFastNode; input += 4, output += 3) {
        code0 = base64Decode[input[AT_INPUT_INDEX_0]];
        code1 = base64Decode[input[AT_INPUT_INDEX_1]];
        code2 = base64Decode[input[AT_INPUT_INDEX_2]];
        code3 = base64Decode[input[AT_INPUT_INDEX_3]];
        if (((code0 | code1) | (code2 | code3)) <= decodeDataMax) {
            output[AT_OUTPUT_INDEX_0] = (unsigned char)((code0 << 2) + (code1 >> 4));
            output[AT_OUTPUT_INDEX_1] = (unsigned char)((code1 << 4) + (code2 >> 2));
            output[AT_OUTPUT_INDEX_2] = (unsigned char)((code2 << 6) + code3);
        } else {
            return b64Result_CODE_ERROR;
        }
    }
    /* ????base64????????3???????????? */
    bordWidth = outputSize % 3;
    if (AtBase64DecodeBordWidthProc(bordWidth, input, base64Decode, output) != VOS_TRUE) {
        return b64Result_CODE_ERROR;
    }

    *outPwritedDataSize = outputSize;
    return b64Result_OK;
}

/*
 * Description: ^ICCID
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_SetIccidPara(TAF_UINT8 indexNum)
{
    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ????????AT_OK */
    return AT_OK;
}

VOS_UINT32 atSetFLNAPara(VOS_UINT8 clientId)
{
    FTM_SetAagcReq fLNASetReq = {0};
    VOS_UINT32     rst;

    /* ???????? */

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    fLNASetReq.aggcLvl = (VOS_UINT8)(g_atParaList[0].paraValue);

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_SET_AAGC_REQ, clientId, (VOS_UINT8 *)(&fLNASetReq),
                           sizeof(fLNASetReq));

    if (rst == AT_SUCCESS) {
        /* ???????????????? */

        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FLNA_SET;
        return AT_WAIT_ASYNC_RETURN; /* ???????????????????? */
    }

    return AT_ERROR;
}

#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetUartTest(VOS_UINT8 indexId)
{
    VOS_UINT32 result;

    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        AT_WARN_LOG("AT_SetUartTest: Incorrect Cmd Type.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ??????????running?????????????????????????????????????? */
    if (AT_GetUartTestState() == AT_UART_TEST_RUNNING) {
        AT_WARN_LOG("AT_SetUartTest: is testing!");
        return AT_ERROR;
    }

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexId].clientId, 0, ID_AT_MTA_UART_TEST_REQ, VOS_NULL, 0,
                                    I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexId].cmdCurrentOpt = AT_CMD_UART_TEST_SET;
        AT_SetUartTestState(AT_UART_TEST_RUNNING);
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_WARN_LOG("AT_SetUartTest: Send msg fail!");
        return AT_ERROR;
    }
}

VOS_UINT32 AT_SetI2sTest(VOS_UINT8 indexId)
{
    VOS_UINT32 rst;

    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ???????? */
    if (AT_GetI2sTestState() == AT_I2S_TEST_RUNNING) {
        AT_WARN_LOG("AT_SetI2sTest: is testing!");
        return AT_ERROR;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexId].clientId, g_atClientTab[indexId].opId, APP_VC_MSG_I2S_TEST_REQ,
                                 VOS_NULL_PTR, 0, I0_WUEPS_PID_VC);

    if (rst == TAF_SUCCESS) {
        /* ???????????????? */
        g_atClientTab[indexId].cmdCurrentOpt = AT_CMD_I2S_TEST_SET;
        AT_SetI2sTestState(AT_I2S_TEST_RUNNING);
        return AT_WAIT_ASYNC_RETURN; /* ???????????????????? */
    } else {
        return AT_ERROR;
    }
}
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
VOS_UINT32 AT_SetFDac(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    TAF_UINT16         dac;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ???? LTE ???????????? */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return AT_CMD_NOT_SUPPORT;
    }
#endif
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return AT_CMD_NOT_SUPPORT;
    }
    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FDAC_SET_FAIL;
    }
    /* ?????????????? */
    if (g_atParaIndex != 1) {
        return AT_FDAC_SET_FAIL;
    }
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    if (atDevCmdCtrl->dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    dac = (VOS_UINT16)g_atParaList[0].paraValue;

    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_AWS)) {
        if (dac > WDSP_MAX_TX_AGC) {
            return AT_FDAC_SET_FAIL;
        } else {
            atDevCmdCtrl->fdac = (VOS_UINT16)g_atParaList[0].paraValue;
        }
    } else {
        if (dac > GDSP_MAX_TX_VPA) {
            return AT_FDAC_SET_FAIL;
        } else {
            atDevCmdCtrl->fdac = (VOS_UINT16)g_atParaList[0].paraValue;
        }
    }

    /* AT^FDAC??????FDAC????AT^FWAVE??????power?????????????????????????????? */
    atDevCmdCtrl->fdacFlag  = VOS_TRUE;
    atDevCmdCtrl->powerFlag = VOS_FALSE;

    return AT_OK; /* ???????????????????? */
}

VOS_UINT32 At_SetFpaPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ????LTE ???????????? */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return AT_CMD_NOT_SUPPORT;
    }
#endif
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return AT_CMD_NOT_SUPPORT;
    }
    /* ???????????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FPA_OTHER_ERR;
    }

    /* ???????? */
    if (g_atParaIndex > 1) {
        return AT_FPA_OTHER_ERR;
    }

    /* ?????????????????????????? */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ??????????????????????????????,??^FCHAN?????????? */
    if (atDevCmdCtrl->dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    /* ???????????????????? */
    atDevCmdCtrl->paLevel = (VOS_UINT8)g_atParaList[0].paraValue;

    return AT_OK;
}

VOS_UINT32 AT_SetFAgcgainPara(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* ???? LTE ???????????? */
#if (FEATURE_LTE == FEATURE_ON)
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_FDD_LTE) ||
#if (FEATURE_LTEV == FEATURE_ON)
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_LTEV) ||
#endif
        (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDD_LTE)) {
        return atSetFLNAPara(indexNum);
    }
#endif

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    if (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_TDSCDMA) {
        return atSetFLNAPara(indexNum);
    }
#endif

    /* ???????????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FLNA_OTHER_ERR;
    }

    /* ???????? */
    if (g_atParaIndex > 1) {
        return AT_FLNA_OTHER_ERR;
    }

    /* ?????????????????????????? */
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ?????????????????????????????? */
    if (atDevCmdCtrl->dspLoadFlag == VOS_FALSE) {
        return AT_CHANNEL_NOT_SET;
    }

    /* ????LNA???????????????? */
    if ((atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_WCDMA) || (atDevCmdCtrl->deviceRatMode == AT_RAT_MODE_AWS)) {
        /* WDSP LNA??????????0-2 */
        if (g_atParaList[0].paraValue > DSP_LNA_HIGH_GAIN_MODE) {
            return AT_FLNA_OTHER_ERR;
        }
    }

    /* ???????????????????? */
    atDevCmdCtrl->lnaLevel = (VOS_UINT8)g_atParaList[0].paraValue;

    return AT_OK;
}
#else

VOS_UINT32 At_CovertAtPaLevelToBbicCal(AT_CmdPalevelUint8 atPaLevel, BBIC_CAL_PA_MODE_ENUM_UINT8 *bbicPaLevel)
{
    VOS_UINT32 result = VOS_TRUE;

    switch (atPaLevel) {
        case AT_CMD_PALEVEL_HIGH:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_HIGH;
            break;

        case AT_CMD_PALEVEL_MID:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_MID;
            break;

        case AT_CMD_PALEVEL_LOW:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_LOW;
            break;

        case AT_CMD_PALEVEL_ULTRALOW:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_ULTRA_LOW;
            break;

        default:
            *bbicPaLevel = BBIC_CAL_PA_GAIN_MODE_BUTT;
            result       = VOS_FALSE;
            break;
    }

    return result;
}

VOS_UINT32 At_SetFpaPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_SetFpaPara Enter");

    /* ???????????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_FPA_OTHER_ERR;
    }

    /* ???????? */
    if (g_atParaIndex > 1) {
        return AT_FPA_OTHER_ERR;
    }

    /* ?????????????????????????? */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ??????????????????????????????,??^FCHAN?????????? */
    if (atMtInfoCtx->atInfo.dspLoadFlag == VOS_FALSE) {
        return AT_NOT_LOAD_DSP;
    }

    if (At_CovertAtPaLevelToBbicCal((AT_CmdPalevelUint8)g_atParaList[AT_FPA_LEVEL].paraValue,
                                    &atMtInfoCtx->bbicInfo.paLevel) == VOS_FALSE) {
        return AT_FPA_LEVEL_ERR;
    }
    /* ???????????????????? */
    atMtInfoCtx->atInfo.paLevel = (AT_CmdPalevelUint8)g_atParaList[AT_FPA_LEVEL].paraValue;

    return AT_OK;
}

VOS_UINT32 AT_SetFAgcgainPara(VOS_UINT8 indexNum)
{
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("AT_SetFAgcgainPara Enter");

    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  ?????????????????????????? */
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM) {
        return AT_DEVICE_MODE_ERROR;
    }

    /* ?????????????????????????????? */
    if (atMtInfoCtx->atInfo.dspLoadFlag == VOS_FALSE) {
        return AT_NOT_LOAD_DSP;
    }

    /* ???????? */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atMtInfoCtx->atInfo.agcGainLevel = (VOS_UINT8)g_atParaList[0].paraValue;

    return AT_OK;
}

#endif

VOS_UINT32 AT_SetWebPwdPara(VOS_UINT8 indexNum)
{
    VOS_CHAR                    webPwdTmp[AT_WEBUI_PWD_MAX + 1];
    TAF_AT_NvimWebAdminPassword webPwd;
    errno_t                     memResult;

    (VOS_VOID)memset_s(&webPwd, sizeof(webPwd), 0x00, sizeof(webPwd));

    /* ???????? */
    if (g_atParaIndex > AT_WUPWD_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    /* ???????????? */
    if (g_atParaList[1].paraLen > AT_WEBUI_PWD_MAX) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    /* ????WEBUI PWD */
    if (g_atParaList[0].paraValue == AT_WEBUI_PWD_VERIFY) {
        if (TAF_ACORE_NV_READ_IN_CCORE(MODEM_ID_0, NV_ITEM_WEB_ADMIN_PASSWORD, (VOS_UINT8 *)&webPwd, sizeof(webPwd)) !=
            VOS_OK) {
            AT_WARN_LOG("AT_SetWebPwdPara:READ NV ERROR");
            return AT_ERROR;
        }

        memResult = memcpy_s(webPwdTmp, sizeof(webPwdTmp), &webPwd, sizeof(webPwd));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(webPwdTmp), sizeof(webPwd));

        webPwdTmp[AT_WEBUI_PWD_MAX] = '\0';

        /* ?????????? */
        if (VOS_StrCmp(webPwdTmp, (VOS_CHAR *)g_atParaList[1].para) != 0) {
            (VOS_VOID)memset_s(webPwdTmp, sizeof(webPwdTmp), 0x00, sizeof(webPwdTmp));
            (VOS_VOID)memset_s(&webPwd, sizeof(webPwd), 0x00, sizeof(webPwd));
            (VOS_VOID)memset_s(g_atParaList[1].para, sizeof(g_atParaList[1].para), 0x00, sizeof(g_atParaList[1].para));
            return AT_ERROR;
        }
    } else {
        /* ????WEBPWD??????NV?? */
        if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_WEB_ADMIN_PASSWORD, g_atParaList[1].para, AT_WEBUI_PWD_MAX) !=
            VOS_OK) {
            AT_WARN_LOG("AT_SetWebPwdPara:WRITE NV ERROR");
            (VOS_VOID)memset_s(g_atParaList[1].para, sizeof(g_atParaList[1].para), 0x00, sizeof(g_atParaList[1].para));
            return AT_ERROR;
        }
    }

    (VOS_VOID)memset_s(webPwdTmp, sizeof(webPwdTmp), 0x00, sizeof(webPwdTmp));
    (VOS_VOID)memset_s(&webPwd, sizeof(webPwd), 0x00, sizeof(webPwd));
    (VOS_VOID)memset_s(g_atParaList[1].para, sizeof(g_atParaList[1].para), 0x00, sizeof(g_atParaList[1].para));
    return AT_OK;
}

#if (FEATURE_LTE == FEATURE_ON)
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_SetSfm(VOS_UINT8 indexNum)
{
    OM_SW_VerFlag customVersion;

    /* ???????? */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* ???????? */
    if (g_atParaIndex > 1) {
        return AT_ERROR;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_ERROR;
    }

    /* AT??????NV???????????????? */
    if (g_atParaList[0].paraValue == 0) {
        customVersion.nvSwVerFlag = 1;
    } else {
        customVersion.nvSwVerFlag = 0;
    }

    /* ?????????????????? */
    if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_SW_VERSION_FLAG, (VOS_UINT8 *)&customVersion.nvSwVerFlag,
                           sizeof(customVersion.nvSwVerFlag)) != VOS_OK) {
        return AT_ERROR;
    } else {
        return AT_OK;
    }
}
#endif
#endif

VOS_VOID AT_StopSimlockDataWriteTimer(VOS_UINT8 indexNum)
{
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx = VOS_NULL_PTR;
    VOS_UINT32                          timerName;
    VOS_UINT32                          tempIndex;

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();

    tempIndex = (VOS_UINT32)indexNum;
    timerName = AT_SIMLOCKWRITEEX_TIMER;
    timerName |= AT_INTERNAL_PROCESS_TYPE;
    timerName |= (tempIndex << 12);

    if (simlockWriteExCtx != VOS_NULL_PTR) {
        (VOS_VOID)AT_StopRelTimer(timerName, &(simlockWriteExCtx->hSimLockWriteExProtectTimer));
    }
}

VOS_UINT32 AT_ProcSimlockWriteExData(VOS_UINT8 *simLockData, VOS_UINT16 paraLen)
{
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx = VOS_NULL_PTR;
    VOS_UINT8                          *tempData          = VOS_NULL_PTR;
    errno_t                             memResult;
    VOS_UINT16                          totalLen;

    if ((simLockData == VOS_NULL_PTR) || (paraLen == 0)) {
        AT_ERR_LOG("AT_ProcSimlockWriteExData: NULL Pointer");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();

    /* ??????????????????????????????????????AT???? */
    if (simlockWriteExCtx->data == VOS_NULL_PTR) {
        /*lint -save -e830*/
        simlockWriteExCtx->data = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, (VOS_UINT32)paraLen);
        /*lint -restore */
        /* ?????????????????????? */
        if (simlockWriteExCtx->data == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_ProcSimlockWriteExData: first data, Alloc mem fail");

            return AT_CME_MEMORY_FAILURE;
        }
        (VOS_VOID)memset_s(simlockWriteExCtx->data, paraLen, 0x00, paraLen);

        memResult = memcpy_s(simlockWriteExCtx->data, paraLen, simLockData, paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, paraLen, paraLen);

        simlockWriteExCtx->simlockDataLen = paraLen;
    } else {
        /* ??????????????????AT?????????????????? */
        totalLen = paraLen + simlockWriteExCtx->simlockDataLen;
        /*lint -save -e516*/
        tempData = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, totalLen);
        /*lint -restore */
        /* ?????????????????????? */
        if (tempData == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_ProcSimlockWriteExData: Non-first data, Alloc mem fail");

            return AT_CME_MEMORY_FAILURE;
        }
        (VOS_VOID)memset_s(tempData, totalLen, 0x00, totalLen);

        if (simlockWriteExCtx->simlockDataLen > 0) {
            memResult = memcpy_s(tempData, totalLen, simlockWriteExCtx->data, simlockWriteExCtx->simlockDataLen);
            TAF_MEM_CHK_RTN_VAL(memResult, totalLen, simlockWriteExCtx->simlockDataLen);
        }

        memResult = memcpy_s((tempData + simlockWriteExCtx->simlockDataLen),
                             (totalLen - simlockWriteExCtx->simlockDataLen), simLockData, paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, (totalLen - simlockWriteExCtx->simlockDataLen), paraLen);

        /*lint -save -e830*/
        PS_MEM_FREE(WUEPS_PID_AT, simlockWriteExCtx->data);
        simlockWriteExCtx->data = VOS_NULL_PTR;
        /*lint -restore */
        simlockWriteExCtx->simlockDataLen = totalLen;
        simlockWriteExCtx->data           = tempData;
    }

    return AT_SUCCESS;
}

LOCAL VOS_UINT32 AT_CheckSaveSimLockDataInSettingProcess(AT_SimlockTypeUint8 netWorkFlg, VOS_UINT8 indexNum,
    AT_SimlockWriteExPara *simlockWriteExPara)
{
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx = VOS_NULL_PTR;

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();
    /* ?????????????????? */
    if (netWorkFlg != simlockWriteExCtx->netWorkFlg) {
        AT_WARN_LOG2("AT_CheckSaveSimLockDataInSettingProcess: ucNetWorkFlg error, PreNetWorkFlg %d, CurNetWorkFlg %d",
                     netWorkFlg, simlockWriteExCtx->netWorkFlg);

        return VOS_FALSE;
    }

    /* ???????????????????????? */
    if (indexNum != simlockWriteExCtx->clientId) {
        AT_WARN_LOG2("AT_CheckSaveSimLockDataInSettingProcess: port error, ucIndex %d, ucClientId %d", indexNum,
                     simlockWriteExCtx->clientId);

        return VOS_FALSE;
    }

    /* ????????????????????????????Layer????????????????Layer???? */
    if ((VOS_UINT8)simlockWriteExPara->layer != simlockWriteExCtx->layer) {
        AT_WARN_LOG2("AT_CheckSaveSimLockDataInSettingProcess: Layer %d wrong, %d", simlockWriteExPara->layer,
                        simlockWriteExCtx->layer);

        return VOS_FALSE;
    }

    /* ????????????????????????????total????????????????total???? */
    if ((VOS_UINT8)simlockWriteExPara->total != simlockWriteExCtx->totalNum) {
        AT_WARN_LOG2("AT_CheckSaveSimLockDataInSettingProcess: total %d wrong, %d", simlockWriteExPara->total,
                        simlockWriteExCtx->totalNum);
        return VOS_FALSE;
    }

    /* ??????????Index????????????Index+1 */
    if ((VOS_UINT8)simlockWriteExPara->index != (simlockWriteExCtx->curIdx + 1)) {
        AT_WARN_LOG2("AT_CheckSaveSimLockDataInSettingProcess: CurIndex %d wrong, %d", simlockWriteExPara->index,
                        simlockWriteExCtx->curIdx);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

VOS_UINT32 AT_SaveSimlockDataIntoCtx(AT_SimlockWriteExPara *simlockWriteExPara, VOS_UINT8 indexNum,
                                     AT_SimlockTypeUint8 netWorkFlg)
{
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx = VOS_NULL_PTR;
    VOS_UINT8                          *simLockData       = VOS_NULL_PTR;
    VOS_UINT8                          *hmac              = VOS_NULL_PTR;
    VOS_UINT32                          result;
    errno_t                             memResult;
    VOS_UINT16                          hmacLen;
    VOS_UINT8                           paraNum;

    /* ?????????? */
    hmacLen        = simlockWriteExPara->hmacLen;
    paraNum        = simlockWriteExPara->paraNum;
    simLockData    = simlockWriteExPara->simLockData;
    hmac           = simlockWriteExPara->hmac;

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();

    /* ???????????????????????????????????? */
    if (simlockWriteExCtx->settingFlag == VOS_FALSE) {
        if (simlockWriteExPara->index != 1) {
            AT_WARN_LOG1("AT_SaveSimlockDataIntoCtx: Invalid ulCurrIndex", simlockWriteExPara->index);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* ?????????????????????????????? */
        result = AT_ProcSimlockWriteExData(simLockData, simlockWriteExPara->simLockDataLen);

        if (result != AT_SUCCESS) {
            AT_WARN_LOG1("AT_SaveSimlockDataIntoCtx: AT_ProcSimlockWriteExData fail %d", result);

            return result;
        }

        simlockWriteExCtx->netWorkFlg  = netWorkFlg;
        simlockWriteExCtx->clientId    = indexNum;
        simlockWriteExCtx->totalNum    = (VOS_UINT8)simlockWriteExPara->total;
        simlockWriteExCtx->curIdx      = (VOS_UINT8)simlockWriteExPara->index;
        simlockWriteExCtx->layer       = (VOS_UINT8)simlockWriteExPara->layer;
        simlockWriteExCtx->settingFlag = VOS_TRUE;
    } else {
        if (AT_CheckSaveSimLockDataInSettingProcess(netWorkFlg, indexNum, simlockWriteExPara) != VOS_TRUE) {
            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* ?????????????????????? */
        result = AT_ProcSimlockWriteExData(simLockData, simlockWriteExPara->simLockDataLen);

        if (result != AT_SUCCESS) {
            AT_WARN_LOG1("AT_SaveSimlockDataIntoCtx: AT_ProcSimlockWriteExData fail %d", result);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return result;
        }

        /* ????CurrIndex */
        simlockWriteExCtx->curIdx = (VOS_UINT8)simlockWriteExPara->index;
    }

    /* ??????????????5??????5??????copy??????????????????????????HMAC???????????????? */
    if ((paraNum == 5) && (hmacLen == AT_SET_SIMLOCK_DATA_HMAC_LEN) && hmac != VOS_NULL_PTR) {
        memResult = memcpy_s(simlockWriteExCtx->hmac, AT_SET_SIMLOCK_DATA_HMAC_LEN, hmac, hmacLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_SET_SIMLOCK_DATA_HMAC_LEN, hmacLen);
        simlockWriteExCtx->hmacLen = (VOS_UINT8)hmacLen;
    }

    return AT_OK;
}

VOS_UINT32 AT_CheckSimlockDataWriteExPara(AT_SimlockWriteExPara *simlockWriteExPara)
{
    /* ??????layer??????0,1,2,3,4,255 */
    if ((simlockWriteExPara->layer > AT_SIMLOCKDATAWRITEEX_LAYER_FOUR_VALID_VALUE) &&
    (simlockWriteExPara->layer != AT_SIMLOCKDATAWRITEEX_LAYER_MAX_VALID_VALUE)) {
        AT_WARN_LOG1("AT_CheckSimlockDataWriteExPara: invalid layer value:", simlockWriteExPara->layer);
        return VOS_ERR;
    }

    /* ??????ulIndex??????1-255 */
    if ((simlockWriteExPara->index == 0) ||
        (simlockWriteExPara->index > AT_SIMLOCKDATAWRITEEX_ULINDEX_MAX_VALID_VALUE)) {
        AT_WARN_LOG1("AT_CheckSimlockDataWriteExPara: invalid ulIndex value:", simlockWriteExPara->index);
        return VOS_ERR;
    }

    /* ??????ulTotal??????1-255 */
    if ((simlockWriteExPara->total == 0) ||
        (simlockWriteExPara->total > AT_SIMLOCKDATAWRITEEX_ULTOTAL_MAX_VALID_VALUE)) {
        AT_WARN_LOG1("AT_CheckSimlockDataWriteExPara: invalid ulTotal value:", simlockWriteExPara->total);
        return VOS_ERR;
    }

    /* Index??????total */
    if (simlockWriteExPara->index > simlockWriteExPara->total) {
        AT_WARN_LOG2("AT_CheckSimlockDataWriteExPara: Index bigger than total", simlockWriteExPara->index,
                     simlockWriteExPara->total);

        return VOS_ERR;
    }

    /* ??????????????simlockdata??????????1400?????? */
    if (simlockWriteExPara->simLockDataLen > AT_SIMLOCKDATA_PER_WRITE_MAX_LEN) {
        AT_WARN_LOG1("AT_CheckSimlockDataWriteExPara: SimLockData is too long:", simlockWriteExPara->simLockDataLen);
        return VOS_ERR;
    }

    return VOS_OK;
}

LOCAL VOS_UINT32 AT_RestartSimLockDataWriteTimer(AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx,
    VOS_UINT8 indexNum)
{
    VOS_UINT32                          timerName;
    VOS_UINT32                          tempIndex;

    /* ???????????????????????????????? */
    AT_StopSimlockDataWriteTimer(indexNum);

    tempIndex = (VOS_UINT32)indexNum;
    timerName = AT_SIMLOCKWRITEEX_TIMER;
    timerName |= AT_INTERNAL_PROCESS_TYPE;
    timerName |= (tempIndex << 12);

    (VOS_VOID)AT_StartRelTimer(&(simlockWriteExCtx->hSimLockWriteExProtectTimer),
                               AT_SIMLOCK_WRITE_EX_PROTECT_TIMER_LEN, timerName, 0, VOS_RELTIMER_NOLOOP);

    return AT_OK;
}
LOCAL VOS_UINT32 AT_SendDrvAgentSimLockWrExReq(VOS_UINT8 indexNum)
{
    DRV_AGENT_SimlockwriteexSetReq     *simlockWriteExSetReq = VOS_NULL_PTR;
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx    = VOS_NULL_PTR;
    VOS_UINT32                          result;
    VOS_UINT32                          length;
    errno_t                             memResult;

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();

    if (simlockWriteExCtx->simlockDataLen > AT_SIMLOCK_DATA_LEN) {
        length = sizeof(DRV_AGENT_SimlockwriteexSetReq) + simlockWriteExCtx->simlockDataLen - AT_SIMLOCK_DATA_LEN;
    }else {
        length = sizeof(DRV_AGENT_SimlockwriteexSetReq);
    }
    simlockWriteExSetReq = (DRV_AGENT_SimlockwriteexSetReq *)PS_MEM_ALLOC(WUEPS_PID_AT, length);

    if (simlockWriteExSetReq == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_SetSimlockDataWriteExPara: alloc mem fail.");

        AT_ClearSimLockWriteExCtx();
        AT_StopSimlockDataWriteTimer(indexNum);

        return AT_CME_MEMORY_FAILURE;
    }
    (VOS_VOID)memset_s(simlockWriteExSetReq, length, 0x00, length);

    simlockWriteExSetReq->hmacLen = simlockWriteExCtx->hmacLen;
    if (simlockWriteExCtx->hmacLen > 0) {
        memResult = memcpy_s(simlockWriteExSetReq->hmac, sizeof(simlockWriteExSetReq->hmac),
                             simlockWriteExCtx->hmac, simlockWriteExCtx->hmacLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(simlockWriteExSetReq->hmac), simlockWriteExCtx->hmacLen);
    }

    simlockWriteExSetReq->simlockDataLen = simlockWriteExCtx->simlockDataLen;
    if (simlockWriteExCtx->simlockDataLen > 0) {
        memResult = memcpy_s(simlockWriteExSetReq->simlockData, simlockWriteExCtx->simlockDataLen,
                             simlockWriteExCtx->data, simlockWriteExCtx->simlockDataLen);
        TAF_MEM_CHK_RTN_VAL(memResult, simlockWriteExCtx->simlockDataLen, simlockWriteExCtx->simlockDataLen);
    }

    /* ??????????????????????????????????????index */
    simlockWriteExSetReq->total = simlockWriteExCtx->totalNum;

    /* ???????????????????????? */
    simlockWriteExSetReq->netWorkFlg = simlockWriteExCtx->netWorkFlg;
    simlockWriteExSetReq->layer      = simlockWriteExCtx->layer;

    AT_ClearSimLockWriteExCtx();
    AT_StopSimlockDataWriteTimer(indexNum);

    /* ????????, ??????????????C??, ???????????? */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    DRV_AGENT_SIMLOCKWRITEEX_SET_REQ, (VOS_UINT8 *)simlockWriteExSetReq, length,
                                    I0_WUEPS_PID_DRV_AGENT);
    /*lint -save -e516*/
    PS_MEM_FREE(WUEPS_PID_AT, simlockWriteExSetReq);
    /*lint -restore */
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetSimlockDataWriteExPara: AT_FillAndSndAppReqMsg fail.");

        return AT_ERROR;
    }

    return AT_OK;
}

VOS_UINT32 AT_SetSimlockDataWriteExPara(AT_SimlockWriteExPara *simlockWriteExPara, VOS_UINT8 indexNum,
                                        AT_SimlockTypeUint8 netWorkFlg)
{
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx    = VOS_NULL_PTR;
    VOS_UINT32                          result;
    VOS_UINT16                          hmacLen;

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();

    if (AT_CheckSimlockDataWriteExPara(simlockWriteExPara) != VOS_OK) {
        if (simlockWriteExCtx->settingFlag == VOS_FALSE) {
            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);
        }

        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (simlockWriteExCtx->totalNum > AT_SIM_LOCK_DATA_WRITEEX_MAX_TOTAL) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ????????????????????  */
    result = AT_SaveSimlockDataIntoCtx(simlockWriteExPara, indexNum, netWorkFlg);

    if (result != AT_OK) {
        return result;
    }

    /* ????????????????????????????????????OK */
    if (simlockWriteExCtx->curIdx < simlockWriteExCtx->totalNum) {
        return AT_RestartSimLockDataWriteTimer(simlockWriteExCtx, indexNum);
    } else {
        /* ??????????????????Simlock_Data???????? */
        result = At_AsciiNum2HexString(simlockWriteExCtx->data, &(simlockWriteExCtx->simlockDataLen));
        if (result != AT_SUCCESS) {
            AT_WARN_LOG2("AT_SetSimlockDataWriteExPara: At_AsciiNum2HexString fail ulResult: %d ulParaLen: %d", result,
                         simlockWriteExCtx->simlockDataLen);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* ??????????????????HMAC???????? */
        hmacLen = simlockWriteExCtx->hmacLen;
        result  = At_AsciiNum2HexString(simlockWriteExCtx->hmac, &hmacLen);

        simlockWriteExCtx->hmacLen = (VOS_UINT8)hmacLen;

        if ((result != AT_SUCCESS) || (simlockWriteExCtx->hmacLen != DRV_AGENT_HMAC_DATA_LEN)) {
            AT_WARN_LOG2("AT_SetSimlockDataWriteExPara: At_AsciiNum2HexString fail ulResult: %d ulParaLen: %d", result,
                         simlockWriteExCtx->hmacLen);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        result = AT_SendDrvAgentSimLockWrExReq(indexNum);
        if (result != AT_OK) {
            return result;
        }

        /* ????SIMLOCKDATAWRITEEX???????????????????????????? */
        if (At_StartTimer(AT_SET_PARA_TIME, indexNum) != AT_SUCCESS) {
            AT_WARN_LOG("AT_SetSimlockDataWriteExPara: At_StartTimer fail.");

            return AT_ERROR;
        }

        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_PEND;

        /* ????AT???????????????????????????? */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SIMLOCKDATAWRITEEX_SET;

        return AT_WAIT_ASYNC_RETURN;
    }
}

VOS_UINT32 AT_ParseSimlockDataWriteExParaValue(VOS_UINT8 *data, AT_SimlockWriteExPara *simlockWriteExPara,
                                               VOS_UINT16 pos, VOS_UINT16 len)
{
    VOS_UINT16 commPos[AT_COMMPOS_LENGTH] = {0};
    VOS_UINT32 firstParaVal = 0;
    VOS_UINT32 secParaVal = 0;
    VOS_UINT32 thirdParaVal = 0;
    VOS_UINT16 loop;
    VOS_UINT16 firstParaLen;
    VOS_UINT16 secondParaLen;
    VOS_UINT16 thirdParaLen;
    VOS_UINT16 fourthParaLen = 0;
    VOS_UINT16 fifthParaLen = 0;
    VOS_UINT16 commaCnt = 0;
    VOS_UINT8  paraNum = 0;

    /* ?????????????????????????? */
    for (loop = pos; loop < len; loop++) {
        if (*(data + loop) == ',') {
            /* ???????????????? */
            if (commaCnt < AT_COMMPOS_LENGTH) {
                commPos[commaCnt] = loop + 1;
            }
            commaCnt++;
        }
    }

    /* ??????????????4????AT???????????????? */
    if ((commaCnt != AT_CONST_NUM_4) && (commaCnt != AT_CONST_NUM_3)) {
        AT_WARN_LOG("AT_ParseSimlockDataWriteExParaValue: Num of Para is  Invalid!");
        return VOS_ERR;
    }

    /* ?????????????? */
    firstParaLen  = (commPos[AT_FIRST_COMMPOS] - pos) - (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
    secondParaLen = (commPos[AT_SECOND_COMMPOS] - commPos[AT_FIRST_COMMPOS]) -
                    (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
    thirdParaLen  = (commPos[AT_THIRD_COMMPOS] - commPos[AT_SECOND_COMMPOS]) -
                    (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);

    /* ????????????????3??????????????????4??????????????????4??????????????????5 */
    if (commaCnt == 3) {
        fourthParaLen = len - commPos[AT_THIRD_COMMPOS];
        paraNum       = 4;
    } else {
        fourthParaLen = (commPos[AT_FOURTH_COMMPOS] - commPos[AT_THIRD_COMMPOS]) -
                        (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
        fifthParaLen  = len - commPos[AT_FOURTH_COMMPOS];
        paraNum       = 5;
    }

    /* ???????????????? */
    if (atAuc2ul(data + pos, firstParaLen, &firstParaVal) == AT_FAILURE) {
        AT_WARN_LOG("AT_ParseSimlockDataWriteExParaValue: ulFirstParaVal value invalid");
        return VOS_ERR;
    }

    /* ???????????????? */
    if (atAuc2ul(data + commPos[0], secondParaLen, &secParaVal) == AT_FAILURE) {
        AT_WARN_LOG("AT_ParseSimlockDataWriteExParaValue: ulSecParaVal value invalid");
        return VOS_ERR;
    }

    /* ???????????????? */
    if (atAuc2ul(data + commPos[1], thirdParaLen, &thirdParaVal) == AT_FAILURE) {
        AT_WARN_LOG("AT_ParseSimlockDataWriteExParaValue: ulThirdParaVal value invalid");
        return VOS_ERR;
    }

    simlockWriteExPara->paraNum        = paraNum;
    simlockWriteExPara->layer          = firstParaVal;
    simlockWriteExPara->index          = secParaVal;
    simlockWriteExPara->total          = thirdParaVal;
    simlockWriteExPara->simLockDataLen = fourthParaLen;
    simlockWriteExPara->simLockData    = data + commPos[AT_THIRD_COMMPOS];

    /* ????????????????5 */
    if (paraNum == 5) {
        simlockWriteExPara->hmacLen = fifthParaLen;
        simlockWriteExPara->hmac    = data + commPos[AT_FOURTH_COMMPOS];
    }

    return VOS_OK;
}

/*
 * ????????: ????????(??????????????AT)??????????
 */
LOCAL VOS_UINT16 AT_GetSimLockDataWriteCmdLen(VOS_UINT8 *data, VOS_CHAR *atCmdStr, VOS_UINT32 atConstNum)
{
    AT_ParseCmdNameType   atCmdName;
    VOS_UINT16            pos;
    errno_t               memResult;

    /* ????????(??????????????AT)?????????? */
    pos = (VOS_UINT16)VOS_StrNLen("AT", AT_CONST_NUM_3);

    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrNLen(atCmdStr, atConstNum);
    memResult            = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrNLen("=", AT_CONST_NUM_2);

    return pos;
}

#if (FEATURE_SC_NETWORK_UPDATE == FEATURE_ON)
LOCAL VOS_UINT32 AT_CheckSimLockNWDataWriteCmdPara(VOS_UINT8 indexNum, VOS_UINT16 len,  VOS_UINT16 *cmdlen)
{
    /* ??????APPVCOM????19?????????? */
    if ((g_atClientTab[indexNum].userType != AT_APP_USER) ||
        (g_atClientTab[indexNum].portNo != APP_VCOM_DEV_INDEX_19)) {
        return VOS_FALSE;
    }

    (*cmdlen) = (VOS_UINT16)VOS_StrNLen("AT^SIMLOCKNWDATAWRITE=", AT_CONST_NUM_23);

    if (len < (*cmdlen)) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

VOS_UINT32 AT_HandleSimLockNWDataWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8            *dataPara = VOS_NULL_PTR;
    VOS_CHAR            *atCmdStr = "^SIMLOCKNWDATAWRITE";
    AT_SimlockWriteExPara simlockWriteExPara;
    VOS_UINT32            result;
    VOS_UINT32            atConstNum = 0;
    errno_t               memResult;
    VOS_UINT16            cmdlen = 0;
    VOS_UINT16            pos;
    VOS_UINT16            length;

    if (AT_CheckSimLockNWDataWriteCmdPara(indexNum, len, &cmdlen) == VOS_FALSE) {
        return AT_FAILURE;
    }

    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen);
    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleSimLockNWDataWriteCmd: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen, 0x00, cmdlen);

    /* ?????????????????????????? */
    memResult = memcpy_s(dataPara, cmdlen, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen, cmdlen);

    /* AT???????????????? */
    At_UpString(dataPara, cmdlen);

    /* ??????????????????????"AT^SIMLOCKDATAWRITEEX="????????AT_FAILURE */
    if (VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^SIMLOCKNWDATAWRITE=", cmdlen) != 0) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        dataPara = VOS_NULL_PTR;
        return AT_FAILURE;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^SIMLOCKNWDATAWRITE", AT_EXTEND_CMD_TYPE);

    /* ????????(??????????????AT)?????????? */
    atConstNum = AT_CONST_NUM_23;
    pos = AT_GetSimLockDataWriteCmdLen(data, atCmdStr, atConstNum);

    /* ?????????????? */
    (VOS_VOID)memset_s(&simlockWriteExPara, sizeof(simlockWriteExPara), 0x00, sizeof(simlockWriteExPara));
    simlockWriteExPara.pucSimLockData = VOS_NULL_PTR;
    simlockWriteExPara.pucHmac        = VOS_NULL_PTR;

    if (AT_ParseSimlockDataWriteExParaValue(data, &simlockWriteExPara, pos, len) != VOS_OK) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        dataPara = VOS_NULL_PTR;
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearSimLockWriteExCtx();
        AT_StopSimlockDataWriteTimer(indexNum);

        return AT_SUCCESS;
    }

    /* ???????????????????????????????? */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    result = AT_SetSimlockDataWriteExPara(&simlockWriteExPara, indexNum, AT_SIMLOCK_TYPE_NW);

    /* ???????? ^SIMLOCKNWDATAWRITE:<index>???? */
    length = 0;

    if (result != AT_WAIT_ASYNC_RETURN) {
        if (result == AT_OK) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", "^SIMLOCKNWDATAWRITE", simlockWriteExPara.ulIndex);
        }

        g_atSendDataBuff.usBufLen = length;
        At_FormatResultData(indexNum, result);
    }

    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    dataPara = VOS_NULL_PTR;
    return AT_SUCCESS;
}
#endif

/*
 * ????????: ????SIMLOCKDATA????????
 */
LOCAL VOS_VOID AT_PrintSimLockDataWriteCmd(VOS_UINT32 result, VOS_UINT8 indexNum, VOS_CHAR *atCmdStr,
    VOS_UINT32 simlockWriteExParaIndex)
{
    VOS_UINT16 length = 0;

    if (result != AT_WAIT_ASYNC_RETURN) {
        if (result == AT_OK) {
            AT_PR_LOGI("return OK");

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", atCmdStr, simlockWriteExParaIndex);
        }

        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, result);
    }
}
VOS_UINT32 AT_HandleSimLockDataWriteExCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8            *dataPara = VOS_NULL_PTR;
    VOS_CHAR             *atCmdStr = "^SIMLOCKDATAWRITEEX";
    VOS_UINT32            atConstNum = 0;
    AT_SimlockWriteExPara simlockWriteExPara;
    VOS_UINT32            result;
    errno_t               memResult;
    VOS_UINT16            cmdlen;
    VOS_UINT16            pos;
    VOS_INT8              ret;

    /* ?????????????? */
    cmdlen = (VOS_UINT16)VOS_StrNLen("AT^SIMLOCKDATAWRITEEX=", AT_CONST_NUM_23);

    /* ???????? */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_FAILURE;
    }

    /* ???????????????????????????? AT^SIMLOCKDATAWRITEEX=1,1,1,  ????????6?????? */
    if ((cmdlen + AT_SIMLOCKDATAWRITEEX_STR_MIN_LEN) > len) {
        return AT_FAILURE;
    }
    /*lint -save -e516*/
    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen);
    /*lint -restore */
    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleSimLockDataWriteExCmd: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen, 0x00, cmdlen);

    /* ?????????????????????????? */
    memResult = memcpy_s(dataPara, cmdlen, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen, cmdlen);

    /* AT???????????????? */
    At_UpString(dataPara, cmdlen);

    /* ??????????????????????"AT^SIMLOCKDATAWRITEEX="????????AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^SIMLOCKDATAWRITEEX=", cmdlen);
    if (ret != 0) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        return AT_FAILURE;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^SIMLOCKDATAWRITEEX", AT_EXTEND_CMD_TYPE);

    /* ????????(??????????????AT)?????????? */
    atConstNum = AT_CONST_NUM_23;
    pos = AT_GetSimLockDataWriteCmdLen(data, atCmdStr, atConstNum);

    /* ?????????????? */
    (VOS_VOID)memset_s(&simlockWriteExPara, sizeof(simlockWriteExPara), 0x00, sizeof(simlockWriteExPara));
    simlockWriteExPara.simLockData = VOS_NULL_PTR;
    simlockWriteExPara.hmac        = VOS_NULL_PTR;

    if (AT_ParseSimlockDataWriteExParaValue(data, &simlockWriteExPara, pos, len) != VOS_OK) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearSimLockWriteExCtx();
        AT_StopSimlockDataWriteTimer(indexNum);

        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);

        return AT_SUCCESS;
    }

    /* ???????????????????????????????? */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetSimlockDataWriteExPara(&simlockWriteExPara, indexNum, AT_SIMLOCK_TYPE_FAC);

    /* ???????? ^SIMLOCKDATAWRITEEX:<index>???? */
    AT_PrintSimLockDataWriteCmd(result, indexNum, atCmdStr, simlockWriteExPara.index);

    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    return AT_SUCCESS;
}

VOS_UINT32 AT_HandleSimLockOtaDataWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8            *dataPara = VOS_NULL_PTR;
    VOS_CHAR             *atCmdStr = "^SIMLOCKOTADATAWRITE";
    AT_SimlockWriteExPara simlockWriteExPara;
    VOS_UINT32            atConstNum = 0;
    VOS_UINT32            result;
    errno_t               memResult;
    VOS_UINT16            cmdlen;
    VOS_UINT16            pos;

    VOS_INT8              ret;

    /* ?????????????? */
    cmdlen = (VOS_UINT16)VOS_StrNLen("AT^SIMLOCKOTADATAWRITE=", AT_CONST_NUM_24);

    /* ???????? */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_FAILURE;
    }

    /* ???????????????????????????? AT^SIMLOCKOTADATAWRITE=1,1,1,  ????????6?????? */
    if ((cmdlen + AT_SIMLOCKOTADATAWRITE_STR_MIN_LEN) > len) {
        return AT_FAILURE;
    }
    /*lint -save -e516*/
    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen);
    /*lint -restore */
    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleSimLockOtaDataWriteCmd: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen, 0x00, cmdlen);

    /* ?????????????????????????? */
    memResult = memcpy_s(dataPara, cmdlen, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen, cmdlen);

    /* AT???????????????? */
    At_UpString(dataPara, cmdlen);

    /* ??????????????????????"AT^SIMLOCKOTADATAWRITE="????????AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^SIMLOCKOTADATAWRITE=", cmdlen);
    if (ret != 0) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        return AT_FAILURE;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^SIMLOCKOTADATAWRITE", AT_EXTEND_CMD_TYPE);

    /* ????????(??????????????AT)?????????? */
    atConstNum = AT_CONST_NUM_24;
    pos = AT_GetSimLockDataWriteCmdLen(data, atCmdStr, atConstNum);

    /* ?????????????? */
    (VOS_VOID)memset_s(&simlockWriteExPara, sizeof(simlockWriteExPara), 0x00, sizeof(simlockWriteExPara));
    simlockWriteExPara.simLockData = VOS_NULL_PTR;
    simlockWriteExPara.hmac        = VOS_NULL_PTR;

    if (AT_ParseSimlockDataWriteExParaValue(data, &simlockWriteExPara, pos, len) != VOS_OK) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearSimLockWriteExCtx();
        AT_StopSimlockDataWriteTimer(indexNum);

        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);

        return AT_SUCCESS;
    }

    /* ???????????????????????????????? */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetSimlockDataWriteExPara(&simlockWriteExPara, indexNum, AT_SIMLOCK_TYPE_OTA);

    /* ???????? ^SIMLOCKDATAWRITEEX:<index>???? */
    AT_PrintSimLockDataWriteCmd(result, indexNum, atCmdStr, simlockWriteExPara.index);

    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    return AT_SUCCESS;
}

VOS_UINT32 AT_SetIdentifyEndPara(VOS_UINT8 indexNum, VOS_UINT8 *rsaData, VOS_UINT32 rsaLen, VOS_UINT8 otaRsaFlag)
{
    DRV_AGENT_IdentifyendOtaSetReq identifyEndSetReq;
    errno_t                        memResult;
    VOS_UINT32                     result;
    VOS_UINT16                     length;

    AT_PR_LOGI("Rcv Msg");

    /* ?????????????? */
    result = VOS_NULL;
    length = (VOS_UINT16)rsaLen;
    (VOS_VOID)memset_s(&identifyEndSetReq, sizeof(identifyEndSetReq), 0x00, sizeof(DRV_AGENT_IdentifyendOtaSetReq));

#if (FEATURE_SC_RSA_PADDING_UPDATE == FEATURE_ON)
    length = (VOS_UINT16)AT_MIN(rsaLen, AT_RSA_TLV_CIPHERTEXT_PARA_LEN);
#else
    /* ???????????????? */
    if (length != AT_RSA_CIPHERTEXT_PARA_LEN) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
#endif

    /* ?????????????????????? */
    result = At_AsciiNum2HexString(rsaData, &length);
    if ((result != AT_SUCCESS) ||
#if (FEATURE_SC_RSA_PADDING_UPDATE == FEATURE_ON)
        (length < DRV_AGENT_RSA_CIPHERTEXT_LEN) || (length > DRV_AGENT_RSA_TLV_CIPHERTEXT_LEN)
#else
        (length != DRV_AGENT_RSA_CIPHERTEXT_LEN)
#endif
    ) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    memResult = memcpy_s(identifyEndSetReq.rsa.rsaText, sizeof(identifyEndSetReq.rsa.rsaText), rsaData, length);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(identifyEndSetReq.rsa.rsaText), length);
    identifyEndSetReq.rsa.rsaTextlen = length;
    identifyEndSetReq.otaFlag        = otaRsaFlag;

    /* ??????????????C??, ???????????? */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        DRV_AGENT_IDENTIFYEND_SET_REQ, (VOS_UINT8 *)&identifyEndSetReq, sizeof(DRV_AGENT_IdentifyendOtaSetReq),
        I0_WUEPS_PID_DRV_AGENT);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetIdentifyEndPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    AT_PR_LOGI("Snd Msg");

    /* ????AT???????????????????????????? */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IDENTIFYEND_SET;
    return AT_WAIT_ASYNC_RETURN;
}

VOS_VOID AT_HandleIdentifyEndCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len, const VOS_CHAR *specialCmdName)
{
    AT_ParseCmdNameType   atCmdName;
    VOS_UINT32            result;
    errno_t               memResult;
    VOS_UINT16            pos;
    VOS_UINT16            paraLen;

    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));
    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)specialCmdName, AT_EXTEND_CMD_TYPE);

    /* ????????(??????????????AT)?????????? */
    pos = (VOS_UINT16)VOS_StrNLen("AT", AT_CONST_NUM_3);

    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrNLen((VOS_CHAR *)specialCmdName, AT_SPECIAL_CMD_MAX_LEN);
    memResult            = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrNLen("=", AT_CONST_NUM_2);

    paraLen = len - pos;

    /* ???????????????????????????????? */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetIdentifyEndPara(indexNum, (data + pos), paraLen, VOS_FALSE);
    if (result != AT_WAIT_ASYNC_RETURN) {
        AT_PR_LOGI("return OK");

        At_FormatResultData(indexNum, result);
    }
}

VOS_UINT32 AT_ProcPhonePhynumCmdCtx(AT_PhonePhynumPara *phonePhynumSetPara){
    AT_PhonePhynumCmdProcCtx *phynumCmdCtx = VOS_NULL_PTR;
    errno_t                   memResult;

    phynumCmdCtx = AT_GetPhonePhynumCmdCtxAddr();

    if ((phonePhynumSetPara->phynumTypeLen == 0) ||
        (phonePhynumSetPara->phonePhynumValueLen == 0) ||
        (phonePhynumSetPara->hmacLen == 0)) {
        AT_ERR_LOG("AT_ProcPhonePhynumCmdCtx: NULL Pointer");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ?????????? */
    /* ?????????????????????????????????? */
    if (phynumCmdCtx->typeValue == VOS_NULL_PTR) {
        phynumCmdCtx->typeValueLen = phonePhynumSetPara->phynumTypeLen;
        phynumCmdCtx->typeValue    = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT,
            (VOS_UINT32)phonePhynumSetPara->phynumTypeLen);
        if (phynumCmdCtx->typeValue == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_PhonePhynumCmdCtx: first data, Alloc mem fail");
            return VOS_ERR;
        }
        (VOS_VOID)memset_s(phynumCmdCtx->typeValue, phynumCmdCtx->typeValueLen, 0x00, phynumCmdCtx->typeValueLen);

        memResult = memcpy_s(phynumCmdCtx->typeValue, phynumCmdCtx->typeValueLen, phonePhynumSetPara->phynumTypeData,
                             phonePhynumSetPara->phynumTypeLen);
        TAF_MEM_CHK_RTN_VAL(memResult, phynumCmdCtx->typeValueLen, phonePhynumSetPara->phynumTypeLen);
    }

    /* ?????????????????????????????????? */
    if (phynumCmdCtx->phynumValue == VOS_NULL_PTR) {
        phynumCmdCtx->phynumValueLen = phonePhynumSetPara->phonePhynumValueLen;
        phynumCmdCtx->phynumValue = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT,
            (VOS_UINT32)phonePhynumSetPara->phonePhynumValueLen);
        if (phynumCmdCtx->phynumValue == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_PhonePhynumCmdCtx: second data, Alloc mem fail");
            return VOS_ERR;
        }
        (VOS_VOID)memset_s(phynumCmdCtx->phynumValue, phynumCmdCtx->phynumValueLen, 0x00, phynumCmdCtx->phynumValueLen);

        memResult = memcpy_s(phynumCmdCtx->phynumValue, phynumCmdCtx->phynumValueLen,
                             phonePhynumSetPara->phonePhynumValue, phonePhynumSetPara->phonePhynumValueLen);
        TAF_MEM_CHK_RTN_VAL(memResult, phynumCmdCtx->phynumValueLen, phonePhynumSetPara->phonePhynumValueLen);
    }

    /* ?????????????????????????????????? */
    if (phynumCmdCtx->hmacValue == VOS_NULL_PTR) {
        phynumCmdCtx->hmacValueLen = phonePhynumSetPara->hmacLen;
        phynumCmdCtx->hmacValue    = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, (VOS_UINT32)phonePhynumSetPara->hmacLen);
        if (phynumCmdCtx->hmacValue == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_PhonePhynumCmdCtx: third data, Alloc mem fail");
            return VOS_ERR;
        }
        (VOS_VOID)memset_s(phynumCmdCtx->hmacValue, phynumCmdCtx->hmacValueLen, 0x00, phynumCmdCtx->hmacValueLen);

        memResult = memcpy_s(phynumCmdCtx->hmacValue, phynumCmdCtx->hmacValueLen,
                             phonePhynumSetPara->hmacData, phonePhynumSetPara->hmacLen);
        TAF_MEM_CHK_RTN_VAL(memResult, phynumCmdCtx->hmacValueLen, phonePhynumSetPara->hmacLen);
    }

    return AT_OK;
}

VOS_UINT32 AT_PhonePhynumParaValue(VOS_UINT8 *data, AT_PhonePhynumPara *phonePhynumPara,
                                   VOS_UINT16 pos, VOS_UINT16 len)
{
    VOS_UINT32 result;
    VOS_UINT32 loop;
    VOS_UINT16 firstParaLen;
    VOS_UINT16 secondParaLen;
    VOS_UINT16 thirdParaLen;
    VOS_UINT16 commaCnt                         = 0;
    VOS_UINT16 commPos[AT_PHONEPHYNUM_PARA_NUM] = {0};

    /* ?????????????????????????? */
    for (loop = pos; loop < (VOS_UINT32)AT_MIN(len, AT_CMD_MAX_LEN); loop++) {
        if (*(data + loop) == ',') {
            /* ???????????????? */
            if (commaCnt < AT_PHONEPHYNUM_PARA_NUM) {
                commPos[commaCnt] = (VOS_UINT16)(loop + 1);
            } else {
                break;
            }
            commaCnt++;
        }
    }

    /* ????????????????2????AT???????????????? */
    if (commaCnt != AT_CONST_NUM_2) {
        AT_WARN_LOG("AT_PhonePhynumParaValue: Num of Para is  Invalid!");
        return VOS_ERR;
    }

    /* ????commPos????????????pos???????????????????????????????? */
    firstParaLen  = (commPos[AT_PHONEPHYNUM_PARA_TYPE_NUM] - pos) - (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
    secondParaLen = (commPos[AT_PHONEPHYNUM_PARA_RSA_NUM] - commPos[AT_PHONEPHYNUM_PARA_TYPE_NUM]) -
                    (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
    thirdParaLen  = (len - commPos[AT_PHONEPHYNUM_PARA_RSA_NUM]);

    phonePhynumPara->phynumTypeLen       = firstParaLen;
    phonePhynumPara->phynumTypeData      = data + pos;
    phonePhynumPara->phonePhynumValueLen = secondParaLen;
    phonePhynumPara->phonePhynumValue    = data + commPos[AT_PHONEPHYNUM_PARA_TYPE_NUM];
    phonePhynumPara->hmacLen             = thirdParaLen;
    phonePhynumPara->hmacData            = data + commPos[AT_PHONEPHYNUM_PARA_RSA_NUM];

    /* ????????????????????  */
    result = AT_ProcPhonePhynumCmdCtx(phonePhynumPara);

    if (result != AT_OK) {
        return result;
    }

    return VOS_OK;
}

VOS_UINT32 AT_CheckPhonePhynumPara(AT_PhonePhynumCmdProcCtx *phonePhynumCtx, DRV_AGENT_PhPhynumTypeUint8 *setType)
{
    /* <type>????????: ??????????????IMEI??SN??????, ?????????????????????? */
    /* ???????????????????? */
    if (At_UpString(phonePhynumCtx->typeValue, phonePhynumCtx->typeValueLen) == AT_FAILURE) {
        return AT_PERSONALIZATION_PH_PHYNUM_TYPE_ERROR;
    }

    if (VOS_StrNiCmp((VOS_VOID *)phonePhynumCtx->typeValue, (VOS_VOID *)"IMEI",
                     (VOS_UINT32)phonePhynumCtx->typeValueLen) == 0) {
        *setType = DRV_AGENT_PH_PHYNUM_IMEI;
    }
    else if (VOS_StrNiCmp((VOS_VOID *)phonePhynumCtx->typeValue, (VOS_VOID *)"SN",
                          (VOS_UINT32)phonePhynumCtx->typeValueLen) == 0) {
        *setType = DRV_AGENT_PH_PHYNUM_SN;
    } else {
        AT_NORM_LOG("AT_CheckPhonePhyNumPara: the type of physical is error.");
        return AT_PERSONALIZATION_PH_PHYNUM_TYPE_ERROR;
    }

#if (FEATURE_SC_RSA_PADDING_UPDATE == FEATURE_OFF)
    /* ???????????? */
    if (phonePhynumCtx->phynumValueLen != AT_RSA_CIPHERTEXT_PARA_LEN) {
        AT_NORM_LOG1("AT_CheckPhonePhyNumPara: rsa para len error.", phonePhynumCtx->phynumValueLen);
        return AT_CME_INCORRECT_PARAMETERS;
    }
#endif

    if (phonePhynumCtx->hmacValueLen!= AT_SET_SIMLOCK_DATA_HMAC_LEN) {
        AT_NORM_LOG1("AT_CheckPhonePhyNumPara: hmac len error.", phonePhynumCtx->hmacValueLen);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}

VOS_UINT32 AT_SetPhonePhynumPara(VOS_UINT8 indexNum)
{
    AT_PhonePhynumCmdProcCtx    *phonePhynumCtx = VOS_NULL_PTR;
    DRV_AGENT_PhonephynumSetReq  phonePhynumSetReq;
    errno_t                      memResult;
    VOS_UINT32                   result;
    VOS_UINT32                   checkParaRst;
    VOS_UINT16                   length;

    /* ?????????????? */
    phonePhynumCtx = AT_GetPhonePhynumCmdCtxAddr();
#if (FEATURE_SC_RSA_PADDING_UPDATE == FEATURE_ON)
    length = AT_MIN(phonePhynumCtx->phynumValueLen, AT_RSA_TLV_CIPHERTEXT_PARA_LEN);
#else
    length = phonePhynumCtx->phynumValueLen;
#endif
    (VOS_VOID)memset_s(&phonePhynumSetReq, sizeof(phonePhynumSetReq), 0x00, sizeof(DRV_AGENT_PhonephynumSetReq));

    AT_PR_LOGI("Rcv Msg");

    checkParaRst = AT_CheckPhonePhynumPara(phonePhynumCtx,&(phonePhynumSetReq.phynumType));
    if (checkParaRst != AT_SUCCESS) {
        return checkParaRst;
    }

    /* ??RSA???????????????????? */
    result = At_AsciiNum2HexString(phonePhynumCtx->phynumValue, &length);
    if ((result != AT_SUCCESS) ||
#if (FEATURE_SC_RSA_PADDING_UPDATE == FEATURE_ON)
        (length < DRV_AGENT_RSA_CIPHERTEXT_LEN) || (length > DRV_AGENT_RSA_TLV_CIPHERTEXT_LEN)
#else
        (length != DRV_AGENT_RSA_CIPHERTEXT_LEN)
#endif
    ) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    memResult = memcpy_s(phonePhynumSetReq.phynumValue, sizeof(phonePhynumSetReq.phynumValue),
        phonePhynumCtx->phynumValue, length);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(phonePhynumSetReq.phynumValue), length);
    phonePhynumSetReq.rsaIMEILen = length;

    /* ??HMAC???????????????????? */
    length = phonePhynumCtx->hmacValueLen;
    result = At_AsciiNum2HexString(phonePhynumCtx->hmacValue, &length);
    if ((result != AT_SUCCESS) || (length != DRV_AGENT_HMAC_DATA_LEN)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    memResult = memcpy_s(phonePhynumSetReq.hmacData, sizeof(phonePhynumSetReq.hmacData),
        phonePhynumCtx->hmacValue, DRV_AGENT_HMAC_DATA_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(phonePhynumSetReq.hmacData), DRV_AGENT_HMAC_DATA_LEN);
    phonePhynumSetReq.hmacLen = DRV_AGENT_HMAC_DATA_LEN;

    /* ??????????????C??, ???????????? */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    DRV_AGENT_PHONEPHYNUM_SET_REQ, (VOS_UINT8 *)&phonePhynumSetReq,
                                    sizeof(DRV_AGENT_PhonephynumSetReq), I0_WUEPS_PID_DRV_AGENT);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetPhonePhynumPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    AT_PR_LOGI("Snd Msg");

    /* ????AT???????????????????????????? */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PHONEPHYNUM_SET;
    return AT_WAIT_ASYNC_RETURN;
}

VOS_VOID AT_HandlePhonePhynumCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len, const VOS_CHAR *specialCmdName)
{
    AT_ParseCmdNameType   atCmdName;
    AT_PhonePhynumPara    phonePhynumPara;
    VOS_UINT32            result;
    errno_t               memResult;
    VOS_UINT16            pos;

    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));
    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)specialCmdName, AT_EXTEND_CMD_TYPE);

    /* ????????(??????????????AT)?????????? */
    pos = (VOS_UINT16)VOS_StrNLen("AT", AT_CONST_NUM_3);

    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrNLen((VOS_CHAR *)specialCmdName, AT_SPECIAL_CMD_MAX_LEN);
    memResult            = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrNLen("=", AT_CONST_NUM_2);

    /* ?????????????? */
    (VOS_VOID)memset_s(&phonePhynumPara, sizeof(phonePhynumPara), 0x00, sizeof(phonePhynumPara));
    phonePhynumPara.phynumTypeData   = VOS_NULL_PTR;
    phonePhynumPara.phonePhynumValue = VOS_NULL_PTR;
    phonePhynumPara.hmacData         = VOS_NULL_PTR;

    if (AT_PhonePhynumParaValue(data, &phonePhynumPara, pos, len) != VOS_OK) {
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearPhonePhynumCtx();
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);

        return;
    }

    /* ???????????????????????????????? */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetPhonePhynumPara(indexNum);
    if (result != AT_WAIT_ASYNC_RETURN) {
        AT_PR_LOGI("return OK");

        At_FormatResultData(indexNum, result);
    }

    AT_ClearPhonePhynumCtx();
}

VOS_UINT32 AT_HandleApModemSpecialCmdProc(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len,
               const VOS_CHAR *specialCmdName)
{
    VOS_CHAR             *specialCmd     = VOS_NULL_PTR;
    VOS_UINT8            *dataPara       = VOS_NULL_PTR;
    AT_SpecialCmdProcFunc specialCmdFunc = VOS_NULL_PTR;
    errno_t               memResult;
    VOS_INT32             specialCmdLen;
    VOS_UINT32            i;
    VOS_UINT16            cmdlen;
    VOS_INT8              ret;

    /* ???????? */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_FAILURE;
    }

    specialCmd = (VOS_CHAR *)PS_MEM_ALLOC(WUEPS_PID_AT, VOS_StrLen(specialCmdName) + 4);
    if (specialCmd == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleApModemSpecialCmdProc: specialCmd Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(specialCmd, VOS_StrLen(specialCmdName) + 4, 0x00, VOS_StrLen(specialCmdName) + 4);

    specialCmdLen = snprintf_s(specialCmd, (VOS_SIZE_T)(VOS_StrLen(specialCmdName) + 4),
        (VOS_SIZE_T)VOS_StrLen(specialCmdName), "AT%s=", specialCmdName);
    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(specialCmdLen, VOS_StrLen(specialCmdName) + 4, VOS_StrLen(specialCmdName));

    /* ?????????????? */
    cmdlen = (VOS_UINT16)VOS_StrNLen(specialCmd, AT_SPECIAL_CMD_MAX_LEN);

    /* ??????????????????specialCmd?????????? */
    if (cmdlen > len) {
        PS_MEM_FREE(WUEPS_PID_AT, specialCmd);
        return AT_FAILURE;
    }

    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen + 1);
    if (dataPara == VOS_NULL_PTR) {
        PS_MEM_FREE(WUEPS_PID_AT, specialCmd);
        AT_ERR_LOG("AT_HandleApModemSpecialCmdProc: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen + 1, 0x00, cmdlen + 1);

    /* ?????????????????????????? */
    memResult = memcpy_s(dataPara, cmdlen + 1, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen + 1, cmdlen);

    /* AT???????????????? */
    At_UpString(dataPara, cmdlen);

    /* ??????????????????????specialCmd????????AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, specialCmd, cmdlen);
    if (ret != 0) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        PS_MEM_FREE(WUEPS_PID_AT, specialCmd);
        return AT_FAILURE;
    }

    for (i = 0; i < sizeof(g_ApModemSpecialCmdFucTab) / sizeof(AT_SpecialCmdFuncTbl); i++) {
        if (VOS_StrNiCmp(g_ApModemSpecialCmdFucTab[i].specialCmdName, specialCmd, cmdlen) == VOS_OK) {
            specialCmdFunc = g_ApModemSpecialCmdFucTab[i].specialCmdFunc;
            break;
        }
    }

    if (specialCmdFunc != VOS_NULL_PTR) {
        specialCmdFunc(indexNum, data, len, specialCmdName); /* ???????????? */
    }

    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    PS_MEM_FREE(WUEPS_PID_AT, specialCmd);
    return AT_SUCCESS;
}

