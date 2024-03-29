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

#ifndef __RFILE_BALONG_H__
#define __RFILE_BALONG_H__

#include "product_config.h"

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/personality.h>
#include <linux/stat.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <linux/statfs.h>
#include <linux/rcupdate.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>

#include "drv_comm.h"
#include "osl_types.h"
#include "osl_sem.h"
#include "osl_list.h"
#include "mdrv_memory.h"
#include "bsp_icc.h"
#include "bsp_rfile.h"
#include "bsp_reset.h"

#pragma pack(4)

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define RFILE_MALLOC(size) kmalloc(size, GFP_KERNEL)
#define RFILE_FREE(ptr) kfree(ptr)
#define DELAY_TIME 1
#define RFILE_WAIT_FOREVER (-1)

#define RFILE_STACK_MAX 12
#define RFILE_PATHLEN_MAX 255

#define RFILE_MAX_SEND_TIMES (10) /* 发送失败时，最大尝试次数 */

#define RFILE_CCORE_ICC_RD_CHAN (ICC_CHN_RFILE << 16 | RFILE_RECV_FUNC_ID)
#define RFILE_CCORE_ICC_WR_CHAN (ICC_CHN_RFILE << 16 | RFILE_RECV_FUNC_ID)

#define RFILE_ICC_PACKET_HEAD_LEN ICC_CHANNEL_PAYLOAD

#define RFILE_LEN_MAX (ICC_RFILE_SIZE - RFILE_ICC_PACKET_HEAD_LEN)

/* RFILE模块写操作依赖于ICC的最大长度 */
#define RFILE_WR_LEN_MAX (ICC_RFILE_SIZE - RFILE_ICC_PACKET_HEAD_LEN - sizeof(struct bsp_rfile_write_req))

/* RFILE模块读操作依赖于ICC的最大长度 */
#define RFILE_RD_LEN_MAX (ICC_RFILE_SIZE - RFILE_ICC_PACKET_HEAD_LEN - sizeof(struct bsp_rfile_read_cnf))

#define RFILE_MNTN_DOT_NUM (100) /* A核记录100个打点信息 */

#define RFILE_MNTN_DATA_LEN (32) /* 可维可测信息数据的最大长度 */

typedef osl_sem_id rfile_sem_id;

/* 操作类型 */
enum _BSP_RFILE_OP_EN {
    EN_RFILE_OP_OPEN = 0,
    EN_RFILE_OP_CLOSE,
    EN_RFILE_OP_WRITE,
    EN_RFILE_OP_WRITE_SYNC,
    EN_RFILE_OP_READ,
    EN_RFILE_OP_SEEK,
    EN_RFILE_OP_TELL,
    EN_RFILE_OP_REMOVE,
    EN_RFILE_OP_MKDIR,
    EN_RFILE_OP_RMDIR,
    EN_RFILE_OP_OPENDIR,
    EN_RFILE_OP_READDIR,
    EN_RFILE_OP_CLOSEDIR,
    EN_RFILE_OP_STAT,
    EN_RFILE_OP_ACCESS,
    EN_RFILE_OP_MASSRD,
    EN_RFILE_OP_MASSWR,
    EN_RFILE_OP_RENAME,
    EN_RFILE_OP_BUTT
};

/* 处理状态 */
enum bsp_rfile_state_en {
    EN_RFILE_IDLE = 0, /* 待处理 */
    EN_RFILE_DOING,    /* 正在处理 */
    EN_RFILE_DONE      /* 处理结束 */
};

/* 处理状态 */
enum bsp_rfile_init_en {
    EN_RFILE_INIT_INVALID = 0,  /* 未初始化 */
    EN_RFILE_INIT_SUSPEND,      /* C核重启过程中 */
    EN_RFILE_INIT_SUSPEND_WAIT, /* C核重启等待文件关闭完成 */
    EN_RFILE_INIT_FINISH        /* 初始化结束 */
};

/* 睡眠唤醒状态 */
enum bsp_rfile_pm_state_en {
    EN_RFILE_WAKEUP_STATE = 0,
    EN_RFILE_SLEEP_STATE
};

struct fp_list {
    s32 fp;
    s8 name[RFILE_NAME_MAX + 1];
    struct list_head stlist;
};
struct dir_list {
    s32 dp;
    s8 name[RFILE_NAME_MAX + 1];
    struct list_head stlist;
};

