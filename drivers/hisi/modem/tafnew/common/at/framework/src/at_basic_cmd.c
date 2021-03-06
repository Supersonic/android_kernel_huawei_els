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

#include "at_basic_cmd.h"
#include "AtParse.h"
#include "at_cmd_proc.h"
#include "at_test_para_cmd.h"

#if (FEATURE_LTE == FEATURE_ON)
#include "at_lte_common.h"
#endif

#include "at_general_drv_set_cmd_proc.h"
#include "at_voice_taf_set_cmd_proc.h"
#include "at_voice_taf_qry_cmd_proc.h"
#include "at_data_basic_set_cmd_proc.h"
#include "at_data_basic_qry_cmd_proc.h"
#include "at_general_taf_set_cmd_proc.h"
#include "at_general_taf_qry_cmd_proc.h"


/*
 * ??????????????????????.C??????????
 */
#define THIS_FILE_ID PS_FILE_ID_AT_BASICCMD_C

/*
 * ????: ^CMDX ??????????E5??????????????????+CLAC????????????????????????????????????????????????????,
 *      ????????????????????????????????????????????????????
 * !!!!!!!!!!!????: param1??param2??????????????????????????????????????(??????????????)!!!!!!!!!!!!!
 *  {AT_CMD_CMDX,
 *  At_SetCmdxPara, AT_SET_PARA_TIME, At_QryCmdxPara, AT_QRY_PARA_TIME, At_TestCmdxPara, AT_NOT_SET_TIME,
 *  AT_ERROR, CMD_TBL_E5_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE,
 *  (VOS_UINT8 *)"^CMDX", (VOS_UINT8 *)"(@param1),(param2),(0-255)"},
 */

