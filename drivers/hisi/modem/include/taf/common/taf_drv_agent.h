/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#ifndef __AT_AGENT_H__
#define __AT_AGENT_H__

#include "at_mn_interface.h"
#include "taf_app_mma.h"
#include "mdrv.h"
#ifdef MODEM_FUSION_VERSION
#include "mdrv_adc.h"
#include "mdrv_version.h"
#include "mdrv_sysboot.h"
#include "observer.h"
#else
#include "observer_at.h"
#endif
#include "at_phy_interface.h"

#include "v_typdef.h"

#ifdef MBB_COMMON
#include "MbbTafFactoryAtComm.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef WIN32
#pragma warning(disable : 4200) /* zero-sized array in struct/union */
#endif

#pragma pack(push, 4)

#define AT_MAX_VERSION_LEN (128) /* ???????????????? */

#define AT_MAX_PDM_VER_LEN (16) /* PDM???????????????? */

#define TAF_DLOAD_INFO_LEN (1024)

#define TAF_AUTH_ID_LEN (50)

#define AT_DLOAD_TASK_DELAY_TIME (500)

#define TAF_MAX_VER_INFO_LEN (6)

#define TAF_MAX_PROPLAT_LEN (63) /* ???????????????????? */

#define TAF_AUTHORITY_LEN (50) /* AUTHORITYVER???????????????? */

/* ????????DRV_GET_VERSION_TIME?????????????????????? */
#define AT_AGENT_DRV_VERSION_TIME_LEN (32)

#define TAF_MAX_FLAFH_INFO_LEN (255)

#define TAF_MAX_GPIO_INFO_LEN (32)

/* ?????????????????? */
#define DRV_AGENT_MAX_MODEL_ID_LEN (31)
#define DRV_AGENT_MAX_REVISION_ID_LEN (31)

/* MS?????????????????????? */
#define DRV_AGENT_MS_SUPPORT_RX_DIV_2100 (0x0001)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_1900 (0x0002)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_1800 (0x0004)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_AWS_1700 (0x0008)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_850 (0x0010)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_800 (0x0020)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_2600 (0x0040)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_900 (0x0080)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_IX_1700 (0x0100)

#define DRV_AGENT_DSP_RF_SWITCH_ON (1)

#define DRV_AGENT_SIMLOCK_MAX_ERROR_TIMES (3) /* Simlock???????????????? */

#define AT_PID_MEM_INFO_LEN (150) /* VOS????????????????????PID???? */

#define AT_KB_TO_BYTES_NUM (1024) /* KB??????????????1024 */

#define NET_DOG_KEY_NUM 3
#if (FEATURE_ON == FEATURE_SECURITY_SHELL)
/* ????SHELL?????????????????? */
#ifndef MBB_COMMON
#define AT_SHELL_PWD_LEN (8)
#endif
#endif

/*
 * ??????????????100????????PAD ??NAND??256MB??????block size??128kB????????2048????
 * ????????????????5%????????????FLASH????2048*5%=102.4????100??????????????????
 * ????????????????100??????????????100??????????????????????????????????
 * ????????<totalNum>??????????????????????
 */
#define DRV_AGENT_NAND_BADBLOCK_MAX_NUM (100)

#define DRV_AGENT_CHIP_TEMP_ERR (65535)

#define DRV_AGENT_HUK_LEN (16)                    /* HUK??128Bits??????, ??????16Bytes */
#define DRV_AGENT_PUBKEY_LEN (1032)               /* ????????????, ??????1032Bytes */
#define DRV_AGENT_RSA_CIPHERTEXT_LEN 256          /* RSA????????, ??????256Bytes */
#define DRV_AGENT_RSA_TLV_CIPHERTEXT_LEN 512      /* RSA????TLV????????, ??????512Bytes */
#define DRV_AGENT_SIMLOCKDATAWRITE_STRU_LEN (516) /* ?????????????????????????????? */
#define DRV_AGENT_PUBKEY_SIGNATURE_LEN (32)       /* ????????SSK???????? */

/* ??????????????CATEGORY??????????????????????:network/network subset/SP/CP */
#define DRV_AGENT_SUPPORT_CATEGORY_NUM (4)

#define DRV_AGENT_SIMLOCK_ENCRYPT_KEYID_NUM 4

#define DRV_AGENT_PH_LOCK_CODE_GROUP_NUM (20) /* ???????????????? */

/* ????????????????????????????CATEGORY??????????????????????:network/network subset/SP/CP */
#define DRV_AGENT_SUPPORT_CATEGORY_NUM_EXTERED (4)
#define DRV_AGENT_PH_LOCK_CODE_GROUP_NUM_EXTERED (10) /* ???????????????????????????????????????? */
#define DRV_AGENT_PH_LOCK_CODE_LEN_EXTERNED (6)       /* ???????????????? */

#define DRV_AGENT_PH_LOCK_CODE_LEN (4) /* ???????????????? */

#define DRV_AGENT_PH_PHYNUM_LEN (16)                                   /* ?????????? */
#define DRV_AGENT_PH_PHYNUM_IMEI_LEN (15)                              /* IMEI?????? */
#define DRV_AGENT_PH_PHYNUM_IMEI_NV_LEN (16)                           /* IMEI??NV?????? */
#define DRV_AGENT_PH_PHYNUM_SN_LEN (16)                                /* SN???? */
#define DRV_AGENT_PH_PHYNUM_SN_NV_LEN (DRV_AGENT_PH_PHYNUM_SN_LEN + 4) /* SN??NV?????? */
#define DRV_AGENT_PORT_PASSWORD_LEN (16)                               /* ?????????????????????? */

#define DRV_AGENT_DH_PUBLICKEY_LEN (128) /* DH??????????????????????Modem?????????? */
#define DRV_AGENT_DH_PRIVATEKEY_LEN (48) /* DH??????????Modem?????????? */

#define DRV_AGENT_HMAC_DATA_LEN (32) /* HMAC?????????? */

#if (FEATURE_ON == FEATURE_PHONE_SC)
#define DRV_AGENT_PIN_CRYPTO_DATA_LEN (16) /* PIN?????????????? */
#define DRV_AGENT_PIN_CRYPTO_IV_LEN (16)   /* PIN IV?????????? */
/* SC??????????KEY ID???? */
#define DRV_AGENT_SUPPORT_SC_KEY_ID_NUM  4
#endif

#define TAF_MAX_CPU_CNT (4)
#define TAF_SRE_ERROR_CODE (0xffffffff)

#define TAF_OTA_SIMLOCK_PUBLIC_ID_NUM 3

#define DRV_AGENT_RSA2048_ENCRYPT_LEN 256
#define DRV_AGENT_NETDOG_AUTH_RANDOM_LEN 16
#define DRV_AGENT_PHY_OM_SAR_MASK_REDUCTION 0x2

/*
 * ????????: ^TSELRF ??????????????????????????
 *           0: DRV_AGENT_TSELRF_SET_ERROR_NO_ERROR ??????????????
 *           1: DRV_AGENT_TSELRF_SET_ERROR_LOADDSP  ??????????????
 */
enum DRV_AGENT_TselrfSetError {
    DRV_AGENT_TSELRF_SET_NO_ERROR = 0,
    DRV_AGENT_TSELRF_SET_LOADDSP_FAIL,

    DRV_AGENT_TSELRF_SET_BUTT
};
typedef VOS_UINT32 DRV_AGENT_TselrfSetErrorUint32;

/*
 * ????????: ????HKADC??????????????????????
 *           0: DRV_AGENT_HKADC_GET_NO_ERROR??????????
 *           1: DRV_AGENT_HKADC_GET_FAIL         ??????????
 */
enum DRV_AGENT_HkadcGetError {
    DRV_AGENT_HKADC_GET_NO_ERROR = 0,
    DRV_AGENT_HKADC_GET_FAIL,

    DRV_AGENT_HKADC_GET_BUTT
};
typedef VOS_UINT32 DRV_AGENT_HkadcGetErrorUint32;

/*
 * ????????: USIMWRITESTUB ??????????????????????????
 *           0: ??????????????
 *           1: ??????????????
 */
enum DRV_AGENT_Error {
    DRV_AGENT_NO_ERROR = 0,
    DRV_AGENT_ERROR,
    DRV_AGENT_PARAM_ERROR,
    DRV_AGENT_CME_ERROR,
    DRV_AGENT_CME_RX_DIV_OTHER_ERR,
    DRV_AGENT_CME_RX_DIV_NOT_SUPPORTED,
    DRV_AGENT_CME_RX_DIV_BAND_ERR,

    DRV_AGENT_BUTT
};
typedef VOS_UINT32 DRV_AGENT_ErrorUint32;


enum DRV_AGENT_MsidQryError {
    DRV_AGENT_MSID_QRY_NO_ERROR              = 0,
    DRV_AGENT_MSID_QRY_READ_PRODUCT_ID_ERROR = 1,
    DRV_AGENT_MSID_QRY_READ_SOFT_VER_ERROR   = 2,
    DRV_AGENT_MSID_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_MsidQryErrorUint32;


enum DRV_AGENT_AppdmverQryError {
    DRV_AGENT_APPDMVER_QRY_NO_ERROR = 0,
    DRV_AGENT_APPDMVER_QRY_ERROR    = 1,

