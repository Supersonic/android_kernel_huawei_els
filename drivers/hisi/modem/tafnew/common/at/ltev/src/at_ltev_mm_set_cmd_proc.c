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
#include "at_ltev_mm_set_cmd_proc.h"
#include "securec.h"

#include "AtParse.h"
#include "at_cmd_proc.h"
#include "at_snd_msg.h"
#if (FEATURE_LTEV == FEATURE_ON)
#include "vnas_at_interface.h"
#endif

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_LTEV_MM_SET_CMD_PROC_C

#if (FEATURE_LTEV == FEATURE_ON)

#define AT_SRCID_PARA_MIN_NUM 1
#define AT_SRCID_PARA_MAX_NUM 2

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 AT_SetSourceId(VOS_UINT8 indexNum)
{
    VNAS_LtevSourceIdSetReq sourceIdPara = {0};
    VOS_UINT32              rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数为1或者2 */
    if (g_atParaIndex != AT_SRCID_PARA_MIN_NUM && g_atParaIndex != AT_SRCID_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    sourceIdPara.isAtSourceIdEffective = (VOS_UINT8)g_atParaList[0].paraValue;
    sourceIdPara.sourceId              = g_atParaList[1].paraValue;
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_VNAS_LTEV_SOURCE_ID_SET_REQ,
                                 (VOS_UINT8 *)&sourceIdPara, sizeof(VNAS_LtevSourceIdSetReq), I0_PS_PID_VNAS);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SRCID_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif
#endif

