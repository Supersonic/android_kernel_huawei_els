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

#ifndef __NRRDR_AGENT_H__
#define __NRRDR_AGENT_H__
#include "product_config.h"
#include "osl_types.h"
#ifdef BSP_CONFIG_PHONE_TYPE
#include <adrv.h>
#else
#include <bsp_rdr.h>
#endif
#define DUMP_NR_QUEUE_NODE_SIZE (4)
#define DUMP_NR_TASK_NAME_SIZE (16)
#define DUMP_NR_WAIT_DONE_FIRST (5000)
#define DUMP_NR_WAIT_DONE_FIQ (15000)
#define DUMP_NR_WAIT_DELAY_BLOCK (1000 * 1)
#define DUMP_NR_WAIT_DELAY_NOBLOCK (5)
#define DUMP_NR_CCPU_MAX_CORE (4)
enum nr_sub_sys_e {
    NR_SYS_CCPU = 0x0,
    NR_SYS_L2HAC = 0x1,
    NR_SYS_HL1C = 0x2,
    NR_SYS_MAX,
};

#define NRRDR_SUB_SYSTEM(n) (1 << (n))

enum nrrdr_subsys_list_e {
    NRRDR_SYS_CCPU = NRRDR_SUB_SYSTEM(NR_SYS_CCPU),
    NRRDR_SYS_L2HAC = NRRDR_SUB_SYSTEM(NR_SYS_L2HAC),
    NRRDR_SYS_HL1C = NRRDR_SUB_SYSTEM(NR_SYS_HL1C),
    NRRDR_SYS_MAX = NRRDR_SUB_SYSTEM(NR_SYS_MAX)
};

#ifdef ENABLE_BUILD_NRRDR
void dump_save_nr_mandatory_logs(const char *dir_name);
s32 dump_wait_nr_done(bool block);
s32 nrrdr_agent_init(void);
void dump_nrrdr_agent_handle(u32 param);
s32 dump_notify_nr(u32 modid, u32 etype, u64 coreid, char *logpath, pfn_cb_dump_done fndone);

#else
static inline void dump_save_nr_mandatory_logs(const char *dir_name)
{
    return;
}
static inline s32 dump_wait_nr_done(bool block)
{
    return BSP_OK;
}
static inline s32 nrrdr_agent_init(void)
{
    return BSP_OK;
}
static inline void dump_nrrdr_agent_handle(s32 param)
{
    return;
}
static inline s32 dump_notify_nr(u32 modid, u32 etype, u64 coreid, char *logpath, pfn_cb_dump_done fndone)
{
    return BSP_OK;
}

#endif

#endif