static const AT_ParCmdElement g_atBasicCmdTbl[] = {
    /* CS */
    /*
     * [????]: ????AT-??????????
     * [????]: ????????
     * [????]: ??????????????+CMOD????????????????????????????????????????????????????????????????????
     *         ??????????ITU T Recommendation V.250??????
     *         +CHUP??????H????????????+CHUP????????????????????????????????H??????
     * [????]:
     *     [????]: H[<value>]
     *     [????]: <CR><LF>OK<CR><LF>
     * [????]:
     *     <value>: ????????????????????0??
     *             ????????????????????????ERROR??
     *             ??????????????<value>??????????????ATH0????????ATH??ATH0??????????????
     * [????]:
     *     ?? ??????????????????
     *       ATH
     *       OK
     */
    { AT_CMD_H,
      At_SetHPara, AT_DETACT_PDP_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"H", (VOS_UINT8 *)"(0)" },

    /*
     * [????]: ????AT-??????????
     * [????]: ????????
     * [????]: ????????????????
     *         ??????????ITU T Recommendation V.250??????
     * [????]:
     *     [????]: A
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????????
     *             <CR><LF>NO CARRIER<CR><LF>
     *             ??????????
     *             <CR><LF> ERROR<CR><LF>
     *             ????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     * [????]:
     * [????]:
     *     ?? ????????????
     *       ATA
     *       OK
     */
    { AT_CMD_A,
      At_SetAPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
      (VOS_UINT8 *)"A", VOS_NULL_PTR },

    /*
     * [????]: ????AT-??????????
     * [????]: ????????
     * [????]: ??????????????????????????????????
     *         ??????????ITU T Recommendation V.250??????
     *         MT??????TE??????PDN??Packet Data Network??????????????????????????????????????????
     *         ??????????3GPP TS 27.007??????
     *         ??????CUG??????????
     *         ??????ME??????????????????????????????ME????????????????????
     * [????]:
     *     [????]: D[<dial_string>][I][G][;]
     *     [????]: ??????<dial_string>??????????????
     *             <CR><LF>OK<CR><LF>
     *             ??????????
     *             ??????????
     *             <CR><LF>BUSY<CR><LF>
     *             ????????????
     *             <CR><LF>NO CARRIER<CR><LF>
     *             ????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     *     [????]: D>mem<n>[I][G][;]
     *     [????]: ????????????mem??????????<n>????????????????????????????????????????????????????+CPBS=???????
     *     [????]: D><n>[I][G][;]
     *     [????]: ??????????????????????<n>??????????????????????????????????
     *     [????]: D><name>[I][G][;]
     *     [????]: ??????????????????????<name>??????????????????????????????????
     *     [????]: D*<mmi_string>[#]
     *     [????]: ????MMI??????mmi_string??
     *     [????]: D*<GPRS_SC>[*[<called_address>][*[<L2P>][*[<cid>]]]]#
     *     [????]: ????????????????
     *             <CR><LF>CONNECT<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     *     [????]: D*<GPRS_SC_IP>[*<cid>]#
     *     [????]: ??????????IP??????
     *             <CR><LF>CONNECT<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     * [????]:
     *     <I>: ??????????????I????????????????????????????????????
     *             ??????????????i????????????????????????????????????
     *     <G>: ??????????????G????g??????????????????????CUG??????????
     *     <??????;??>: ??????????????????????????????????????
     *             ??????????????????????????????????
     *     <dial_string>: ??????????
     *     <GPRS_SC>: GPRS??????????????99??
     *     <called_address>: ????????????????
     *     <L2P>: TE??MT??????????????????????????????????????????PPP??????
     *             NULL??????????????????
     *             PPP??????????PPP??????
     *     <cid>: ????????PDP????????????????????????????????cid????????PDP??
     *     <GPRS_SC_IP>: GPRS??IP????????????????????98??
     * [????]:
     *     ?? ????????????????????????????????????????????
     *       ATD13900000000I;
     *       OK
     *     ?? ????????????28????????????????????????????????????
     *       ATD>28;
     *       OK
     *     ?? ????????????????????????PDP????
     *       ATD*99**PPP*1#
     *       CONNECT
     *     ?? ??????????IP??????????????PDP????
     *       ATD*98*1#
     *       CONNECT
     */
    { AT_CMD_D,
      At_SetDPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"D", VOS_NULL_PTR },

#if (FEATURE_AT_HSUART == FEATURE_ON)
    /*
     * [????]: ????AT-GUC????
     * [????]: ??????????????????????
     * [????]: ??????????????????????????????????????
     *         ??????MBB??????????
     * [????]:
     *     [????]: O
     *     [????]: <CR><LF>CONNECT [<value >]<CR><LF>
     *             ????????????????
     *             <CR><LF>NO CARRIER<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     * [????]:
     *     <value>: ????????
     *             0??????????????????????????
     * [????]:
     *     ?? ??????????????????????
     *       ATO
     *       CONNECT 9600
     */
    { AT_CMD_O,
      AT_SetOPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"O", (VOS_UINT8 *)"(0)" },
#endif

    /*
     * [????]: ????AT-??????????
     * [????]: ????????????
     * [????]: ??????????????
     *         ??????????ITU T Recommendation V.250??????
     * [????]:
     *     [????]: S0=[<n>]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ????????:
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     *     [????]: S0?
     *     [????]: <CR><LF><n><CR><LF>
     *             <CR><LF>OK<CR><LF>
     *     [????]: S0=?
     *     [????]: <CR><LF>S0: (list of supported <n>s) <CR><LF>
     *             <CR><LF>OK<CR><LF>
     * [????]:
     *     <n>: ????????????????0??
     *             0????????????????
     *             1??255????????????????????????????s??
     * [????]:
     *     ?? ????????????????????5s
     *       ATS0=5
     *       OK
     *     ?? ????????????????
     *       ATS0?
     *       5
     *       OK
     *     ?? ????S0
     *       ATS0=?
     *       S0: (0-255)
     *       OK
     */
    { AT_CMD_S0,
      At_SetS0Para, AT_NOT_SET_TIME, At_QryS0Para, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"S0", (VOS_UINT8 *)"(0-255)" },

    /* ATC */
    /*
     * [????]: ????AT-????????
     * [????]: ????????????
     * [????]: ????????????????S3??
     *         ????S3??????ASCII??????????????
     *         ??TE????????????????????????????MT??????????
     *         ??MT??????????????????????????????????????????????????????????????
     *         ??????S3=<value>??????????S3????????????????S3??????????????????????????????????????????????????????????????????S3??????????????????????????
     *         ??????????ITU T Recommendation V.250??????
     * [????]:
     *     [????]: ATS3=[<value>]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     *     [????]: ATS3?
     *     [????]: <CR><LF><value><CR><LF>
     *             <CR><LF>OK<CR><LF>
     *     [????]: ATS3=?
     *     [????]: <CR><LF>S3: (list of supported <value>s)<CR><LF>
     *             <CR><LF>OK<CR><LF>
     * [????]:
     *     <value>: ??????????????????????????????13??
     *             0??127????ASCII????????????S3??????
     * [????]:
     *     ?? ????S3
     *       ATS3=13
     *       OK
     *     ?? ????S3
     *       ATS3?
     *       013
     *       OK
     *     ?? ????S3
     *       ATS3=?
     *       S3: (0-127)
     *       OK
     */
    { AT_CMD_S3,
      At_SetS3Para, AT_NOT_SET_TIME, At_QryS3Para, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"S3", (VOS_UINT8 *)"(0-127)" },

    /*
     * [????]: ????AT-????????
     * [????]: ????????????
     * [????]: ????????????????S4??????????????S4??????ASCII??????????????????????????MT??????????????????????????????????????????????????????????????
     *         ??????????????????????S4????????????????????????????????????????????S4????????????????????????????
     *         ??????????ITU T Recommendation V.250??????
     * [????]:
     *     [????]: ATS4=[<value>]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     *     [????]: ATS4?
     *     [????]: <CR><LF><value><CR><LF>
     *             <CR><LF>OK<CR><LF>
     *     [????]: ATS4=?
     *     [????]: <CR><LF>S4: (list of supported <value>s)<CR><LF>
     *             <CR><LF>OK<CR><LF>
     * [????]:
     *     <value>: ??????????????????????????????10??
     *             0??127????ASCII????????????S4??????
     * [????]:
     *     ?? ????S4
     *       ATS4=10
     *       OK
     *     ?? ????S4
     *       ATS4?
     *       010
     *       OK
     *     ?? ????S4
     *       ATS4=?
     *       S4: (0-127)
     *       OK
     */
    { AT_CMD_S4,
      At_SetS4Para, AT_NOT_SET_TIME, At_QryS4Para, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"S4", (VOS_UINT8 *)"(0-127)" },

    /*
     * [????]: ????AT-????????
     * [????]: ????????????
     * [????]: ????????????????S5??????????????S5??????ASCII??????????????????????TE????????????????????????????MT??????????
     *         ??????????ITU T Recommendation V.250??????
     * [????]:
     *     [????]: ATS5=[<value>]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     *     [????]: ATS5?
     *     [????]: <CR><LF><value><CR><LF>
     *             <CR><LF>OK<CR><LF>
     *     [????]: ATS5=?
     *     [????]: <CR><LF>S5: (list of supported <value>s)<CR><LF>
     *             <CR><LF>OK<CR><LF>
     * [????]:
     *     <value>: ??????????????????????????????8??
     *             0??127????ASCII????????????S5??????
     * [????]:
     *     ?? ????S5
     *       ATS5=8
     *       OK
     *     ?? ????S5
     *       ATS5?
     *       008
     *       OK
     *     ?? ????S5
     *       ATS5=?
     *       S5: (0-127)
     *       OK
     */
    { AT_CMD_S5,
      At_SetS5Para, AT_NOT_SET_TIME, At_QryS5Para, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"S5", (VOS_UINT8 *)"(0-127)" },

    { AT_CMD_S6,
      At_SetS6Para, AT_NOT_SET_TIME, At_QryS6Para, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"S6", (VOS_UINT8 *)"(2-10)" },

    { AT_CMD_S7,
      At_SetS7Para, AT_NOT_SET_TIME, At_QryS7Para, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"S7", (VOS_UINT8 *)"(1-255)" },

    /*
     * [????]: ????AT-????????
     * [????]: ????????
     * [????]: ????MT??????????TE????????????
     *         ??????????ITU T Recommendation V.250??????
     * [????]:
     *     [????]: E[<n>]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     * [????]:
     *     <n>: ??????????????????????????1??
     *             0??MT????????TE????????????
     *             1??MT??????TE????????????
     * [????]:
     *     ?? ????????
     *       ATE1
     *       OK
     *     ?? ????????
     *       ATE0
     *       OK
     */
    { AT_CMD_E,
      At_SetEPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_E5_IS_LOCKED | CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"E", (VOS_UINT8 *)"(0,1)" },

    /*
     * [????]: ????AT-????????
     * [????]: ????????????????
     * [????]: ????AT????????????????????????????????????????????????????????????????
     *         ??????????ITU T Recommendation V.250??????
     *         ??1??<CR>????S3??????
     *         ??2??<LF>????S4??????
     * [????]:
     *     [????]: V[<n>]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     * [????]:
     *     <n>: ??????????????????????????????1??
     *             0????????????????????????????????????????????????
     *             1??????????????????????????????????????????????????
     * [????]:
     *     ?? <n>??0??
     *       ATV0
     *       0
     *     ?? <n>??1??
     *       ATV1
     *       OK
     *     ?? ????????
     *       ATV=?
     *       ERROR
     */
    { AT_CMD_V,
      At_SetVPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"V", (VOS_UINT8 *)"(0,1)" },

    /*
     * [????]: ????AT-????????
     * [????]: ????MS????ID????
     * [????]: ????MS??????ID????????????????+GMI????????????????+GMM????????????????+GMR????ESN/IMEI??+GSN??????????????+GCAP????
     *         ??????????ITU T Recommendation V.250??????
     * [????]:
     *     [????]: I[<n>]
     *     [????]: <CR><LF><GMI>, <GMM>, <GMR>, <GSN>, <GCAP><CR><LF>
     *             <CR><LF>OK<CR><LF>
     * [????]:
     *     <n>: ??????????????????????????0??
     *             0??255??????MS????ID??????
     *     <GMI>: ??????????
     *     <GMM>: ????????????
     *     <GMR>: ????????????
     *     <GSN>: ESN/IMEI??
     *     <GCAP>: ??????????
     * [????]:
     *     ?? ????MS????ID????
     *       ATI0
     *       Manufacturer: Huawei Technologies CO.,Ltd.
     *       Model: H8180
     *       Revision: TEMP_VERSION
     *       IMEI: 350447370464090
     *       +GCAP: +CGSM,+DS,+ES
     *       OK
     */
    { AT_CMD_I,
      At_SetMsIdInfo, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_E5_IS_LOCKED | CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"I", (VOS_UINT8 *)"(0-255)" },

    { AT_CMD_T,
      At_SetTPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (TAF_UINT8 *)"T", TAF_NULL_PTR },

    { AT_CMD_P,
      At_SetPPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"P", TAF_NULL_PTR },

    { AT_CMD_X,
      At_SetXPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"X", (VOS_UINT8 *)"(0-4)" },

    { AT_CMD_Z,
      At_SetZPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"Z", (VOS_UINT8 *)"(0)" },

    { AT_CMD_Q,
      At_SetQPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"Q", (VOS_UINT8 *)"(0,1)" },

    /*
     * [????]: ????AT-GUC????
     * [????]: ????DCD????????
     * [????]: ??????????????DCD??????????
     *         ????????????HSUART??????????????????????????????OK??
     *         ??????MBB??????????
     * [????]:
     *     [????]: &C[value]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR:<err><CR><LF>
     * [????]:
     *     <value>: ??????
     *             0??DCD????????????ON
     *             1????????????????????????????DCD????????ON????????????????????????DCD????????OFF??
     * [????]:
     *     ?? ????DCD????????
     *       AT&C0
     *       OK
     */
    { AT_CMD_AMP_C,
      At_SetAmpCPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"&C", (VOS_UINT8 *)"(0-2)" },

    /*
     * [????]: ????AT-GUC????
     * [????]: ????DTR????????
     * [????]: ??????????????????????????????????DTR??????????ON??OFF????DCE????????????
     *         ????????????HSUART??????????????????????????????OK??
     *         ??????MBB??????????
     * [????]:
     *     [????]: &D[value]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR:<err><CR><LF>
     * [????]:
     *     <value>: ????????
     *             0??DCE????DTR??????????????
     *             1??DCE????????????????
     *             2??DCE??????????????
     * [????]:
     *     ?? ????DTR????????
     *       AT&D2
     *       OK
     */
    { AT_CMD_AMP_D,
      At_SetAmpDPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"&D", (VOS_UINT8 *)"(0-2)" },

#if (FEATURE_AT_HSUART == FEATURE_ON)
    /*
     * [????]: ????AT-GUC????
     * [????]: ????DSR????????
     * [????]: ??????????????DSR??????????
     *         ????????????HSUART??????????????????????????????OK??
     *         ??????MBB??????????
     * [????]:
     *     [????]: &S[value]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR:<err><CR><LF>
     * [????]:
     *     <value>: ??????
     *             0??DSR????????????ON??
     *             1??????????????????DSR????????ON??
     * [????]:
     *     ?? ????DSR????????
     *       AT&S0
     *       OK
     */
    { AT_CMD_AMP_S,
      At_SetAmpSPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"&S", (VOS_UINT8 *)"(0-1)" },
#endif

    { AT_CMD_L,
      AT_SetLPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"L", (VOS_UINT8 *)"(0)" },

    { AT_CMD_M,
      AT_SetMPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_E5,
      (VOS_UINT8 *)"M", (VOS_UINT8 *)"(1)" },

    /*
     * [????]: ????AT-????????
     * [????]: ????????????
     * [????]: ????????????????????????????????APP??????E5??????????????????????????????????????????APP????????????^FACTORY????????????????????????????
     *         ??????????ITU T Recommendation V.250??????
     * [????]:
     *     [????]: &F[<value>]
     *     [????]: <CR><LF>OK<CR><LF>
     *             ??????????
     *             <CR><LF>+CME ERROR: <err><CR><LF>
     * [????]:
     *     <value>: ????????????????0??
     *             Stick????????0????????????????
     *             E5????????0??????????????????????????
     * [????]:
     *     ?? ??????????????
     *       AT&F
     *       0
     *       OK
     */
    { AT_CMD_AMP_F,
      atSetNVFactoryRestore, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"&F", (VOS_UINT8 *)"(0)" },
};