    DRV_AGENT_APPDMVER_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_AppdmverQryErrorUint32;


enum DRV_AGENT_DloadinfoQryError {
    DRV_AGENT_DLOADINFO_QRY_NO_ERROR = 0,
    DRV_AGENT_DLOADINFO_QRY_ERROR    = 1,

    DRV_AGENT_DLOADINFO_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_DloadinfoQryErrorUint32;


enum DRV_AGENT_AuthorityidQryError {
    DRV_AGENT_AUTHORITYID_QRY_NO_ERROR = 0,
    DRV_AGENT_AUTHORITYID_QRY_ERROR    = 1,

    DRV_AGENT_AUTHORITYID_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_AuthorityidQryErrorUint32;


enum DRV_AGENT_GodloadSetError {
    DRV_AGENT_GODLOAD_SET_NO_ERROR = 0,
    DRV_AGENT_GODLOAD_SET_ERROR    = 1,

    DRV_AGENT_GODLOAD_SET_BUTT
};
typedef VOS_UINT32 DRV_AGENT_GodloadSetErrorUint32;

enum DRV_AGENT_HwnatqryQryError {
    DRV_AGENT_HWNATQRY_QRY_NO_ERROR = 0,
    DRV_AGENT_HWNATQRY_QRY_ERROR    = 1,

    DRV_AGENT_HWNATQRY_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_HwnatqryQryErrorUint32;


enum DRV_AGENT_CpuloadQryError {
    DRV_AGENT_CPULOAD_QRY_NO_ERROR = 0,
    DRV_AGENT_CPULOAD_QRY_ERROR    = 1,

    DRV_AGENT_CPULOAD_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_CpuloadQryErrorUint32;


enum DRV_AGENT_MfreelocksizeQryError {
    DRV_AGENT_MFREELOCKSIZE_QRY_NO_ERROR = 0,
    DRV_AGENT_MFREELOCKSIZE_QRY_ERROR    = 1,

    DRV_AGENT_MFREELOCKSIZE_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_MfreelocksizeQryErrorUint32;


enum DRV_AGENT_AuthverQryError {
    DRV_AGENT_AUTHVER_QRY_NO_ERROR = 0,
    DRV_AGENT_AUTHVER_QRY_ERROR    = 1,

    DRV_AGENT_AUTHVER_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_AuthverQryErrorUint32;


enum DRV_AGENT_FlashinfoQryError {
    DRV_AGENT_FLASHINFO_QRY_NO_ERROR = 0,
    DRV_AGENT_FLASHINFO_QRY_ERROR    = 1,

    DRV_AGENT_FLASHINFO_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_FlashinfoQryErrorUint32;


enum DRV_AGENT_PfverQryError {
    DRV_AGENT_PFVER_QRY_NO_ERROR               = 0,
    DRV_AGENT_PFVER_QRY_ERROR_GET_VERSION_TIME = 1,
    DRV_AGENT_PFVER_QRY_ERROR_GET_VERSION      = 2,
    DRV_AGENT_PFVER_QRY_ERROR_ALLOC_MEM        = 3,

    DRV_AGENT_PFVER_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_PfverQryErrorUint32;


enum DRV_AGENT_PersonalizationErr {
    DRV_AGENT_PERSONALIZATION_NO_ERROR = 0,                     /* ???????? */
    DRV_AGENT_PERSONALIZATION_IDENTIFY_FAIL,                    /* ???????????? */
    DRV_AGENT_PERSONALIZATION_SIGNATURE_FAIL,                   /* ???????????? */
    DRV_AGENT_PERSONALIZATION_DK_INCORRECT,                     /* ???????????? */
    DRV_AGENT_PERSONALIZATION_PH_PHYNUM_LEN_ERROR,              /* ?????????????? */
    DRV_AGENT_PERSONALIZATION_PH_PHYNUM_VALUE_ERROR,            /* ???????????? */
    DRV_AGENT_PERSONALIZATION_PH_PHYNUM_TYPE_ERROR,             /* ?????????????? */
    DRV_AGENT_PERSONALIZATION_RSA_ENCRYPT_FAIL,                 /* RSA???????? */
    DRV_AGENT_PERSONALIZATION_RSA_DECRYPT_FAIL,                 /* RSA???????? */
    DRV_AGENT_PERSONALIZATION_GET_RAND_NUMBER_FAIL,             /* ??????????????(crypto_rand) */
    DRV_AGENT_PERSONALIZATION_WRITE_HUK_FAIL,                   /* HUK???????? */
    DRV_AGENT_PERSONALIZATION_FLASH_ERROR,                      /* Flash???? */
    DRV_AGENT_PERSONALIZATION_OTHER_ERROR,                      /* ???????? */
    DRV_AGENT_PERSONALIZATION_OTA_DATA_UPDATE,                  /* OTA?????????????????? */
    DRV_AGENT_PERSONALIZATION_OTA_RAND_CMP_FAIL,                /* OTA?????????????????????? */
    DRV_AGENT_PERSONALIZATION_OTA_SUPPORT_CAT_CHECK_FAIL,       /* OTA???????????????? */
    DRV_AGENT_PERSONALIZATION_OTA_SUPPORT_MODEM_NUM_CHECK_FAIL, /* OTA????????modem???????? */
    DRV_AGENT_PERSONALIZATION_OTA_SUPPORT_DATA_CFG_CHECK_FAIL,  /* OTA???????????????????? */
    DRV_AGENT_PERSONALIZATION_OTA_DATA_LAYER_CHECK_FAIL,        /* OTA?????????????? */
    DRV_AGENT_PERSONALIZATION_OTA_DATA_SIZE_CHECK_FAIL,         /* OTA???????????????????? */
    DRV_AGENT_PERSONALIZATION_INTERGITY_CHECK_FAIL,             /* ?????????????????? */

    DRV_AGENT_PERSONALIZATION_ERR_BUTT
};
typedef VOS_UINT32 DRV_AGENT_PersonalizationErrUint32;

enum DRV_AGENT_EncryptRsaAlgorithm {
    DRV_AGENT_ENCRYPT_ALGORITHM_RSA2048 = 1,
    DRV_AGENT_ENCRYPT_ALGORITHM_RSA_BUTT,
};
typedef VOS_UINT32 DRV_AGENT_EncryptRsaAlgorithmUint32;


enum DRV_AGENT_PersonalizationCategory {
    DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK          = 0x00, /* category:???? */
    DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET   = 0x01, /* category:?????? */
    DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER = 0x02, /* category:??SP */
    DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE        = 0x03, /* category:?????? */
    DRV_AGENT_PERSONALIZATION_CATEGORY_SIM_USIM         = 0x04, /* category:??(U)SIM?? */

    DRV_AGENT_PERSONALIZATION_CATEGORY_BUTT
};
typedef VOS_UINT8 DRV_AGENT_PersonalizationCategoryUint8;


enum DRV_AGENT_PersonalizationIndicator {
    DRV_AGENT_PERSONALIZATION_INDICATOR_INACTIVE = 0x00, /* ?????? */
    DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE   = 0x01, /* ?????? */

    DRV_AGENT_PERSONALIZATION_INDICATOR_BUTT
};
typedef VOS_UINT8 DRV_AGENT_PersonalizationIndicatorUint8;


enum DRV_AGENT_PersonalizationStatus {
    DRV_AGENT_PERSONALIZATION_STATUS_READY = 0x00, /* ???????? */
    DRV_AGENT_PERSONALIZATION_STATUS_PIN   = 0x01, /* ???????????????????????? */
    DRV_AGENT_PERSONALIZATION_STATUS_PUK   = 0x02, /* Block????????????UnBlock?? */

    DRV_AGENT_PERSONALIZATION_STATUS_BUTT
};
typedef VOS_UINT8 DRV_AGENT_PersonalizationStatusUint8;


enum DRV_AGENT_PhPhynumType {
    DRV_AGENT_PH_PHYNUM_IMEI = 0x00, /* IMEI */
    DRV_AGENT_PH_PHYNUM_SN   = 0x01, /* SN */

    DRV_AGENT_PH_PHYNUM_BUTT
};
typedef VOS_UINT8 DRV_AGENT_PhPhynumTypeUint8;


enum DRV_AGENT_PortStatus {
    DRV_AGENT_PORT_STATUS_OFF = 0, /* ???????? */
    DRV_AGENT_PORT_STATUS_ON  = 1, /* ???????? */

    DRV_AGENT_PORT_STATUS_BUTT
};
typedef VOS_UINT32 DRV_AGENT_PortStatusUint32;


enum DRV_AGENT_UsimOperate {
    DRV_AGENT_USIM_OPERATE_DEACT = 0, /* ??????USIM */
    DRV_AGENT_USIM_OPERATE_ACT   = 1, /* ????USIM */

