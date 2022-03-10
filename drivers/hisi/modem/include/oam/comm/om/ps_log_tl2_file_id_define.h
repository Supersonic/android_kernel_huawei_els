/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#ifndef PS_LOG_TL2_FILE_ID_DEFINE_H
#define PS_LOG_TL2_FILE_ID_DEFINE_H

#include "ps_log_file_id_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*TL2 file id begin*/
typedef enum
{
    /*mac*/
    PS_FILE_ID_T_MAC00NUL_C= TL2_TEAM_FILE_ID,
    PS_FILE_ID_T_MAC01SEL_C,
    PS_FILE_ID_T_MAC02IDL_C,
    PS_FILE_ID_T_MAC03ACC_C,
    PS_FILE_ID_T_MAC04FCH_C,
    PS_FILE_ID_T_MAC05DCH_C,
    PS_FILE_ID_T_MAC06SLP_C,
    PS_FILE_ID_T_MAC07IRAT_C,
    PS_FILE_ID_T_MAC08WAIT_C,
    PS_FILE_ID_T_MAC09AT_C,
    PS_FILE_ID_T_MAC10COM_C,
    PS_FILE_ID_T_MAC29COM_C,
    PS_FILE_ID_T_MAC30COM_C,
    PS_FILE_ID_T_MAC31COM_C,
    PS_FILE_ID_T_TMACCSVOICE_C,
    /*pdc*/
    PS_FILE_ID_T_PDC00NUL_C,
    PS_FILE_ID_T_PDC01TRA_C,
    PS_FILE_ID_T_PDC30PUB_C,
    PS_FILE_ID_T_PDC31CRE_C,
    /*rlc*/
    PS_FILE_ID_T_RLC00COM_C,
    PS_FILE_ID_T_RLC01NUL_C,
    PS_FILE_ID_T_RLC02AMT_C,
    PS_FILE_ID_T_RLC03TUT_C,
    PS_FILE_ID_T_RLC04AMS_C,
    PS_FILE_ID_T_RLC05UMS_C,
    PS_FILE_ID_T_RLC06RST_C,
    PS_FILE_ID_T_RLC07RAS_C,
    PS_FILE_ID_T_RLC20PUB_C,
    PS_FILE_ID_T_RLC21PUB_C,
    PS_FILE_ID_T_RLC22PUB_C,
    PS_FILE_ID_T_RLC23PUB_C,
    /*hl*/
    PS_FILE_ID_T_HL100ISR_C,
    PS_FILE_ID_T_HL101COMM_C,
    PS_FILE_ID_T_HL102_C,
    PS_FILE_ID_T_HL103_C,
    PS_FILE_ID_T_HL1_INTEGRATE_C,
    /*fsm*/
    PS_FILE_ID_T_FSMMAC_C,
    PS_FILE_ID_T_FSMPDC_C,
    PS_FILE_ID_T_FSMRLC_C,
    /*test*/
    PS_FILE_ID_T_TESTMAC_C,
    PS_FILE_ID_T_TESTPDC_C,
    PS_FILE_ID_T_TESTRLC_C,
    /*comm*/
    PS_FILE_ID_T_TDSL2DEBUG_C,
    PS_FILE_ID_T_TDSL2OM_C,
}TL2_FILE_ID_DEFINE_ENUM;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif


