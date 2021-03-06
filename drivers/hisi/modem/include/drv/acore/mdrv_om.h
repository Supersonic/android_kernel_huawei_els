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

#ifndef _MDRV_OM_H_
#define _MDRV_OM_H_

#ifdef _cplusplus
extern "C" {
#endif
/**
 *  @brief   ?????????????????????????????????
 *  @file    mdrv_om.h
 *  @author  xxx
 *  @version v1.0
 *  @date    2019.11.23
 *  @note    ????????????(?????????|????????????|??????)
 * <ul><li>v1.0|2019.05.30|????????????</li></ul>
 *  @since
 */
/**
 * @brief ??????????????????Field ID ????????????
 */
enum om_field_id_e {
#ifndef CONFIG_HISI_BALONG_EXTRA_MODEM
    OM_AP_FIELD_BEGIN = (0x01200000),        /**< Modem AP ??????Field id????????? */
    OM_AP_OSA = (OM_AP_FIELD_BEGIN),         /**< ???????????? Phone??? Modem AP ??????Field id????????? */
    OM_AP_DIAG = (OM_AP_FIELD_BEGIN + 1),    /**< ???????????? Phone??? Modem AP ??????Field id????????? */
    OM_AP_OSA_ICC = (OM_AP_FIELD_BEGIN + 0x2), /**< ???????????? Phone??? Modem AP ??????Field id????????? */
    OM_AP_AT = (OM_AP_FIELD_BEGIN + 0x3),      /**< ???????????? Phone??? Modem AP ??????Field id????????? */
    OM_AP_DMS = (OM_AP_FIELD_BEGIN + 0x4),     /**< Modem AP DMS ?????????Field id */

    OM_AP_FIELD_END,
#else
    OM_AP_FIELD_BEGIN = (0x0c200000),        /**< Modem AP ??????Field id????????? */
    OM_AP_OSA = (OM_AP_FIELD_BEGIN),         /**< Modem AP OSA ?????????Field id */
    OM_AP_DIAG = (OM_AP_FIELD_BEGIN + 1),    /**< Modem AP DIAG ?????????Field id */
    OM_AP_OSA_ICC = (OM_AP_FIELD_BEGIN + 0x2), /**< Modem AP OSA_ICC ?????????Field id */
    OM_AP_DMS = (OM_AP_FIELD_BEGIN + 0x3),     /**< Modem AP DMS ?????????Field id */
    OM_AP_FIELD_END,
#endif

#ifndef __OS_NRCCPU__
    OM_CP_FIELD_BEGIN = (0x02200000),          /**< Modem CP ??????Field id????????? */
    OM_CP_OSA = (OM_CP_FIELD_BEGIN),           /**< Modem CP OSA ?????????Field id */
    OM_CP_MSP_SLEEP = (OM_CP_FIELD_BEGIN + 0x1), /**< Modem CP MSP_SLEEP ?????????Field id */
    OM_CP_TLPS = (OM_CP_FIELD_BEGIN + 0x2),      /**< Modem CP TLPS ?????????Field id */
    OM_CP_FTM_MNTN = (OM_CP_FIELD_BEGIN + 0x3),  /**< Modem CP FTM ?????????Field id */
    OM_CP_GUAS = (OM_CP_FIELD_BEGIN + 0x4),      /**< Modem CP GUAS ?????????Field id */
    OM_CP_DIAG = (OM_CP_FIELD_BEGIN + 0x5),      /**< Modem CP DIAG ?????????Field id */
    OM_CP_GUNAS = (OM_CP_FIELD_BEGIN + 0x6),     /**< Modem CP GUNAS ?????????Field id */
    OM_CP_CPROC = (OM_CP_FIELD_BEGIN + 0x7),     /**< Modem CP CPROC ?????????Field id */
    OM_CP_CAS = (OM_CP_FIELD_BEGIN + 0x8),       /**< Modem CP CAS ?????????Field id */
    OM_CP_CNAS = (OM_CP_FIELD_BEGIN + 0x9),      /**< Modem CP CNAS ?????????Field id */
    OM_CP_MSCC = (OM_CP_FIELD_BEGIN + 0xa),     /**< Modem CP MSCC ?????????Field id */
    OM_CP_CTTF = (OM_CP_FIELD_BEGIN + 0xb),     /**< Modem CP CTTF ?????????Field id */
    OM_CP_USIMM0 = (OM_CP_FIELD_BEGIN + 0xc),   /**< Modem CP USIM0 ?????????Field id */
    OM_CP_USIMM1 = (OM_CP_FIELD_BEGIN + 0xd),   /**< Modem CP USIM1 ?????????Field id */
    OM_CP_TLPS1 = (OM_CP_FIELD_BEGIN + 0xe),    /**< Modem CP TLPS1 ?????????Field id */
    OM_CP_GUPHY = (OM_CP_FIELD_BEGIN + 0xf),    /**< Modem CP GUPHY ?????????Field id */
    OM_CP_RFDSP = (OM_CP_FIELD_BEGIN + 0x10),    /**< Modem CP RFDSP ?????????Field id */
    OM_CP_GUCSLEEP = (OM_CP_FIELD_BEGIN + 0x11), /**< Modem CP GUCSLEEP ?????????Field id */
    OM_CP_TAF = (OM_CP_FIELD_BEGIN + 0x12),      /**< Modem CP TAF ?????????Field id */
    OM_CP_BASTET = (OM_CP_FIELD_BEGIN + 0x13),   /**< Modem CP BASTET ?????????Field id */
    OM_CP_IMS = (OM_CP_FIELD_BEGIN + 0x14),      /**< Modem CP IMS ?????????Field id */
    OM_CP_FIELD_END,
#else
    OM_CP_FIELD_BEGIN = (0x07200000),          /**< Modem NRCCPU ??????Field id????????? */
    OM_CP_OSA = (OM_CP_FIELD_BEGIN),           /**< Modem NRCCPU OSA ?????????Field id */
    OM_CP_MSP_SLEEP = (OM_CP_FIELD_BEGIN + 0x1), /**< Modem NRCCPU MSP_SLEEP ?????????Field id */
    OM_CP_TLPS = (OM_CP_FIELD_BEGIN + 0x2),      /**< Modem NRCCPU TLPS ?????????Field id */
    OM_CP_FTM_MNTN = (OM_CP_FIELD_BEGIN + 0x3),  /**< Modem NRCCPU FTM ?????????Field id */
    OM_CP_GUAS = (OM_CP_FIELD_BEGIN + 0x4),      /**< Modem NRCCPU GUAS ?????????Field id */
    OM_CP_DIAG = (OM_CP_FIELD_BEGIN + 0x5),      /**< Modem NRCCPU DIAG ?????????Field id */
    OM_CP_GUNAS = (OM_CP_FIELD_BEGIN + 0x6),     /**< Modem NRCCPU GUNAS  ?????????Field id */
    OM_CP_CPROC = (OM_CP_FIELD_BEGIN + 0x7),     /**< Modem NRCCPU CPROC  ?????????Field id */
    OM_CP_CAS = (OM_CP_FIELD_BEGIN + 0x8),       /**< Modem NRCCPU CAS ?????????Field id */
    OM_CP_CNAS = (OM_CP_FIELD_BEGIN + 0x9),      /**< Modem NRCCPU CNAS  ?????????Field id */
    OM_CP_MSCC = (OM_CP_FIELD_BEGIN + 0xa),     /**< Modem NRCCPU MSCC ?????????Field id */
    OM_CP_CTTF = (OM_CP_FIELD_BEGIN + 0xb),     /**< Modem NRCCPU CTTF ?????????Field id */
    OM_CP_USIMM0 = (OM_CP_FIELD_BEGIN + 0xc),   /**< Modem NRCCPU USIMM0  ?????????Field id */
    OM_CP_USIMM1 = (OM_CP_FIELD_BEGIN + 0xd),   /**< Modem NRCCPU USIMM1 ?????????Field id */
    OM_CP_TLPS1 = (OM_CP_FIELD_BEGIN + 0xe),    /**< Modem NRCCPU TLPS1 ?????????Field id */
    OM_CP_GUPHY = (OM_CP_FIELD_BEGIN + 0xf),    /**< Modem NRCCPU GUPHY ?????????Field id */
    OM_CP_RFDSP = (OM_CP_FIELD_BEGIN + 0x10),    /**< Modem NRCCPU RFDSP ?????????Field id */
    OM_CP_GUCSLEEP = (OM_CP_FIELD_BEGIN + 0x11), /**< Modem NRCCPU GUCSLEEP ?????????Field id */
    OM_CP_NRNAS = (OM_CP_FIELD_BEGIN + 0x12),    /**< Modem NRCCPU NRNAS ?????????Field id */
    OM_CP_NRRC = (OM_CP_FIELD_BEGIN + 0x13),     /**< Modem NRCCPU NRRC ?????????Field id */
    OM_CP_LL1CSTL = (OM_CP_FIELD_BEGIN + 0x14),  /**< Modem NRCCPU LL1CSTL  ?????????Field id */
    OM_CP_IMS = (OM_CP_FIELD_BEGIN + 0x15),      /**< Modem NRCCPU IMS ?????????Field id */
    OM_CP_FIELD_END,
#endif
    OM_HAC_FIELD_BEGIN = (0x08200000), /**< Modem L2HAC ??????Field id????????? */
    OM_HAC_FIELD_END,
};
#define LOG_DESC_LEN 4
/**
 * @brief Dump??????Head???????????????
 */
typedef struct {
    unsigned char description[LOG_DESC_LEN]; /**<  dump????????????????????????????????????????????????ASCII???  */
    unsigned int dump_size;       /**<  dump?????????????????????????????????byte  */
} dump_log_head_s;

/**
 * @brief Modem????????????
 */
enum {
    DUMP_OUT_VOICE = 0, /**< Modem??????????????? */
    DUMP_IN_VOICE = 1,  /**< Modem???????????? */
    DUMP_VOICE_BUTT,
};
/**
 * @brief ??????????????????????????????
 */
typedef int dump_handle;
/**
 * @brief ??????????????????
 */
typedef void (*dump_hook)(void);

/**
 * @brief ??????Modem???????????????
 * @par ??????:
 * modemap?????????????????????????????????????????????
 *
 * @attention
 * <ul><li> mod_id??????????????????????????????????????????????????????????????????</li></ul>
 * @param[in]  mod_id , ???????????????
 * @param[in]  arg1, ????????????1
 * @param[in]  arg2, ????????????2
 * @param[in]  arg3, ????????????3???????????????
 * @param[in]  arg3_length, ????????????3??????
 *
 * @par ??????:
 * <ul><li>mdrv_om.h???????????????????????????????????????</li></ul>
 */
void mdrv_om_system_error(int modId, int arg1, int arg2, char *arg3_virt_addr, int arg3_length);

/**
 * @brief ??????????????????
 * @par ??????:
 * ???????????????????????????????????????????????????????????????mimidump??????
 *
 * @attention
 * <ul><li> field_id????????????????????????????????????????????????</li></ul>
 * @param[in]  field_id , ???????????????field
 * @param[in]  field_name, ?????????????????????
 * @param[in]  length,  ???????????????????????????
 * @param[in]  version, ????????????????????????
 *
 * @retval NULL,??????????????????
 * @retval != NULL,?????????????????????????????????filedid?????????????????????
 * @see enum om_filed_id_enum
 */
unsigned char *mdrv_om_register_field(unsigned int field_id, char *field_name, unsigned int length,
                                      unsigned short version);

/**
 * @brief ????????????fieldid????????????????????????????????????
 * @par ??????:
 * ??????field id???????????????field id??????????????????????????????
 *
 * @attention
 * <ul><li> field_id????????????????????????????????????????????????</li></ul>
 * @param[in]  field_id , ???????????????field
 *
 * @retval NULL,??????????????????
 * @retval != NULL,??????fieldid???????????????
 * @par ??????:
 * <ul><li>mdrv_om_register_field</li></ul>
 * @see enum om_filed_id_enum
 */
unsigned char *mdrv_om_get_field_addr(unsigned int field_id);

/**
 * @brief ????????????????????????
 * @par ??????:
 * ????????????????????????????????????????????????????????????????????????????????????????????????
 *
 * @param[in]  name , ??????????????????
 * @param[in]  p_func , ?????????????????????
 * @retval -1, ????????????
 * @retval ???0,??????????????????
 */
dump_handle mdrv_om_register_callback(char *name, dump_hook p_func);

#ifdef _cplusplus
}
#endif
#endif