    DRV_AGENT_USIM_OPERATE_BUTT
};
typedef VOS_UINT32 DRV_AGENT_UsimOperateUint32;


enum DRV_AGENT_MsgType {
    /* ???????? */ /* ????ID */ /* ???? */
    /* AT????DRV AGENT?????? */
    /* AT->DRV_AGENT */
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_MSID_QRY_REQ = 0x0000,
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_GCF_IND      = 0x0002,
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_GPIOPL_SET_REQ_STRU */
    DRV_AGENT_GPIOPL_SET_REQ        = 0x000E,
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_GPIOPL_QRY_REQ        = 0x0010,
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_DATALOCK_SET_REQ_STRU */
    DRV_AGENT_DATALOCK_SET_REQ      = 0x0012,
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_TBATVOLT_QRY_REQ      = 0x0014,
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_VERTIME_QRY_REQ       = 0x0016,
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_YJCX_SET_REQ          = 0x0020,
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_YJCX_QRY_REQ          = 0x0022,
    /* _MSGPARSE_Interception  DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_APPDMVER_QRY_REQ      = 0x0024,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_DLOADINFO_QRY_REQ     = 0x0028,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_FLASHINFO_QRY_REQ     = 0x002A,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_AUTHVER_QRY_REQ       = 0x002C,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_AUTHORITYID_QRY_REQ   = 0x0032,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_GODLOAD_SET_REQ       = 0x0034,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_CPULOAD_QRY_REQ       = 0x0038,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_MFREELOCKSIZE_QRY_REQ = 0x003A,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_MEMINFO_QRY_REQ = 0x003B,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_HARDWARE_QRY            = 0x003E, /* ?????????????? */
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_FULL_HARDWARE_QRY       = 0x0040, /* ???????????????????? */
    /* _MSGPARSE_Interception DRV_ELF_AGENT_TMODE_SET_REQ_STRU */
    DRV_AGENT_TMODE_SET_REQ           = 0x0046,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_VERSION_QRY_REQ         = 0x0048,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_PFVER_QRY_REQ           = 0x004E,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_FCHAN_SET_REQ_STRU */
    DRV_AGENT_FCHAN_SET_REQ           = 0x0052,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_PRODTYPE_QRY_REQ        = 0x0060,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_SDREBOOT_REQ            = 0x0062,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_SIMLOCK_SET_REQ_STRU */
    DRV_AGENT_SIMLOCK_SET_REQ = 0x006C,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_IMSICHG_QRY_REQ = 0x0072,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_NVRESTORE_SET_REQ = 0x0074,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_NVRSTSTTS_QRY_REQ = 0x0078,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_CPNN_TEST_REQ     = 0x007A,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_NVBACKUP_SET_REQ  = 0x007C,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_CPNN_QRY_REQ      = 0x007E,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_NVRESTORE_MANU_DEFAULT_REQ = 0x0080,
    /* _MSGPARSE_Interception SPY_ELF_TEMP_THRESHOLD_PARA_STRU */
    DRV_AGENT_ADC_SET_REQ = 0x0082,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_TSELRF_SET_REQ_STRU */
    DRV_AGENT_TSELRF_SET_REQ   = 0x0084,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_HKADC_GET_REQ    = 0x0086,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_TBAT_QRY_REQ     = 0x0088,
    /* _MSGPARSE_Interception TAF_ELF_CUSTOM_CARDLOCK_STATUS_STRU */
    DRV_AGENT_SIMLOCK_NV_SET_REQ = 0x008c,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_SPWORD_SET_REQ_STRU */
    DRV_AGENT_SPWORD_SET_REQ = 0x008e,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_PSTANDBY_REQ_STRU */
    DRV_AGENT_PSTANDBY_SET_REQ = 0x0090,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_NVBACKUPSTAT_QRY_REQ = 0x0092,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_NANDBBC_QRY_REQ      = 0x0094,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_NANDVER_QRY_REQ      = 0x0096,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_CHIPTEMP_QRY_REQ     = 0x0098,

    /* _MSGPARSE_Interception DRV_ELF_AGENT_HUK_SET_REQ_STRU  */
    DRV_AGENT_HUK_SET_REQ = 0x009A,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_FACAUTHPUBKEY_SET_REQ_STRU */
    DRV_AGENT_FACAUTHPUBKEY_SET_REQ = 0x009C,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_IDENTIFYSTART_SET_REQ = 0x009E,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_IDENTIFYEND_SET_REQ_STRU */
    DRV_AGENT_IDENTIFYEND_SET_REQ = 0x00A0,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_PHONESIMLOCKINFO_QRY_REQ = 0x00A4,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_SIMLOCKDATAREAD_QRY_REQ  = 0x00A6,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_PHONEPHYNUM_SET_REQ_STRU */
    DRV_AGENT_PHONEPHYNUM_SET_REQ = 0x00A8,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_INFORRS_SET_REQ = 0x0100,
    /* _MSGPARSE_Interception TAF_ELF_CUSTOM_SIM_LOCK_MAX_TIMES_STRU */
    DRV_AGENT_MAX_LOCK_TIMES_SET_REQ = 0x0102,

    /* _MSGPARSE_Interception DRV_ELF_AGENT_AP_SIMST_SET_REQ_STRU */
    DRV_AGENT_AP_SIMST_SET_REQ = 0x0104,

    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_SWVER_SET_REQ         = 0x0106,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_QRY_CCPU_MEM_INFO_REQ = 0x0108,




    /* _MSGPARSE_Interception DRV_ELF_AGENT_SIMLOCKDATAWRITEEX_SET_REQ_STRU */
    DRV_AGENT_SIMLOCKWRITEEX_SET_REQ     = 0x0112,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_SIMLOCKDATAREADEX_READ_REQ_STRU */
    DRV_AGENT_SIMLOCKDATAREADEX_READ_REQ = 0x0114,
    /* AT->DRV_AGENT */
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_GETMODEMSCID_QRY_REQ = 0x0132,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSG_REQ_STRU */
    DRV_AGENT_ADCTEMP_QRY_REQ = 0x0148,

    /* DRV AGENT????AT?????? */
    /* DRV_AGENT->AT */
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MSID_QRY_CNF_STRU */
    DRV_AGENT_MSID_QRY_CNF = 0x0001,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_GPIOPL_SET_CNF_STRU */
    DRV_AGENT_GPIOPL_SET_CNF   = 0x000D,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_GPIOPL_QRY_CNF_STRU */
    DRV_AGENT_GPIOPL_QRY_CNF   = 0x000F,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_DATALOCK_SET_CNF_STRU */
    DRV_AGENT_DATALOCK_SET_CNF = 0x0011,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_TBATVOLT_QRY_CNF_STRU */
    DRV_AGENT_TBATVOLT_QRY_CNF = 0x0013,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_VERSION_TIME_STRU */
    DRV_AGENT_VERTIME_QRY_CNF  = 0x0015,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_YJCX_SET_CNF_STRU */
    DRV_AGENT_YJCX_SET_CNF     = 0x001F,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_YJCX_QRY_CNF_STRU */
    DRV_AGENT_YJCX_QRY_CNF     = 0x0021,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_APPDMVER_QRY_CNF_STRU */
    DRV_AGENT_APPDMVER_QRY_CNF = 0x0023,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_DLOADINFO_QRY_CNF_STRU */
    DRV_AGENT_DLOADINFO_QRY_CNF = 0x0027,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_FLASHINFO_QRY_CNF_STRU */
    DRV_AGENT_FLASHINFO_QRY_CNF = 0x0029,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_AUTHVER_QRY_CNF_STRU */
    DRV_AGENT_AUTHVER_QRY_CNF = 0x002B,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_AUTHORITYID_QRY_CNF_STRU */
    /* _MSGPARSE_Interception DRV_ELF_AGENT_AUTHORITYID_QRY_CNF_STRU */
    DRV_AGENT_AUTHORITYID_QRY_CNF = 0x0031,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_GODLOAD_SET_CNF_STRU */
    DRV_AGENT_GODLOAD_SET_CNF = 0x0033,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_HWNATQRY_QRY_CNF_STRU */
    DRV_AGENT_HWNATQRY_QRY_CNF      = 0x0035,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_CPULOAD_QRY_CNF_STRU */
    DRV_AGENT_CPULOAD_QRY_CNF       = 0x0037,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_MFREELOCKSIZE_QRY_CNF_STRU */
    DRV_AGENT_MFREELOCKSIZE_QRY_CNF = 0x0039,
    /* _MSGPARSE_Interception DRV_AGENT_MeminfoQryRsp */
    DRV_AGENT_MEMINFO_QRY_CNF = 0x003A,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_HARDWARE_QRY_CNF_STRU */
    DRV_AGENT_HARDWARE_QRY_RSP        = 0x003D, /* ?????????????? */
    /* _MSGPARSE_Interception DRV_ELF_AGENT_FULL_HARDWARE_QRY_CNF_STRU */
    DRV_AGENT_FULL_HARDWARE_QRY_RSP   = 0x003F, /* ???????????????????? */
    /* _MSGPARSE_Interception DRV_ELF_AGENT_VERSION_QRY_CNF_STRU */
    DRV_AGENT_VERSION_QRY_CNF         = 0x0047,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_PFVER_QRY_CNF_STRU */
    DRV_AGENT_PFVER_QRY_CNF    = 0x004D,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_FCHAN_SET_CNF_STRU */
    DRV_AGENT_FCHAN_SET_CNF    = 0x0053,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_PRODTYPE_QRY_CNF_STRU */
    DRV_AGENT_PRODTYPE_QRY_CNF = 0x0061,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_QRY_RXDIV_CNF_STRU */
    DRV_AGENT_RXDIV_QRY_CNF    = 0x0069,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_SET_SIMLOCK_CNF_STRU */
    DRV_AGENT_SIMLOCK_SET_CNF  = 0x006B,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_IMSICHG_QRY_CNF_STRU */
    DRV_AGENT_IMSICHG_QRY_CNF = 0x0071,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_NVRESTORE_RST_STRU */
    DRV_AGENT_NVRESTORE_SET_CNF = 0x0073,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_INFORBU_SET_CNF_STRU */
    DRV_AGENT_INFORBU_SET_CNF = 0x0075,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_NVRESTORE_RST_STRU */
    DRV_AGENT_NVRSTSTTS_QRY_CNF = 0x0077,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_CPNN_TEST_CNF_STRU */
    DRV_AGENT_CPNN_TEST_CNF = 0x0079,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_NVBACKUP_SET_CNF_STRU */
    DRV_AGENT_NVBACKUP_SET_CNF = 0x007B,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_CPNN_QRY_CNF_STRU */
    DRV_AGENT_CPNN_QRY_CNF = 0x007D,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_NVRESTORE_RST_STRU */
    DRV_AGENT_NVRESTORE_MANU_DEFAULT_CNF = 0x007F,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_ADC_SET_CNF_STRU */
    DRV_AGENT_ADC_SET_CNF                = 0x0081,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_TSELRF_SET_CNF_STRU */
    DRV_AGENT_TSELRF_SET_CNF = 0x0083,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_HKADC_GET_CNF_STRU */
    DRV_AGENT_HKADC_GET_CNF = 0x0085,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_TBAT_QRY_CNF_STRU */
    DRV_AGENT_TBAT_QRY_CNF  = 0x0087,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_SPWORD_SET_CNF_STRU */
    DRV_AGENT_SPWORD_SET_CNF   = 0x008B,
    /* DRV_AGENT->AT */
    /* _MSGPARSE_Interception DRV_ELF_AGENT_NVBACKUPSTAT_QRY_CNF_STRU */
    DRV_AGENT_NVBACKUPSTAT_QRY_CNF = 0x008D,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_NANDBBC_QRY_CNF_STRU */
    DRV_AGENT_NANDBBC_QRY_CNF = 0x008F,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_NANDVER_QRY_CNF_STRU */
    DRV_AGENT_NANDVER_QRY_CNF = 0x0091,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_CHIPTEMP_QRY_CNF_STRU */
    DRV_AGENT_CHIPTEMP_QRY_CNF = 0x0093,

