/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Create: 2012/10/20
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 and
 *  only version 2 as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1) Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2) Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3) Neither the name of Huawei nor the names of its contributors may
 *     be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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

#ifndef __NDISINTERFACE_H__
#define __NDISINTERFACE_H__

/*
 * 1 Include Headfile
 */
#include "ttf_comm.h"

/*
 * 1.1 Cplusplus Announce
 */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*
 * #pragma pack(*)    设置字节对齐方式
 */
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

#define NDIS_ReportEventLog(moduleId, logType, eventID)
#define NDIS_LOG(moduleId, SubMod, level, prinString)
#define NDIS_LOG1(moduleId, SubMod, level, prinString, para1)
#define NDIS_LOG2(moduleId, SubMod, level, prinString, para1, para2)
#define NDIS_LOG3(moduleId, SubMod, level, prinString, para1, para2, para3)
#define NDIS_LOG4(moduleId, SubMod, level, prinString, para1, para2, para3, para4)

#define NDIS_INFO_LOG(moduleId, prinString)
#define NDIS_INFO_LOG1(moduleId, prinString, para1)
#define NDIS_INFO_LOG2(moduleId, prinString, para1, para2)
#define NDIS_INFO_LOG3(moduleId, prinString, para1, para2, para3)
#define NDIS_INFO_LOG4(moduleId, prinString, para1, para2, para3, para4)

#define NDIS_WARNING_LOG(moduleId, prinString)
#define NDIS_WARNING_LOG1(moduleId, prinString, para1)
#define NDIS_WARNING_LOG2(moduleId, prinString, para1, para2)
#define NDIS_WARNING_LOG3(moduleId, prinString, para1, para2, para3)
#define NDIS_WARNING_LOG4(moduleId, prinString, para1, para2, para3, para4)

#define NDIS_ERROR_LOG(moduleId, prinString)
#define NDIS_ERROR_LOG1(moduleId, prinString, para1)
#define NDIS_ERROR_LOG2(moduleId, prinString, para1, para2)
#define NDIS_ERROR_LOG3(moduleId, prinString, para1, para2, para3)
#define NDIS_ERROR_LOG4(moduleId, prinString, para1, para2, para3, para4)

#if (VOS_OS_VER != VOS_WIN32) /* 单字节对齐 */
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

/*
 * 结构名: ETH_Ipfixhdr
 * 协议表格:
 * ASN.1描述:
 * 结构说明: IP固定头
 */
typedef struct {
    VOS_UINT8 ipHdrLen : 4; /* header length */
    VOS_UINT8 ipVer : 4;    /* version */

    VOS_UINT8  serviceType;    /* type of service */
    VOS_UINT16 totalLen;       /* total length */
    VOS_UINT16 identification; /* identification */
    VOS_UINT16 offset;         /* fragment offset field */
    VOS_UINT8  ttl;            /* time to live */
    VOS_UINT8  protocol;       /* protocol */
    VOS_UINT16 checkSum;       /* checksum */
    VOS_UINT32 srcAddr;        /* source address */
    VOS_UINT32 destAddr;       /* dest address */
} ETH_Ipfixhdr;

/*
 * 结构名: ETH_Udphdr
 * 协议表格:
 * ASN.1描述:
 * 结构说明: UDP头
 */
typedef struct {
    VOS_UINT16 srcPort;  /* source port */
    VOS_UINT16 dstPort;  /* dest port */
    VOS_UINT16 len;      /* udp length */
    VOS_UINT16 checksum; /* udp check sum */
} ETH_Udphdr;

/*
 * 结构名: IPV4 UDP包结构
 * 协议表格:
 * ASN.1描述:
 * 结构说明:
 */
typedef struct {
    VOS_UINT8  dstAddr[ETH_MAC_ADDR_LEN];
    VOS_UINT8  srcAddr[ETH_MAC_ADDR_LEN];
    VOS_UINT16 frameType;

    ETH_Ipfixhdr ipHdt;
    VOS_UINT8         rev[4]; /* 4:rsv */
} ETHFRM_Ipv4Pkt;

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of ip_comm.h */


