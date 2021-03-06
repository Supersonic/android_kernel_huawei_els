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

#include "at_general_mm_cmd_tbl.h"
#include "at_general_mm_set_cmd_proc.h"
#include "at_general_mm_qry_cmd_proc.h"
#include "AtParse.h"
#include "at_cmd_proc.h"
#include "at_test_para_cmd.h"


/*
 * ??????????????????????.C??????????
 */
#define THIS_FILE_ID PS_FILE_ID_AT_GENERAL_MM_CMD_TBL_C

static const AT_ParCmdElement g_atGeneralMmCmdTbl[] = {
    /*
     * [????]: ????AT-????????
     * [????]: TE??MT????
     * [????]: ??????????TE??MT??????????TE??????????????????????????????????????MT??MT????????????????TE??
     * [????]:
     *     [????]: ^HS=<id>,<action>
     *     [????]: <CR><LF>^HS: <id>,<protocol>,<is_offline>,<product_class>,<product_id>,[<software_id>]<CR><LF>
     *             <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     *     [????]: ^HS=?
     *     [????]: <CR><LF>^HS: (list of supported <id>s),(list of supported <action>s)<CR><LF>
     *             <CR><LF>OK<CR><LF>
     * [????]:
     *     <id>: ????????8??????????MT????????????MT????????TE????????????????id????0??????????id????MT??????????
     *     <action>: ????????????TE??????????????0??1??
     *             0??TE????Living??????????TE????????????????????????
     *             1??TE??????
     *     <protocol>: ????????????????????????????????????????????0??1??
     *             0????????????APP??
     *             1????????????BOOT??
     *     <is_offline>: ??????????????????????0??2??
     *             0??online??????
     *             1??offline??????
     *             2??LPM????????????????
     *     <product_class>: ??????????????????
     *             0??GW??
     *     <product_id>: ??????????????ID??????????????????????????????????ID??
     *     <software_id>: ??????????????
     * [????]:
     *     ?? ????????
     *       AT^HS=0,0
     *       ^HS: 12345678,0,1,0,55
     *       OK
     */
    { AT_CMD_HS,
      At_SetHSPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^HS", (VOS_UINT8 *)"(0-99999999),(0,1)" },

    /*
     * [????]: ????AT-????????
     * [????]: ????????????
     * [????]: ????????????????????????????<PLMN>??<LAC>??<RAC>??<CELL ID>??
     * [????]:
     *     [????]: ^LOCINFO?
     *     [????]: <CR><LF>^LOCINFO: <PLMN>,<LAC>,<RAC>,<CELL ID><CR><LF><CR><LF>OK<CR><LF>
     * [????]:
     *     <PLMN>: PLMN??????????????????????????????????MCC????????????????MNC??
     *     <LAC>: ??????3bytes??16????????????????????????????????????????0xC3??????10??????195??
     *             ????????LTE/NR????????????TAC????????????????????LAC????
     *     <RAC>: ??????2bytes??16??????????????????????????????????????????0xC3??????10??????195??
     *     <CELL ID>: 8bytes??16????????????????????????????????????<LAC>??????Half-Byte??????????????????????0x100000005????????4??????0x00000001????4??????0x00000005??
     * [????]:
     *     ?? ????UE????????????
     *       AT^LOCINFO?
     *       ^LOCINFO:24005,0xC3,0xC3,0x5
     *       OK
     *       ????NR????????UE????????????
     *          AT^LOCINFO?
     *          ^LOCINFO:24005,0xC3,0xC3,0x100000005
     *          OK
     */
    { AT_CMD_LOCINFO,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryLocinfoPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
      (VOS_UINT8 *)"^LOCINFO", VOS_NULL_PTR },

    /*
     * [????]: ????AT-????????
     * [????]: ????OPLMN????????
     * [????]: ????????????????????????OPLMN????????????OPLMN??????????NV????????SIM??????OPLMN????????????????OPLMN????????????????SIM??????OPLMN??????
     *         ????OPLMN??????????NV????????????????????IMSI????????????????????????????????????????????256??OPLMN????????????6????????????????????????50??OPLMN??
     *         ??????1????????????length??????0??pdu????????????????????????????????OPLMN??????
     *         CL????????????????????
     * [????]:
     *     [????]: ^EOPLMN=<version>,<index>,<length>,<pdu>
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>ERROR<CR><LF>
     *     [????]: ^EOPLMN?
     *     [????]: <CR><LF>^EOPLMN: <version>,<length>,<pdu><CR><LF>
     *             <CR><LF>OK<CR><LF>
     *     [????]: ^EOPLMN=?
     *     [????]: <CR><LF>OK<CR><LF>
     * [????]:
     *     <version>: ????????????????????????????0??7????8????????????????????????????????OPLMN????????
     *     <index>: ??????????????????????????????????????0~5??
     *             0????1??
     *             1????2??
     *             2????3??
     *             3????4??
     *             4????5??
     *             5????6??
     *     <length>: ????????????PDU????????????????[0,500]??
     *     <pdu>: ??????????????????[0,500]??????????????OPLMN??????OPLMN??10????????????
     * [????]:
     *     ?? ????????OPLMN????
     *       AT^EOPLMN="1.0",0,10,"64F0008080"
     *       OK
     *     ?? ????????OPLMN????
     *       AT^EOPLMN?
     *       ^EOPLMN: "1.0",10,"64F0000808"
     *       OK
     *     ?? ????????OPLMN ????
     *       AT^EOPLMN=?
     *       OK
     */
    { AT_CMD_EOPLMN,
      AT_SetUserCfgOPlmnPara, AT_SET_PARA_TIME, AT_QryUserCfgOPlmnPara, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^EOPLMN", (VOS_UINT8 *)"(@version),(0-5),(0-500),(@PDU_DATA)" },

    /*
     * [????]: ????AT-????????
     * [????]: ????????UE??????????
     * [????]: ??????????????????????????UE????????????
     *         ??????????3GPP TS 27.007??????
     * [????]:
     *     [????]: +WS46=<n>
     *     [????]: ??????????
     *             <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>ERROR<CR><LF>
     *     [????]: +WS46?
     *     [????]: <CR><LF><n><CR><LF><CR><LF>OK<CR><LF>
     *     [????]: +WS46=?
     *     [????]: <CR><LF> (list of supported <n>s) <CR><LF><CR><LF>OK<CR><LF>
     * [????]:
     *     <+WS46=<n>??????<n>>: <n>:integer type
     *             12: 3GPP System
     *             22,25,28,29,30,31??????12??????????
     *     <+WS46=???\n??????<n>>: 12??GSM Digital Cellular Systems (GERAN only)
     *             22??UTRAN only
     *             25??3GPP Systems (GERAN, UTRAN and E-UTRAN)
     *             28??E-UTRAN only
     *             29??GERAN and UTRAN
     *             30??GERAN and E-UTRAN
     *             31??UTRAN and E-UTRAN
     * [????]:
     *     ?? ????????
     *       AT+WS46=12
     *       OK
     *     ?? ????????
     *       AT+WS46?
     *       25
     *       OK
     *     ?? ????????
     *       AT+WS46=?
     *       (12,22,25,28,29,30,31)
     *       OK
     */
    { AT_CMD_WS46,
      AT_SetWs46Para, AT_SET_PARA_TIME, AT_QryWs46Para, AT_QRY_PARA_TIME, AT_TestWs46Para, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"+WS46", (VOS_UINT8 *)"(12,22,25,28,29,30,31)" },

    /*
     * [????]: ????AT-GUC????
     * [????]: ????RSSI????
     * [????]: ??RSSI ????????NdB??N????????1-5??????????????????????????????????M(M????????1-20s)????MT??????????????TE??
     *         ??AT????????????????^CURC??????
     * [????]:
     *     [????]: ^CERSSI=<n>,<m>
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>ERROR<CR><LF>
     *     [????]: ^CERSSI?
     *     [????]: ??????????
     *             <CR><LF>^CERSSI: <n>,<m>,<RSSI>,<RSCP>,<EC/IO>,
     *             <RSRP>,<RSRQ>,<RSSNR>,<RI>,<CQI0>,<CQI1>,<5GRSRP>,<5GRSRQ>,<5GRSSNR><CR><LF>
     *             <CR><LF>OK<CR><LF>
     *             MBB??????
     *             <CR><LF>^CERSSI: <n>,<m>,<RSSI>,<RSCP>,<EC/IO>,<RSRP>,<RSRQ>,<RSSNR>,<RI>,<CQI0>,<CQI1>,<RXANTNUM >,
     *             <RSRPRX0>,<RSRPRX1>,<RSRPRX2>,<RSRPRX3>,<SINRRX0>,<SINRRX1>,<SINRRX2>,<SINRRX3>,<5GRSRP>,<5GRSRQ>,<5GRSSNR><CR><LF>
     *             <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>ERROR<CR><LF>
     *             ??????
     *             1????????????????????????????????????????MBB????????<EC/IO>,<RSRP>,<RSRQ>,<RSSNR>,<RI>[,<CQI0>][,<CQI1>],<rxANTNum>,<rsrpRx0>,<rsrpRx1>,<rsrpRx2>,<rsrpRx3>,<sinrRx0>,<sinrRx1>,<sinrRx2>,<sinrRx3>,<s5GRsrp>,<s5GRsrq>,<l5GSinr>??????????????255, 0, 0, 0, 0, 32639, 32639, 0, 99, 99, 99, 99, 99, 99, 99, 99, 0, 0, 0
     *             2??ANDROID????????<EC/IO>,<RSRP>,<RSRQ>,<RSSNR>,<RI>[,<CQI0>][,<CQI1>],<s5GRsrp>,<s5GRsrq>,<l5GSinr>??????????????255, 0, 0, 0, 0, 32639, 32639, 0, 0, 0
     *             3????????????????????????????????????????????????????????
     *     [????]: ^CERSSI=?
     *     [????]: <CR><LF>^CERSSI: (list of supported <n>s), (list of supported<m>s) <CR><LF>
     *             <CR><LF>OK<CR><LF>
     *     [????]: -
     *     [????]: 1??NV_System_APP_Config????????ANDROID??MBB??????: <CR><LF>^CERSSI: <RSSI>,<RSCP>,<EC/IO>,<RSRP>,<RSRQ>,<RSSNR>,<RI>,<CQI0>,<CQI1>,<RXANTNUM >,<RSRPRX0>,<RSRPRX1>,<RSRPRX2>,<RSRPRX3>,<SINRRX0>,<SINRRX1>,<SINRRX2>,<SINRRX3>,<5GRSRP>,<5GRSRQ>,<5GRSSNR><CR><LF>
     *             2??NV_System_APP_Config??????ANDROID????:
     *             <CR><LF>^CERSSI: <RSSI>,<RSCP>,<EC/IO>,<RSRP>,<RSRQ>,<RSSNR>,<RI>,<CQI0>,<CQI1>,<5GRSRP>,<5GRSRQ>,<5GRSSNR><CR><LF>
     * [????]:
     *     <n>: 0??????????????????????????,??<n>??0????<m>????????????????0??
     *             1~5????????????????????ndBm??????????????????????????????????
     *             ??????????????????????????????????????
     *     <m>: 0??????????????????????????????
     *             1-20????????????????????????????????????????S??
     *             ????????????????????????0??
     *     <RSSI>: UE????2G????????????????????????????2G??????????????0??
     *     <RSCP>: UE????3G????????????????????????????3G??????????????0??
     *     <EC/IO>: UE????3G FDD??????????????????????????????3G FDD??????????????255??
     *     <RSRP>: UE????4G??????????????????????????????????99????????????????????4G??????????????0??
     *     <RSRQ>: UE????4G??????????????????????????????????99????????????????????4G??????????????0??
     *     <RSSNR>: UE????4G??????????????????????4G??????????????0??
     *             ????????????SINR??????????[-10,40]??????????99??
     *     <RI>: RI??????1??????????????????CQI????????CQI0??????????????????????CQI??????????????????????0x7F7F??????????CQI1????
     *             RI??????2??????????????????CQI????????CQI0??CQI1??
     *             UE??????4G????RI??????0??????CQI0??CQI1??????
     *             [CQI0]??UE????4G????????????????????????
     *             [CQI1]??UE????4G????????????????????????
     *     <5GRSRP>: UE ????5G??????????????????????????????????(-141,-44)????????99????????5G??????????????0??
     *     <5GRSRQ>: UE ????5G??????????????????????????????????(-40, -6) ????????99????????5G??????????????0??
     *     <5GRSSNR>: UE ????5G??????????????????????5G??????????????0??????????????SINR??????????[-10,40]??????????99??
     *     <RXANTNUM>: ??????????????????4????
     *     <RSRPRX>: RSRPRX0??UE ????4G ??????0??????????????????????????4G ??????????????99??
     *             RSRPRX1??UE????4G ??????1??????????????????????????4G ??????????????99??
     *             RSRPRX2??UE????4G ??????2??????????????????????????4G ??????????????99??
     *             RSRPRX3??UE????4G ??????3??????????????????????????4G ??????????????99??
     *     <SINRRX>: SINRRX0??????0??????????UE????4G??????????????????????4G??????????????99??
     *             SINRRX1??????1??????????UE????4G??????????????????????4G??????????????99??
     *             SINRRX2??????2??????????UE????4G??????????????????????4G??????????????99??
     *             SINRRX3??????3??????????UE????4G??????????????????????4G??????????????99??
     * [????]:
     *     ?? ???? 2G??????????????????????5dB ????????
     *       AT^CERSSI=5
     *       OK
     *       ?6?1 ???????????????? 5dB??????????
     *       ^CERSSI: -42,0,255,0,0,0,0,32639,32639,0,0,0
     *       OK
     *     ?? ?6?1 ????????
     *       AT^CERSSI?
     *       ^CERSSI: 5,0,-42,0,255,0,0,0,0,32639,32639,0,0,0
     *       OK
     *     ?? ???? 3G??????????????????????3dB ????????????????5s??????????
     *       AT^CERSSI=3,5
     *       OK
     *       ?6?1 ???????????????? 3dB ????????????????????5s??????????
     *       ^CERSSI: 0,-68,-6,0,0,0,0,32639,32639,0,0,0
     *       OK
     *     ?? ?6?1 ????????
     *       AT^CERSSI?
     *       ^CERSSI: 3,5,0,-68,-6,0,0,0,0,32639,32639,0,0,0
     *       OK
     *     ?? ???? 4G??????????????????????4dB ????????????????5s??????????
     *       AT^CERSSI=4,5
     *       OK
     *       ?6?1 ???????????????? 4dB ????????????????5s??????????
     *       ^CERSSI: 0,0,255,-61,-42,5,2,2,0,0,0,0
     *       OK
     *     ?? ?6?1 ????????
     *       AT^CERSSI?
     *       ^CERSSI: 4,5,0,0,255,-61,-42,5,2,2,0,0,0,0
     *       OK
     *     ?? ???? 5G??????????????????????4dB ????????????????5s??????????
     *       AT^CERSSI=4,5
     *       OK
     *       ?6?1 ???????????????? 4dB ????????????????5s??????????
     *       ^CERSSI: 0,0,255,0,0,0,0,32639,32639,-61,-42,5
     *       OK
     *     ?? ?6?1 ????????
     *       AT^CERSSI?
     *       ^CERSSI: 4,5,0,0,255,0,0,0,0,32639,32639,-61,-42,5
     *       OK
     *     ?? ????????????
     *       AT^CERSSI=?
     *       ^CERSSI: (0,1,2,3,4,5),(0-20)
     *       OK
     */
    { AT_CMD_CERSSI,
      At_SetCerssiPara, AT_SET_PARA_TIME, At_QryCerssiPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CERSSI", (VOS_UINT8 *)"(0,1,2,3,4,5),(0-20)" },

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    /*
     * [????]: ????AT-????????
     * [????]: ????NSA???????????? RSSI ????
     * [????]: ??NSA??????????????????RSSI ????????NdB??N ????????1-5??????????????????????????????????M(M????????1-20s)????MT ??????????????TE??
     *         ??????????????NR????????????????????????????????????????????
     * [????]:
     *     [????]: -
     *     [????]: <CR><LF>^CSERSSI: <RSSI>,<RSCP>,<EC/IO>,<RSRP>,<RSRQ>,<RSSNR>,<RI>,<CQI0>,<CQI1>,<5GRSRP>,<5GRSRQ
     *             >,<5GRSSNR><CR><LF>
     *     [????]: ^CSERSSI?
     *     [????]: <CR><LF>^CSERSSI: <n>,<m>,<RSSI>,<RSCP>,<EC/IO>,<RSRP>,<RSRQ>,<RSSNR>,<RI>,<CQI0>,<CQI1>,<5GRSRP>,<5GRSRQ>,<5GRSSNR><CR><LF>
     *             <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>ERROR<CR><LF>
     *             ??????????????????????????????????????????????????
     *             <RSSI>,<RSCP>,<EC/IO>,<RSRP>,<RSRQ>,<RSSNR>,<RI>
     *             , <CQI0>,<CQI1>,<5GRSRP>,<5GRSRQ>,<5GRSSNR>??????????????0,0,255,0,0,0,0,32639,32639,0,0,0
     *             ????????????????????????????????????????????????????
     * [????]:
     *     <n>: 0??????????????????????????,??<n>??0 ????<m>????????????????
     *             0??
     *             1~5????????????????????ndBm??????????????????????????????????
     *             ??????????????????????????????????????
     *     <m>: 0??????????????????????????????
     *             1-20????????????????????????????????????????S??
     *             ????????????????????????0??
     *     <RSSI>: UE ????2G ????????????????????????????2G ??????????????0??
     *     <RSCP>: UE ????3G ????????????????????????????3G ??????????????0??
     *     <EC/IO>: UE ????3G FDD ??????????????????????????????3G FDD ????????
     *             ??????255??
     *     <RSRP>: UE ????4G ????????????????????????99????????????????????4G ??
     *             ????????????0??
     *     <RSRQ>: UE ????4G ????????????????????????99????????????????????4G ??
     *             ????????????0??
     *     <RSSNR>: UE ????4G ??????????????????????4G ??????????????0??
     *             ????????????SINR??????????99??
     *     <RI>: RI ??????1 ??????????????????CQI????????CQI0 ????????????????
     *             ??????CQI ??????????????????????0x7F7F??????????CQI1 ????
     *             RI ??????2 ??????????????????CQI????????CQI0 ??CQI1??
     *             UE ??????4G ????RI ??????0??CQI0??CQI1????????????????????32639????
     *             [CQI0]??UE ????4G ????????????????????????
     *             [CQI1]??UE ????4G ????????????????????????
     *     <5GRSRP>: UE ????5G????????????????????????99????????????????????5G??????????????0??
     *     <5GRSRQ>: UE ????5G????????????????????????99????????????????????5G??????????????0??
     *     <5GRSSNR>: UE ????5G??????????????????????5G??????????????0??
     *             ????????????SINR??????????[-10,40]??????????99??
     * [????]:
     *     ?? ??????????????????????????????????4dB ????????????????5s
     *       AT^CERSSI=4,5
     *       AT^CSERSSI?
     *       ^CSERSSI: 4,5,0,0,255,0,0,0,0,32639,32639,0,0,0
     *       OK
     *     ?? ?????????????? 3G
     *       AT^CSERSSI?
     *       ^CSERSSI: 4,5,0,0,255,0,0,0,0,32639,32639,0,0,0
     *       OK
     *     ?? ?????????????? 4G??SA????
     *       AT^CSERSSI?
     *       ^CSERSSI: 4,5,0,0,255,0,0,0,0,32639,32639,0,0,0
     *       OK
     *     ?? ?????????????? 5G??SA????
     *       AT^CSERSSI?
     *       ^CSERSSI: 4,5,0,0,255,0,0,0,0,32639,32639,0,0,0
     *       OK
     *     ?? ?????????????? 4G??NSA????
     *       AT^CSERSSI?
     *       ^CSERSSI: 4,5,0,0,255,0,0,0,0,32639,32639,-61,-42,5
     *       OK
     *       ?????????????? 4G??NSA????
     *       ?6?1 NR???????????????? 4dB ????????????????5s??????????
     *       ^CSERSSI: 0,0,255,0,0,0,0,32639,32639,-61,-42,5
     *       OK
     */
    { AT_CMD_CSERSSI,
      VOS_NULL_PTR, AT_NOT_SET_TIME, At_QryCserssiPara, AT_QRY_PARA_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CSERSSI", VOS_NULL_PTR },
#endif
};

/* ????LMM????AT?????? */
VOS_UINT32 AT_RegisterGeneralMmCmdTable(VOS_VOID)
{
    return AT_RegisterCmdTable(g_atGeneralMmCmdTbl, sizeof(g_atGeneralMmCmdTbl) / sizeof(g_atGeneralMmCmdTbl[0]));
}