VOS_UINT32 At_RegisterBasicCmdTable(VOS_VOID)
{
    return AT_RegisterCmdTable(g_atBasicCmdTbl, sizeof(g_atBasicCmdTbl) / sizeof(g_atBasicCmdTbl[0]));
}

const AT_ParCmdElement* At_GetBasicCmdTable(VOS_VOID)
{
    return g_atBasicCmdTbl;
}

VOS_UINT32 At_GetBasicCmdNum(VOS_VOID)
{
    return sizeof(g_atBasicCmdTbl) / sizeof(g_atBasicCmdTbl[0]);
}

VOS_UINT32 At_CheckIfDCmdValidChar(VOS_UINT8 charValue)
{
    /*
     * LINUX??????????????????????:ATDT"*99#"??????????????????????????????????????
     * 27007??ITUT-V.250????????????????????????????????????????LINUX??????????????
     * ????????????????????????
     */
    const VOS_UINT8 dCmdValidChar[] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', '+', 'A',
        'B', 'C', ',', 'T', 'P', '!', 'W', '@', '>', 'I', 'i', 'G', 'g', '"'
    };

    VOS_UINT32 indexNum = 0;

    while (indexNum < sizeof(dCmdValidChar)) {
        if (charValue == dCmdValidChar[indexNum]) {
            return AT_SUCCESS;
        }

        indexNum++;
    }

    return AT_FAILURE;
}