/* 主控信息 */
struct bsp_rfile_main_stru {
    enum bsp_rfile_init_en init_flag;
    struct task_struct *taskid;
    rfile_sem_id sem_task;    /* 任务信号量 */
    rfile_sem_id sem_closefp; /* 关闭文件句柄完成信号量 */
    struct list_head fplist;  /* C核打开文件链表头 */
    struct list_head dplist;  /* C核打开目录链表头 */
    u8 *data;                 /* 缓存ICC通道读取的数据 */
    u8 *data_send;            /* 准备发送给ICC通道的数据 */
    struct wakeup_source wake_lock;
    u32 pm_state; /* 低功耗状态 */
    u32 op_state; /* 操作状态 */

    u32 lpmstate; /* 低功耗唤醒状态，由低功耗状态唤醒时设置为1 */
};

struct rfile_mntn_dot_stru {
    u32 ptr;
    u32 line[RFILE_MNTN_DOT_NUM];
    u32 slice[RFILE_MNTN_DOT_NUM];
};

struct rfile_mntn_stru {
    struct rfile_mntn_dot_stru stdot;
};

struct bsp_rfile_common_cnf {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 errorno;
    s32 ret;
};

/* open==================================================================== */
struct bsp_rfile_open_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 mode;
    s32 flags;
    s32 namelen;
    u8 data[0];
};

struct bsp_rfile_open_cnf {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 errorno;
    s32 ret; /* 打开文件的描述符或错误码 */
};

/* seek==================================================================== */
struct bsp_rfile_seek_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 fd;
    s32 offset;
    s32 whence;
};

/* read ==================================================================== */
struct bsp_rfile_read_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 fd;
    u32 size;
};

struct bsp_rfile_read_cnf {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 errorno;
    s32 size;
    u8 data[0];
};

/* write ==================================================================== */
struct bsp_rfile_write_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 fd;
    u32 size;
    u8 data[0];
};

/* close ==================================================================== */
struct bsp_rfile_close_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 errorno;
    u32 fd;
};

/* remove==================================================================== */
struct bsp_rfile_remove_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 pathlen;
    u8 data[0];
};

/* tell  ==================================================================== */
struct bsp_rfile_tell_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 fd;
};

/* mkdir ==================================================================== */
struct bsp_rfile_mkdir_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 pathlen;
    s32 mode;
    u8 data[0];
};

/* rmdir ==================================================================== */
struct bsp_rfile_rmdir_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 pathlen;
    u8 data[0];
};

/* opendir ================================================================= */
struct bsp_rfile_opendir_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 namelen;
    u8 data[0];
};

struct bsp_rfile_opendir_cnf {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 errorno;
    s32 dirhandle;
};

/* readdir================================================================== */
struct bsp_rfile_readdir_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 dir;
    u32 count;
};

struct bsp_rfile_readdir_cnf {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 errorno;
    s32 size;
    u8 data[0];
};

/* closedir================================================================== */
struct bsp_rfile_closedir_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 dir;
};

/* stat====================================================================== */
struct bsp_rfile_stat_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 size;
    u8 data[0];
};

struct bsp_rfile_stat_cnf {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 errorno;
    s32 ret;
    struct rfile_stat_stru ststat;
};

/* access===================================================================== */
struct bsp_rfile_access_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 mode;
    s32 pathlen;
    u8 data[0];
};

/* mass read================================================================== */
struct bsp_rfile_massread_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 offset;
    u32 size;
    u32 datalen; /* the length of partition string(include \0) */
    u8 data[0];  /* partition */
};

/* mass write================================================================= */
struct bsp_rfile_masswrite_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 offset;
    u32 size;
    u32 partitionlen; /* the length of partition string(include \0) */
    u32 datalen;      /* the size of all data */
    u8 data[0];       /* partition + write buffer */
};

/* rename====================================================================== */
struct bsp_rfile_rename_req {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    u32 size;
    u8 data[0];
};

struct bsp_rfile_rename_cnf {
    u32 op_type;
    u32 pstlist; /* struct list_head        *pstlist; */
    s32 errorno;
    s32 ret;
};

#pragma pack(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of rfile_balong.h */