    /* _MSGPARSE_Interception DRV_ELF_AGENT_HUK_SET_CNF_STRU */
    DRV_AGENT_HUK_SET_CNF = 0x0095,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_FACAUTHPUBKEY_SET_CNF_STRU */
    DRV_AGENT_FACAUTHPUBKEY_SET_CNF = 0x0097,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_IDENTIFYSTART_SET_CNF_STRU */
    DRV_AGENT_IDENTIFYSTART_SET_CNF = 0x0099,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_IDENTIFYEND_SET_CNF_STRU */
    DRV_AGENT_IDENTIFYEND_SET_CNF = 0x009B,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_SIMLOCKDATAWRITE_SET_CNF_STRU */
    DRV_AGENT_SIMLOCKDATAWRITE_SET_CNF = 0x009D,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_PHONESIMLOCKINFO_QRY_CNF_STRU */
    DRV_AGENT_PHONESIMLOCKINFO_QRY_CNF = 0x009F,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_SIMLOCKDATAREAD_QRY_CNF_STRU */
    DRV_AGENT_SIMLOCKDATAREAD_QRY_CNF = 0x00A1,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_PHONEPHYNUM_SET_CNF_STRU */
    DRV_AGENT_PHONEPHYNUM_SET_CNF = 0x00A3,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_OPWORD_SET_CNF_STRU */
    DRV_AGENT_OPWORD_SET_CNF   = 0x00AD,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_INFORRS_SET_CNF_STRU */
    DRV_AGENT_INFORRS_SET_CNF = 0x0101,

    /* _MSGPARSE_Interception DRV_ELF_AGENT_MAX_LOCK_TMS_SET_CNF_STRU */
    DRV_AGENT_MAX_LOCK_TIMES_SET_CNF = 0x0103,

    /* _MSGPARSE_Interception DRV_ELF_AGENT_AP_SIMST_SET_CNF_STRU */
    DRV_AGENT_AP_SIMST_SET_CNF = 0x0105,

    /* _MSGPARSE_Interception DRV_ELF_AGENT_SWVER_SET_CNF_STRU */
    DRV_AGENT_SWVER_SET_CNF = 0x0107,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_NVMANUFACTUREEXT_SET_CNF_STRU */
    DRV_AGENT_NVMANUFACTUREEXT_SET_CNF = 0x010B,
    /* DRV_AGENT->AT */
    /* _MSGPARSE_Interception DRV_ELF_AGENT_SWVER_SET_CNF_STRU */
    DRV_AGENT_QRY_CCPU_MEM_INFO_CNF = 0x0111,

    /* _MSGPARSE_Interception DRV_ELF_AGENT_SIMLOCKDATAWRITEEX_SET_CNF_STRU */
    DRV_AGENT_SIMLOCKWRITEEX_SET_CNF       = 0x0113,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_SIMLOCKDATAREADEX_READ_CNF_STRU */
    DRV_AGENT_SIMLOCKDATAREADEX_SET_CNF    = 0x0115,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_PHONESIMLOCKINFO_QRY_CNF_NEW_STRU */
    DRV_AGENT_PHONESIMLOCKINFO_QRY_NEW_CNF = 0x0117,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_GETSIMLOCKENCRYPTID_QRY_CNF_STRU */
    DRV_AGENT_GETMODEMSCID_QRY_CNF = 0x0135,
    /* _MSGPARSE_Interception DRV_ELF_AGENT_ADC_QRY_CNF_STRU */
    DRV_AGENT_HKADC_QRY_CNF = 0x0147,

    /* BUTT->BUTT */
    /* _MSGPARSE_Interception DRV_AGENT_SwverSetCnf */
    DRV_AGENT_MSG_TYPE_BUTT = 0xFFFF
};
typedef VOS_UINT32 DRV_AGENT_MsgTypeUint32;


enum AT_DEVICE_CmdRatMode {
    AT_RAT_MODE_WCDMA = 0,
    AT_RAT_MODE_CDMA,
    AT_RAT_MODE_TDSCDMA,
    AT_RAT_MODE_GSM,
    AT_RAT_MODE_EDGE,
    AT_RAT_MODE_AWS,
    AT_RAT_MODE_FDD_LTE,
    AT_RAT_MODE_TDD_LTE,
    AT_RAT_MODE_WIFI,
    AT_RAT_MODE_NR,
#if (FEATURE_LTEV == FEATURE_ON)
    AT_RAT_MODE_LTEV = 10,
#endif
    AT_RAT_MODE_BUTT
};
typedef VOS_UINT8 AT_DEVICE_CmdRatModeUint8;

/* ????????????????????  */
enum AT_MEMQUERY_Para {
    AT_MEMQUERY_VOS = 0, /* ????VOS?????????????????????????? */
    AT_MEMQUERY_TTF,     /* ????TTF?????????????????????????? */
    AT_MEMQUERY_BUTT
};
typedef VOS_UINT32 AT_MEMQUERY_ParaUint32;


enum DRV_AGENT_DhKeyType {
    DRV_AGENT_DH_KEY_SERVER_PUBLIC_KEY = 0x00, /* ?????????? */
    DRV_AGENT_DH_KEY_MODEM_PUBLIC_KEY  = 0x01, /* MODEM?????? */
    DRV_AGENT_DH_KEY_MODEM_PRIVATE_KEY = 0x02, /* MODEM?????? */

    DRV_AGENT_DH_KEY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_DhKeyTypeUint32;


enum DRV_AGENT_HvpdhErr {
    DRV_AGENT_HVPDH_NO_ERROR      = 0x00, /* ???????? */
    DRV_AGENT_HVPDH_AUTH_UNDO     = 0x01, /* ?????????????????????? */
    DRV_AGENT_HVPDH_NV_READ_FAIL  = 0x02, /* NV???????? */
    DRV_AGENT_HVPDH_NV_WRITE_FAIL = 0x03, /* NV???????? */
    DRV_AGENT_HVPDH_OTHER_ERROR   = 0x04, /* ???????? */

    DRV_AGENT_HVPDH_ERR_BUTT
};
typedef VOS_UINT32 DRV_AGENT_HvpdhErrUint32;


enum AT_SimlockType {
    AT_SIMLOCK_TYPE_FAC = 0, /* ?????????????? */
    AT_SIMLOCK_TYPE_NW  = 1, /* ???????????? */
    AT_SIMLOCK_TYPE_OTA = 2,
    AT_SIMLOCK_TYPE_BUTT
};
typedef VOS_UINT8 AT_SimlockTypeUint8;


typedef struct {
    /* ????????????????????????????(??????ms),???????? 0~65535,??????5000?? */
    VOS_UINT32 standbyTime;
    /* ????????PC??????????????????????????????????????????????????ms?????????????? 0~65535????????500?? */
    VOS_UINT32 switchTime;
} DRV_AGENT_PstandbyReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT8  content[4]; /* ????????????????????????TAF_DEFAULT_CONTENT_LEN?? */
} DRV_AGENT_Msg;


enum GAS_AT_Cmd {
    GAS_AT_CMD_GAS_STA             = 0,  /* GAS???? */
    GAS_AT_CMD_NCELL               = 1,  /* ???????? */
    GAS_AT_CMD_SCELL               = 2,  /* ???????????? */
    GAS_AT_CMD_MEASURE_DOWN        = 3,  /* ???????? */
    GAS_AT_CMD_MEASURE_UP          = 4,  /* ???????? */
    GAS_AT_CMD_FREQ_LOCK           = 5,  /* ???????????????????? */
    GAS_AT_CMD_FREQ_UNLOCK         = 6,  /* ???? */
    GAS_AT_CMD_FREQ_SPEC_SEARCH    = 7,  /* ???????????? */
    GAS_AT_CMD_ADD_FORB_CELL       = 8,  /* ???????????? */
    GAS_AT_CMD_DEL_FORB_CELL       = 9,  /* ???????????? */
    GAS_AT_CMD_DEL_HISTORY_SI_DATA = 10, /* ???????????? */
    GAS_AT_CMD_BUTT
};

typedef VOS_UINT8 GAS_AT_CmdUint8;


typedef struct {
    GAS_AT_CmdUint8 cmd;
    VOS_UINT8       paraNum;
    VOS_UINT16      reserve;
    VOS_UINT32      para[10];
} GAS_AT_CmdPara;


typedef struct {
    AT_APPCTRL                   atAppCtrl;                                  /* AT???????????? */
    DRV_AGENT_MsidQryErrorUint32 result;                                     /* ???????????? */
    VOS_INT8                     modelId[TAF_MAX_MODEL_ID_LEN + 1];          /* ???????? */
    VOS_INT8                     softwareVerId[TAF_MAX_REVISION_ID_LEN + 1]; /* ???????????? */
    VOS_UINT8                    imei[TAF_PH_IMEI_LEN];
} DRV_AGENT_MsidQryCnf;


typedef struct {
    VOS_UINT8 loadDspMode;
    VOS_UINT8 deviceRatMode;
} DRV_AGENT_TselrfSetReq;


typedef struct {
    AT_APPCTRL                       atAppCtrl;
    DRV_AGENT_AppdmverQryErrorUint32 result;
    VOS_INT8                         pdmver[AT_MAX_PDM_VER_LEN + 1];
    VOS_UINT8                        reserved1[3];
} DRV_AGENT_AppdmverQryCnf;


typedef struct {
    AT_APPCTRL                        atAppCtrl;
    DRV_AGENT_DloadinfoQryErrorUint32 result;
    VOS_UCHAR                         dlodInfo[TAF_DLOAD_INFO_LEN];
} DRV_AGENT_DloadinfoQryCnf;


typedef struct {
    AT_APPCTRL                          atAppCtrl;
    DRV_AGENT_AuthorityidQryErrorUint32 result;
    VOS_UINT8                           authorityId[TAF_AUTH_ID_LEN + 1];
    VOS_UINT8                           reserved1[1];
} DRV_AGENT_AuthorityidQryCnf;


typedef struct {
    VOS_UINT8 pfVer[TAF_MAX_PROPLAT_LEN + 1];
    VOS_CHAR  verTime[AT_AGENT_DRV_VERSION_TIME_LEN];
} DRV_AGENT_PfverInfo;


typedef struct {
    AT_APPCTRL                    atAppCtrl;
    DRV_AGENT_PfverQryErrorUint32 result;
    DRV_AGENT_PfverInfo           pfverInfo;
} DRV_AGENT_PfverQryCnf;

typedef struct {
    AT_APPCTRL                      atAppCtrl;
    DRV_AGENT_GodloadSetErrorUint32 result;
} DRV_AGENT_GodloadSetCnf;


typedef struct {
    AT_APPCTRL                       atAppCtrl;
    DRV_AGENT_HwnatqryQryErrorUint32 result;
    VOS_UINT                         netMode;
} DRV_AGENT_HwnatqryQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 curCpuLoadCnt;
    VOS_UINT32 curCpuLoad[TAF_MAX_CPU_CNT];
} DRV_AGENT_CpuloadQryCnf;


typedef struct {
    AT_APPCTRL                            atAppCtrl;
    DRV_AGENT_MfreelocksizeQryErrorUint32 result;
    VOS_INT32                             maxFreeLockSize;
} DRV_AGENT_MfreelocksizeQryCnf;


typedef struct {
    VOS_UINT32 blockCount;  /* Block???? */
    VOS_UINT32 pageSize;    /* page?????? */
    VOS_UINT32 pgCntPerBlk; /* ????Block????page???? */
} DRV_AGENT_DloadFlash;


typedef struct {
    AT_APPCTRL                        atAppCtrl;
    DRV_AGENT_FlashinfoQryErrorUint32 result;
    DRV_AGENT_DloadFlash              flashInfo;
} DRV_AGENT_FlashinfoQryCnf;


typedef struct {
    AT_APPCTRL                      atAppCtrl;
    DRV_AGENT_AuthverQryErrorUint32 result;
    VOS_UINT32                      simLockVersion;
} DRV_AGENT_AuthverQryCnf;

typedef struct {
    VOS_UINT16 dspBand;      /* DSP???????????? */
    VOS_UINT8  rxOnOff;      /* Rx on off?? */
    VOS_UINT8  reserve;      /* ?????? */
    VOS_UINT32 setLowBands;  /* ???????????????????????? */
    VOS_UINT32 setHighBands; /* ???????????????????????? */
} AT_DRV_AgentRxdivSet;

typedef struct {
    AT_APPCTRL atAppCtrl;   /* ?????? */
    VOS_UINT32 result;      /* ???????? */
    VOS_UINT8  rxOnOff;     /* RX???????????? */
    VOS_UINT8  reserve;     /* ?????? */
    VOS_UINT16 setDivBands; /* ????????BAND */
} DRV_AGENT_AtRxdivCnf;

typedef struct {
    AT_APPCTRL atAppCtrl;     /* ?????? */
    VOS_UINT32 result;        /* ???????? */
    VOS_UINT16 drvDivBands;   /* ??????BAND */
    VOS_UINT16 curBandSwitch; /* ??????????BAND */
} DRV_AGENT_QryRxdivCnf;


typedef struct {
    VOS_UINT32 pwdLen;
    VOS_UINT8  pwd[TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX + 4];
} DRV_AGENT_SimlockSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl; /* ?????? */
    VOS_UINT32 result;    /* ???????? */
} DRV_AGENT_SetSimlockCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;                             /* ?????? */
    VOS_UINT32 result;                                /* ???????? */
    VOS_UINT8  hwVer[DRV_AGENT_MAX_HARDWARE_LEN + 1]; /* ?????????? */
} DRV_AGENT_HardwareQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;                                     /* ?????? */
    VOS_UINT32 result;                                        /* ???????? */
    VOS_UINT8  modelId[DRV_AGENT_MAX_MODEL_ID_LEN + 1];       /* MODE ID?? */
    VOS_UINT8  revisionId[DRV_AGENT_MAX_REVISION_ID_LEN + 1]; /* ?????????? */
    VOS_UINT8  hwVer[DRV_AGENT_MAX_HARDWARE_LEN + 1];         /* ?????????? */
} DRV_AGENT_FullHardwareQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl; /* ?????? */
    VOS_UINT32 result;    /* ???????? */
} DRV_AGENT_NvrestoreRst;

#define AT_SD_DATA_UNIT_LEN (512)
#define AT_GPIOPL_MAX_LEN (20)


typedef struct {
    VOS_UINT32 gpioOper;
    VOS_UINT8  gpiopl[AT_GPIOPL_MAX_LEN];

} DRV_AGENT_GpioplSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;

} DRV_AGENT_GpioplSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
    VOS_UINT8  gpiopl[AT_GPIOPL_MAX_LEN];

} DRV_AGENT_GpioplQryCnf;


typedef struct {
    VOS_UINT8 pwd[TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX + 1];

} DRV_AGENT_DatalockSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail; /* DATALOCK???????????? */

} DRV_AGENT_DatalockSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
    VOS_INT32  batVol;

} DRV_AGENT_TbatvoltQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;

} DRV_AGENT_TmodeSetCnf;


enum DRV_AGENT_VersionQryError {
    DRV_AGENT_VERSION_QRY_NO_ERROR      = 0,
    DRV_AGENT_VERSION_QRY_VERTIME_ERROR = 1,

    DRV_AGENT_VERSION_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_VersionQryErrorUint32;


typedef struct {
    AT_APPCTRL                      atAppCtrl;
    TAF_PH_RevisionId               softVersion;                            /* ?????????? */
    TAF_PH_HwVer                    fullHwVer;                              /* ?????????????? */
    TAF_PH_HwVer                    interHwVer;                             /* ?????????????? */
    TAF_PH_ModelId                  modelId;                                /* ????????ID */
    TAF_PH_ModelId                  interModelId;                           /* ????????ID */
    TAF_PH_CdromVer                 isoVer;                                 /* ?????????????? */
    DRV_AGENT_VersionQryErrorUint32 result;                                 /* ?????????? */
    VOS_CHAR                        verTime[AT_AGENT_DRV_VERSION_TIME_LEN]; /* ???????? */

} DRV_AGENT_VersionQryCnf;

#ifndef MBB_COMMON

enum AT_FEATURE_DrvType {
    AT_FEATURE_LTE = 0,
    AT_FEATURE_HSPAPLUS,
    AT_FEATURE_HSDPA,
    AT_FEATURE_HSUPA,
    AT_FEATURE_DIVERSITY,
    AT_FEATURE_UMTS,
    AT_FEATURE_EVDO,
    AT_FEATURE_EDGE,
    AT_FEATURE_GPRS,
    AT_FEATURE_GSM,
    AT_FEATURE_CDMA,
    AT_FEATURE_WIMAX,
    AT_FEATURE_WIFI,
    AT_FEATURE_GPS,
    AT_FEATURE_TDSCDMA,

    AT_FEATURE_NR,
#if (FEATURE_LTEV == FEATURE_ON)
    AT_FEATURE_LTEV = 16,
#endif
};
typedef VOS_UINT32 AT_FEATURE_DrvTypeUint32;
#endif


typedef struct {
    VOS_UINT8 powerClass;       /* UE???????? */
    VOS_UINT8 txRxFreqSeparate; /* Tx/Rx ???????? */
    VOS_UINT8 reserve1[2];      /* 4?????????????? */
} AT_RF_Capa;


typedef struct {
    VOS_UINT32 hspaStatus; /* 0??????????,????DPA??UPA??????;1???????? */
    VOS_UINT8  macEhsSupport;
    VOS_UINT8  hsdschPhyCat;
    VOS_UINT8  reserve1[2];       /* ??????????????ulHspaStatus???????????????????????? */
    AT_RF_Capa rfCapa;            /* RF ???????? */
    VOS_UINT8  dlSimulHsDschCfg;  /* ENUMERATED  OPTIONAL */
    VOS_UINT8  asRelIndicator;    /* Access Stratum Release Indicator */
    VOS_UINT8  hsdschPhyCategory; /* ????HS-DSCH???????????????? */
    VOS_UINT8  edchSupport;       /* ????????EDCH?????? */
    VOS_UINT8  edchPhyCategory;   /* ????UPA?????? */
    VOS_UINT8  fdpchSupport;      /* ????????FDPCH?????? */
    VOS_UINT8  v690Container;     /* V690Container???? */
    VOS_UINT8  hsdschSupport;     /* ????????enHSDSCHSupport?????? */
} AT_UE_Capa;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
} DRV_AGENT_SecubootSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
    VOS_UINT8  secuBootEnable;
    VOS_UINT8  reserve1[3]; /* 4?????????????? */
} DRV_AGENT_SecubootQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
    VOS_UINT8  secuBootFeature;
    VOS_UINT8  reserve1[7]; /* 4?????????????? */
} DRV_AGENT_SecubootfeatureQryCnf;


enum AT_DEVICE_CmdBand {
    AT_BAND_2100M = 0,
    AT_BAND_1900M,
    AT_BAND_1800M,
    AT_BAND_1700M,
    AT_BAND_1600M,
    AT_BAND_1500M,
    AT_BAND_900M,
    AT_BAND_850M,
    AT_BAND_800M,
    AT_BAND_450M,
    AT_BAND_2600M,
    AT_BAND_1X_BC0,
    AT_BAND_WIFI = 15,
    AT_BAND_BUTT
};
typedef VOS_UINT8 AT_DEVICE_CmdBandUint8;


typedef struct {
    VOS_UINT16 ulArfcn; /* ??????Channel No */
    VOS_UINT16 dlArfcn; /* ??????Channel No */
    VOS_UINT16 dspBand; /* DSP???????????? */
    VOS_UINT8  reserved[2];
} AT_DSP_BandArfcn;


typedef struct {
    VOS_UINT16 arfcnMin;
    VOS_UINT16 arfcnMax;
} AT_ARFCN_Range;


enum AT_TMODE {
    AT_TMODE_NORMAL = 0, /* ????????,???????????? */
    AT_TMODE_FTM,        /* ??????????,???????? */
    AT_TMODE_UPGRADE,    /* ????????,?????????????? */
    AT_TMODE_RESET,      /* ????????  */
    AT_TMODE_POWEROFF,
    AT_TMODE_SIGNALING = 11,
    AT_TMODE_OFFLINE,
#if ((FEATURE_ON == FEATURE_LTE) || (FEATURE_ON == FEATURE_UE_MODE_TDS))
    AT_TMODE_F_NONESIGNAL   = 13,
    AT_TMODE_SYN_NONESIGNAL = 14,
    AT_TMODE_SET_SECONDARY  = 15,
    AT_TMODE_GU_BT          = 16,
    AT_TMODE_COMM_CT        = 19,
#endif
    AT_TMODE_BUTT
};
typedef VOS_UINT8 TAF_PH_TmodeUint8;


typedef struct {
    VOS_UINT8                 loadDspMode;
    VOS_UINT8                 currentDspMode;
    VOS_UINT16                channelNo;
    AT_DEVICE_CmdRatModeUint8 deviceRatMode;
    AT_DEVICE_CmdBandUint8    deviceAtBand;
    VOS_UINT8                 reserved[2];
    VOS_BOOL                  dspLoadFlag;
    AT_DSP_BandArfcn          dspBandArfcn;
} DRV_AGENT_FchanSetReq;


enum DRV_AGENT_FchanSetError {
    DRV_AGENT_FCHAN_SET_NO_ERROR           = 0,
    DRV_AGENT_FCHAN_BAND_NOT_MATCH         = 1,
    DRV_AGENT_FCHAN_BAND_CHANNEL_NOT_MATCH = 2,
    DRV_AGENT_FCHAN_OTHER_ERR              = 3,

    DRV_AGENT_FCHAN_SET_BUTT
};
typedef VOS_UINT32 DRV_AGENT_FchanSetErrorUint32;


typedef struct {
    AT_APPCTRL                    atAppCtrl;
    DRV_AGENT_FchanSetReq         fchanSetReq;
    DRV_AGENT_FchanSetErrorUint32 result;

} DRV_AGENT_FchanSetCnf;


#define AT_MS_SUPPORT_RX_DIV_2100 0x0001
#define AT_MS_SUPPORT_RX_DIV_1900 0x0002
#define AT_MS_SUPPORT_RX_DIV_1800 0x0004
#define AT_MS_SUPPORT_RX_DIV_AWS_1700 0x0008
#define AT_MS_SUPPORT_RX_DIV_850 0x0010
#define AT_MS_SUPPORT_RX_DIV_800 0x0020
#define AT_MS_SUPPORT_RX_DIV_2600 0x0040
#define AT_MS_SUPPORT_RX_DIV_900 0x0080
#define AT_MS_SUPPORT_RX_DIV_IX_1700 0x0100


enum AT_DSP_RfOnOff {
    AT_DSP_RF_SWITCH_OFF = 0,
    AT_DSP_RF_SWITCH_ON,
    AT_DSP_RF_SWITCH_BUTT
};
typedef VOS_UINT8 AT_DSP_RfOnOffUint8;


enum AT_DSP_CltEnable {
    AT_DSP_CLT_DISABLE = 0,
    AT_DSP_CLT_ENABLE,

    AT_DSP_CLT_BUTT
};
typedef VOS_UINT8 AT_DSP_CltEnableUint8;


enum AT_DCXOTEMPCOMP_Enable {
    AT_DCXOTEMPCOMP_DISABLE = 0,
    AT_DCXOTEMPCOMP_ENABLE,

    AT_DCXOTEMPCOMP_BUTT
};
typedef VOS_UINT8 AT_DCXOTEMPCOMP_EnableUint8;

typedef struct {
    VOS_PID    pid;
    VOS_UINT32 msgPeakSize;
    VOS_UINT32 memPeakSize;
} AT_PID_MemInfoPara;


typedef struct {
    VOS_UINT32       setLowBands;
    VOS_UINT32       setHighBands;
    AT_DSP_BandArfcn dspBandArfcn;
} DRV_AGENT_RxpriSetReq;


typedef struct {
    AT_APPCTRL            atAppCtrl;
    DRV_AGENT_ErrorUint32 result;
    DRV_AGENT_RxpriSetReq rxpriSetReq;
    VOS_UINT16            wPriBands;
    VOS_UINT16            gPriBands;
    VOS_UINT16            setPriBands;
    VOS_UINT8             reserved[2];
} DRV_AGENT_RxpriSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 prodType;

} DRV_AGENT_ProdtypeQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 tbatType;

} DRV_AGENT_TbatQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT16 wDrvPriBands;
    VOS_UINT16 gDrvPriBands;
    VOS_BOOL   fail;
} DRV_AGENT_RxpriQryCnf;

typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
} DRV_AGENT_AdcSetCnf;

typedef struct
{
    AT_APPCTRL atAppCtrl;
    SPY_TempThresholdPara tempProtect;
    VOS_BOOL fail;
}DRV_AGENT_AdcQryCnf;

typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT8  len;
    VOS_UINT8  reserved[2];
    VOS_CHAR   data[AT_AGENT_DRV_VERSION_TIME_LEN + 1];
} DRV_AGENT_VersionTime;

/*
 * ????????: AT??AT AGENT AT^YJCX????????????????
 */
typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
    VOS_UCHAR  flashInfo[TAF_MAX_FLAFH_INFO_LEN + 1];
} DRV_AGENT_YjcxSetCnf;

/*
 * ????????: AT??AT AGENT AT^YJCX????????????????
 */
typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
    VOS_UCHAR  gpioInfo[TAF_MAX_GPIO_INFO_LEN + 1];
    VOS_UINT8  reserved[3];
} DRV_AGENT_YjcxQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 curImsiSign;
    VOS_UINT16 dualIMSIEnable;
    VOS_UINT8  reserved[2];
} DRV_AGENT_ImsichgQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 rslt;
} DRV_AGENT_InforbuSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 rslt;
} DRV_AGENT_InforrsSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   normalSrvStatus;
    VOS_UINT32 pnnExistFlg;
    VOS_UINT32 oplExistFlg;

} DRV_AGENT_CpnnTestCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 rslt;
} DRV_AGENT_NvbackupSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   normalSrvStatus;
} DRV_AGENT_CpnnQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
    /* ????????????0:VOS????, 1: TTF???? */
    AT_MEMQUERY_ParaUint32 memQryType;
    VOS_UINT32             pidNum;  /* PID?????? */
    VOS_UINT8              data[4]; /* ????PID??????????????,????????????????????????TAF_DEFAULT_CONTENT_LEN?? */
} DRV_AGENT_MeminfoQryRsp;


typedef struct {
    AT_APPCTRL                     atAppCtrl;
    DRV_AGENT_TselrfSetErrorUint32 result;
    VOS_UINT8                      deviceRatMode;
    VOS_UINT8                      reserved[3];
} DRV_AGENT_TselrfSetCnf;


typedef struct {
    AT_APPCTRL                    atAppCtrl;
    DRV_AGENT_HkadcGetErrorUint32 result;
    VOS_INT32                     tbatHkadc;
} DRV_AGENT_HkadcGetCnf;

#if (FEATURE_ON == FEATURE_SECURITY_SHELL)

typedef struct {
    VOS_CHAR shellPwd[AT_SHELL_PWD_LEN];
} DRV_AGENT_SpwordSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
} DRV_AGENT_SpwordSetCnf;
#endif


typedef struct {
    AT_APPCTRL atAppCtrl;    /* ?????? */
    VOS_UINT32 result;       /* ???????? */
    VOS_UINT32 nvBackupStat; /* NV???????? */
} DRV_AGENT_NvbackupstatQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;         /* ?????? */
    VOS_UINT32 result;            /* ???????? */
    VOS_UINT32 badBlockNum;       /* ???????? */
    VOS_UINT32 badBlockIndex[0];  //lint !e43 /* ?????????? */
} DRV_AGENT_NandbbcQryCnf;


#define DRV_AGENT_NAND_MFU_NAME_MAX_LEN 16 /* ???????????????? */
#define DRV_AGENT_NAND_DEV_SPEC_MAX_LEN 32 /* ???????????????? */

typedef struct {
    VOS_UINT32 mufId;                                    /* ????ID */
    VOS_UINT8  mufName[DRV_AGENT_NAND_MFU_NAME_MAX_LEN]; /* ?????????????? */
    VOS_UINT32 devId;                                    /* ????ID */
    VOS_UINT8  devSpec[DRV_AGENT_NAND_DEV_SPEC_MAX_LEN]; /* ?????????????? */
} DRV_AGENT_NandDevInfo;


typedef struct {
    AT_APPCTRL            atAppCtrl;   /* ?????? */
    VOS_UINT32            result;      /* ???????? */
    DRV_AGENT_NandDevInfo nandDevInfo; /* ???????? */
} DRV_AGENT_NandverQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl; /* ?????? */
    VOS_UINT32 result;    /* ???????? */
    VOS_INT    gpaTemp;   /* G PA???? */
    VOS_INT    wpaTemp;   /* W PA???? */
    VOS_INT    lpaTemp;   /* L PA???? */
    VOS_INT    simTemp;   /* SIM?????? */
    VOS_INT    batTemp;   /* ???????? */
} DRV_AGENT_ChiptempQryCnf;



typedef struct {
    AT_APPCTRL atAppCtrl; /* ?????? */
    VOS_UINT16 antState;
    VOS_UINT8  rsv[2];
} DRV_AGENT_AntStateInd;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
} DRV_AGENT_MaxLockTmsSetCnf;


typedef struct {
    VOS_UINT32 usimState;
} DRV_AGENT_ApSimstSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
} DRV_AGENT_ApSimstSetCnf;


typedef struct {
    VOS_UINT8 huk[DRV_AGENT_HUK_LEN]; /* HUK???? 128Bits */
} DRV_AGENT_HukSetReq;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
} DRV_AGENT_HukSetCnf;


typedef struct {
    /* ???????????? */
    VOS_UINT8 pubKey[DRV_AGENT_PUBKEY_LEN];
    /* ????????SSK???????? */
    VOS_UINT8 pubKeySign[DRV_AGENT_PUBKEY_SIGNATURE_LEN];
} DRV_AGENT_FacauthpubkeySetReq;


typedef struct {
    /* 2:OTA???? 1: ???????? 0: ???????? */
    AT_SimlockTypeUint8 netWorkFlg;
    VOS_UINT8           layer;
    VOS_UINT8           reserve[2];
    VOS_UINT32          total; /* ?????????????? */
    VOS_UINT32          hmacLen;
    VOS_UINT32          simlockDataLen;
    VOS_UINT8           hmac[DRV_AGENT_HMAC_DATA_LEN];
    VOS_UINT8           simlockData[4]; /* ????????????????????????????TAF_DEFAULT_CONTENT_LEN?? */
} DRV_AGENT_SimlockwriteexSetReq;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    VOS_UINT32                         total;     /* ?????????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
} DRV_AGENT_SimlockwriteexSetCnf;


typedef struct {
    VOS_UINT8 layer;    /* ?????????? */
    VOS_UINT8 indexNum; /* ?????????? */
} DRV_AGENT_SimlockdatareadexReadReq;


typedef struct {
    AT_APPCTRL atAppCtrl; /* ?????? */
    VOS_UINT32 result;
    VOS_UINT8  layer;    /* ?????????? */
    VOS_UINT8  indexNum; /* ?????????? */
    VOS_UINT8  total;    /* ???????????? */
    VOS_UINT8  reserve;
    VOS_UINT32 dataLen; /* ???????????? */
    VOS_UINT8  data[4]; /* ???? */
} DRV_AGENT_SimlockdatareadexReadCnf;


typedef struct {
    VOS_UINT8 para[DRV_AGENT_PUBKEY_LEN];
} DRV_AGENT_FacauthpubkeyexSetReq;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
} DRV_AGENT_FacauthpubkeySetCnf;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
    /* ????????????RSA???????????? */
    VOS_UINT8  rsaText[DRV_AGENT_RSA_CIPHERTEXT_LEN];
    VOS_UINT32 publicId[TAF_OTA_SIMLOCK_PUBLIC_ID_NUM];
    VOS_UINT8  otaFlag; /* ????OTA????:0-??OTA,1-OTA */
    VOS_UINT8  rsv[3];
} DRV_AGENT_IdentifystartSetCnf;


typedef struct {
    /* ????????????RSA???????????????? */
    VOS_UINT32 rsaTextlen;
    /* ????????????RSA???????????? */
    VOS_UINT8  rsaText[DRV_AGENT_RSA_TLV_CIPHERTEXT_LEN];
} DRV_AGENT_IdentifyendSetReq;


typedef struct {
    /* ????????????RSA???????????? */
    DRV_AGENT_IdentifyendSetReq rsa;
    VOS_UINT8                   otaFlag;
    VOS_UINT8                   rsv[3];
} DRV_AGENT_IdentifyendOtaSetReq;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
} DRV_AGENT_IdentifyendSetCnf;



typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
} DRV_AGENT_SimlockdatawriteSetCnf;


typedef struct {
    VOS_UINT8 phLockCodeBegin[DRV_AGENT_PH_LOCK_CODE_LEN_EXTERNED];
    VOS_UINT8 phLockCodeEnd[DRV_AGENT_PH_LOCK_CODE_LEN_EXTERNED];
} DRV_AGENT_PhLockCodeNew;


typedef struct {
    VOS_UINT8 phLockCodeBegin[DRV_AGENT_PH_LOCK_CODE_LEN];
    VOS_UINT8 phLockCodeEnd[DRV_AGENT_PH_LOCK_CODE_LEN];
} DRV_AGENT_PhLockCode;


typedef struct {
    DRV_AGENT_PersonalizationCategoryUint8  category;  /* ??????????category???? */
    DRV_AGENT_PersonalizationIndicatorUint8 indicator; /* ?????????????????? */
    DRV_AGENT_PersonalizationStatusUint8    status;
    VOS_UINT8                               maxUnlockTimes;    /* ?????????????????????? */
    VOS_UINT8                               remainUnlockTimes; /* ?????????????????????? */
    VOS_UINT8                               rsv[3];            /* ???????????????????????? */
} DRV_AGENT_SimlockDataCategory;


typedef struct {
    AT_APPCTRL                         atAppCtrl;          /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;             /* ???????????? */
    VOS_UINT8                          supportCategoryNum; /* ????category???? */
    VOS_UINT8                          rsv[3];             /* ???????????????????????? */

    /* ????????3??category????????????????network->network subset->SP?????????? */
    DRV_AGENT_SimlockDataCategory categoryData[DRV_AGENT_SUPPORT_CATEGORY_NUM];
} DRV_AGENT_SimlockdatareadQryCnf;

/*
 * ????????: ??????TAF????AT^GETMODEMSCID????????????SC????????ID??????????????
 */
typedef struct {
    DRV_AGENT_EncryptRsaAlgorithmUint32 scAlgorithm;
    VOS_UINT32                          scEncryptIdNum;
    VOS_UINT32                          scEncryptId[DRV_AGENT_SIMLOCK_ENCRYPT_KEYID_NUM];
} DRV_AGENT_SimlockEncryptIdInfo;

/*
 * ????????: AT??DRV AGENT??????????????????????????????????
 */
typedef struct {
    AT_APPCTRL                                atAppCtrl;          /* ?????? */
    DRV_AGENT_PersonalizationErrUint32        result;             /* ???????????? */
    DRV_AGENT_SimlockEncryptIdInfo            scEncryptIdInfo;
} DRV_AGENT_GetSimlockEncryptIdQryCnf ;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
    /* ????????4??category????????????????network->network subset->SP->CP?????????? */
    DRV_AGENT_SimlockDataCategory categoryData[DRV_AGENT_SUPPORT_CATEGORY_NUM];
} DRV_AGENT_SimlockdatareadQryCnfEx;


typedef struct {
    DRV_AGENT_PersonalizationCategoryUint8  category;  /* ??????????category???? */
    DRV_AGENT_PersonalizationIndicatorUint8 indicator; /* ?????????????????? */
    VOS_UINT8                               flag;      /* 0: Trust???? 1: Block???? */
    VOS_UINT8                               rsv;       /* 4????????, ?????? */
    /* ??????????????begin/end?????????? */
    VOS_UINT32 groupNum;
    /* ?????????????????? */
    DRV_AGENT_PhLockCodeNew lockCode[DRV_AGENT_PH_LOCK_CODE_GROUP_NUM_EXTERED];
} DRV_AGENT_SimlockInfoCategoryNew;


typedef struct {
    DRV_AGENT_PersonalizationCategoryUint8  category;  /* ??????????category???? */
    DRV_AGENT_PersonalizationIndicatorUint8 indicator; /* ?????????????????? */
    /* ??????????????begin/end?????????? */
    VOS_UINT8 groupNum;
    VOS_UINT8 rsv; /* 4????????, ?????? */
    /* ?????????????????? */
    DRV_AGENT_PhLockCode lockCode[DRV_AGENT_PH_LOCK_CODE_GROUP_NUM];
} DRV_AGENT_SimlockInfoCategory;


typedef struct {
    AT_APPCTRL                         atAppCtrl;          /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;             /* ???????????? */
    VOS_UINT8                          supportCategoryNum; /* ????category???? */
    VOS_UINT8                          reserve[3];
    DRV_AGENT_SimlockInfoCategoryNew   categoryInfo[DRV_AGENT_SUPPORT_CATEGORY_NUM_EXTERED];
} DRV_AGENT_PhonesimlockinfoQryCnfNew;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */

    /* ????????3??category????????????????network->network subset->SP?????????? */
    DRV_AGENT_SimlockInfoCategory categoryInfo[DRV_AGENT_SUPPORT_CATEGORY_NUM];
} DRV_AGENT_PhonesimlockinfoQryCnf;


typedef struct {
    DRV_AGENT_PhPhynumTypeUint8 phynumType; /* ?????????? */
    VOS_UINT8  reserve[3];
    VOS_UINT8  phynumValue[DRV_AGENT_RSA_TLV_CIPHERTEXT_LEN]; /* ??????RSA???????? */
    VOS_UINT32 rsaIMEILen;                                /* Rsa???????????????? */
    VOS_UINT8  hmacData[DRV_AGENT_HMAC_DATA_LEN];         /* hmac???? */
    VOS_UINT32 hmacLen;                                   /* hmac???????? */
} DRV_AGENT_PhonephynumSetReq;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
} DRV_AGENT_PhonephynumSetCnf;



typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
} DRV_AGENT_PortattribsetSetCnf;


typedef struct {
    AT_APPCTRL                         atAppCtrl;  /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;     /* ???????????? */
    DRV_AGENT_PortStatusUint32         portStatus; /* ???????????? */
} DRV_AGENT_PortattribsetQryCnf;



typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ?????? */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ???????????? */
} DRV_AGENT_OpwordSetCnf;


typedef struct {
    DRV_AGENT_DhKeyTypeUint32 keyType; /* DH???????? */
    VOS_UINT32                keyLen;  /* DH???????? */
    /* DH?????????????????????????????????????????????????? */
    VOS_UINT8 key[DRV_AGENT_DH_PUBLICKEY_LEN];
} DRV_AGENT_HvpdhReq;


typedef struct {
    AT_APPCTRL               atAppCtrl; /* ?????? */
    DRV_AGENT_HvpdhErrUint32 result;    /* ???????????? */
} DRV_AGENT_HvpdhCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 rslt;
} DRV_AGENT_NvmanufactureextSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 chkPoolLeakRslt;
    VOS_UINT32 chkNodeLeakRslt;
    VOS_UINT32 chkRrmNodeLeakRslt;
} DRV_AGENT_QryCcpuMemInfoCnf;
/*
 * H2ASN????????????????
 */
typedef struct {
    DRV_AGENT_MsgTypeUint32 msgId; /* _H2ASN_MsgChoice_Export DRV_AGENT_MsgTypeUint32  */
    VOS_UINT8               msgBlock[4];
    /*
     * _H2ASN_MsgChoice_When_Comment          DRV_AGENT_MsgTypeUint32
     */
} DRV_AGENT_MsgData;

typedef struct {
    VOS_MSG_HEADER
    DRV_AGENT_MsgData msgData;
} TafDrvAgent_Msg;


typedef struct {
    VOS_UINT8 swVer[TAF_MAX_REVISION_ID_LEN + 1];
    VOS_CHAR  verTime[AT_AGENT_DRV_VERSION_TIME_LEN];
} DRV_AGENT_SwverInfo;


typedef struct {
    AT_APPCTRL            atAppCtrl;
    DRV_AGENT_ErrorUint32 result;
    DRV_AGENT_SwverInfo   swverInfo;
} DRV_AGENT_SwverSetCnf;


typedef struct {
    VOS_UINT8 drvAgentSystemAppConfigAddr; /* DrvAgent???????????????? */
    VOS_UINT8 gcfInd;                      /* ??????????????GCF???? */
    VOS_UINT8 simLockErrTimes;             /* ????SIMLOCK?????????????????? */
    VOS_UINT8 reserve[5];                  /* PCLINT-64 ????8???????? */
} DRV_AGENT_Context;

extern DRV_AGENT_Context* DRVAGENT_GetCtxAddr(VOS_VOID);

extern VOS_VOID   TAF_MMA_GetProductionVersion(VOS_CHAR *pcDest);
extern VOS_UINT32 At_SendRfCfgAntSelToHPA(VOS_UINT8 divOrPriOn, VOS_UINT8 index);
extern VOS_UINT32 At_DelCtlAndBlankCharWithEndPadding(VOS_UINT8 *data, VOS_UINT16 *cmdLen);

#if (VOS_RTOSCK == VOS_OS_VER)
extern VOS_UINT32 SRE_CpuUsageGet(VOS_UINT32 coreId);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of TafDrvAgent.h */