VOS_UINT8* At_GetFirstBasicCmdAddr(VOS_UINT8 *data, VOS_UINT32 *len)
{
    VOS_UINT32 i           = 0;
    VOS_UINT32 j           = 0;
    VOS_UINT8 *addr        = NULL;
    VOS_UINT8 *curAddr     = NULL;
    VOS_UINT32 dCmdLen     = 0;
    VOS_UINT32 basicCmdNum = sizeof(g_atBasicCmdTbl) / sizeof(AT_ParCmdElement);

    for (i = 0; i < basicCmdNum; i++) {
        if (g_atBasicCmdTbl[i].cmdName == NULL) {
            break;
        }

        curAddr = (VOS_UINT8 *)strstr((VOS_CHAR *)data, (VOS_CHAR *)(g_atBasicCmdTbl[i].cmdName));
        if (curAddr != NULL) {
            if ((addr == NULL) || (curAddr < addr)) {
                if (g_atBasicCmdTbl[i].cmdIndex == AT_CMD_D) { /* D?????????????????????????????????? */
                    dCmdLen = strlen((VOS_CHAR *)curAddr);

                    /* ????????D????????????????????1????'D'?????????? */
                    for (j = 1; j < dCmdLen; j++) {
                        if (At_CheckIfDCmdValidChar(*(curAddr + j)) != AT_SUCCESS) {
                            /* ??????D???????????????????? */
                            break;
                        }
                    }

                    *len = j;
                } else {
                    *len = strlen((VOS_CHAR *)(g_atBasicCmdTbl[i].cmdName));
                }

                addr = curAddr;
            }
        }
    }

    return addr;
}

