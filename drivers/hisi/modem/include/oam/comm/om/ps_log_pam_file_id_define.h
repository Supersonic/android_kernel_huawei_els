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

#ifndef PS_LOG_PAM_FILE_ID_DEFINE_H
#define PS_LOG_PAM_FILE_ID_DEFINE_H

#include "ps_log_file_id_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* PAM 文件ID号 */
typedef enum
{
/* 18432 */      PS_FILE_ID_HPA_ENTITY_C = PAM_TEAM_FILE_ID,
/* 18433 */      PS_FILE_ID_HPA_GLOBAL_VAR_C,
/* 18434 */      PS_FILE_ID_HPA_ISR_C,
/* 18435 */      PS_FILE_ID_HPA_INIT_C,
/* 18436 */      PS_FILE_ID_HPA_RINGBUFFER_C,
/* 18437 */      PS_FILE_ID_HPA_OPERATE_RTT_C,
/* 18438 */      PS_FILE_ID_APM_PROCESS_C,
/* 18439 */      PS_FILE_ID_NVIM_FILEOPERATEINTERFACE_C,
/* 18440 */      PS_FILE_ID_NVIM_FUNCTION_C,
/* 18441 */      PS_FILE_ID_NVIM_GCFFUNCTION_C,
/* 18442 */      PS_FILE_ID_OMRL_C,
/* 18443 */      PS_FILE_ID_DIAG_C,
/* 18444 */      PS_FILE_ID_OM_OUTSIDE_C,
/* 18445 */      PS_FILE_ID_OMFS_C,
/* 18446 */      PS_FILE_ID_OM_C,
/* 18447 */      PS_FILE_ID_OM_SOCK_C,
/* 18448 */      PS_FILE_ID_V_BLKMEM_C,
/* 18449 */      PS_FILE_ID_V_LIB_C,
/* 18450 */      PS_FILE_ID_V_MONITOR_DSP_C,
/* 18451 */      PS_FILE_ID_V_MSG_C,
/* 18452 */      PS_FILE_ID_V_NSPRINTF_C,
/* 18453 */      PS_FILE_ID_V_QUEUE_C,
/* 18454 */      PS_FILE_ID_V_RTC_TIMER_C,
/* 18455 */      PS_FILE_ID_V_SEM_C,
/* 18456 */      PS_FILE_ID_V_SOCK_C,
/* 18457 */      PS_FILE_ID_V_SPRINTF_C,
/* 18458 */      PS_FILE_ID_V_SSCANF_C,
/* 18459 */      PS_FILE_ID_V_TASK_C,
/* 18460 */      PS_FILE_ID_V_TIMER_C,
/* 18461 */      PS_FILE_ID_V_WATCH_DOG_C,
/* 18462 */      PS_FILE_ID_V_WIN32_SEM_C,
/* 18463 */      PS_FILE_ID_V_WIN32_SOCK_C,
/* 18464 */      PS_FILE_ID_V_WIN32_TASK_C,
/* 18465 */      PS_FILE_ID_V_NUCLEUS_SEM_C,
/* 18466 */      PS_FILE_ID_V_NUCLEUS_SOCK_C,
/* 18467 */      PS_FILE_ID_V_NUCLEUS_TASK_C,
/* 18468 */      PS_FILE_ID_VOS_ID_C,
/* 18469 */      PS_FILE_ID_VOS_MAIN_C,
/* 18470 */      PS_FILE_ID_SC_APP_COMM_C,
/* 18471 */      PS_FILE_ID_M_LOWPOWER_C,
/* 18472 */      PS_FILE_ID_CBT_MAIL_BOX_C,
/* 18473 */      PS_FILE_ID_DH_R_DH_C,
/* 18474 */      PS_FILE_ID_DH_AES_UTIL_C,
/* 18475 */      PS_FILE_ID_SC_IMEI_C,
/* 18476 */      PS_FILE_ID_SC_PUBLICKKEY_C,
/* 18477 */      PS_FILE_ID_PS_COMM_FUNC_C,
/* 18478 */      PS_FILE_ID_PAM_NR_OM_C,
/* 18479 */      PS_FILE_ID_V_RTOSCK_SEM_C,
/* 18480 */      PS_FILE_ID_V_RTOSCK_TASK_C,
/* 18481 */      PS_FILE_ID_PAM_LOG_FILETER_C,
/* 18482 */      PS_FILE_ID_PAM_TASK_PROC_C,
/* 18483 */      PS_FILE_ID_PAM_APP_LOG_FILETER_C,
/* 18484 */      PS_FILE_ID_PAM_APP_TASK_PROC_C,

/* 18475 */      PS_FILE_ID_SC_TIME_RECORD_C,
/* 18476 */      PS_FILE_ID_SC_UNLOCK_C,
/* 18477 */      PS_FILE_ID_SC_VERIFY_C,
/* 18478 */      PS_FILE_ID_SC_FILE_API_C,
/* 18479 */      PS_FILE_ID_SC_DATA_READ_C,
/* 18480 */      PS_FILE_ID_SC_INIT_C,
/* 18481 */      PS_FILE_ID_SC_DATA_WRITE_C,
/* 18482 */      PS_FILE_ID_SC_GET_CUST_PUBLICK_KEY_C,
/* 18483 */      PS_FILE_ID_SC_CRYPTO_C,

/* 18580 */      PS_FILE_ID_USIMM_APDU_C = 18580,
/* 18581 */      PS_FILE_ID_USIMM_API_C,
/* 18582 */      PS_FILE_ID_USIMM_AUTH_C,
/* 18583 */      PS_FILE_ID_USIMM_BASE_FILE_C,
/* 18584 */      PS_FILE_ID_USIMM_BASE_INFO_C,
/* 18585 */      PS_FILE_ID_USIMM_CARD_APP_INFO_C,
/* 18586 */      PS_FILE_ID_USIMM_CDMA_AUTH_C,
/* 18587 */      PS_FILE_ID_USIMM_CHANNEL_C,
/* 18588 */      PS_FILE_ID_USIMM_CLEAR_VAR_C,
/* 18589 */      PS_FILE_ID_USIMM_COMM_INIT_C,
/* 18590 */      PS_FILE_ID_USIMM_CSIM_INIT_C,
/* 18591 */      PS_FILE_ID_USIMM_DL_C,
/* 18592 */      PS_FILE_ID_USIMM_DRV_C,
/* 18593 */      PS_FILE_ID_USIMM_EXTERNAL_API_C,
/* 18594 */      PS_FILE_ID_USIMM_FDN_C,
/* 18595 */      PS_FILE_ID_USIMM_FILE_POOL_C,
/* 18596 */      PS_FILE_ID_USIMM_HOTPLUG_C,
/* 18597 */      PS_FILE_ID_USIMM_ICC_INIT_C,
/* 18598 */      PS_FILE_ID_USIMM_ISIM_INIT_C,
/* 18599 */      PS_FILE_ID_USIMM_LOG_C,
/* 18600 */      PS_FILE_ID_USIMM_MSG_PROC_C,
/* 18601 */      PS_FILE_ID_USIMM_PIN_C,
/* 18602 */      PS_FILE_ID_USIMM_PRINT_C,
/* 18603 */      PS_FILE_ID_USIMM_PROTECT_RESET_C,
/* 18604 */      PS_FILE_ID_USIMM_QUERY_SET_C,
/* 18605 */      PS_FILE_ID_USIMM_READ_FILE_C,
/* 18606 */      PS_FILE_ID_USIMM_ROAMING_C,
/* 18607 */      PS_FILE_ID_USIMM_SAT_C,
/* 18608 */      PS_FILE_ID_USIMM_SCI_CONFIG_C,
/* 18609 */      PS_FILE_ID_USIMM_SELECT_FILE_C,
/* 18610 */      PS_FILE_ID_USIMM_SIM_INIT_C,
/* 18611 */      PS_FILE_ID_USIMM_STATUS_C,
/* 18612 */      PS_FILE_ID_USIMM_UICC_INIT_C,
/* 18613 */      PS_FILE_ID_USIMM_UIM_INIT_C,
/* 18614 */      PS_FILE_ID_USIMM_UPDATE_FILE_C,
/* 18615 */      PS_FILE_ID_USIMM_USIM_INIT_C,
/* 18616 */      PS_FILE_ID_USIMM_VSIM_C,
/* 18617 */      PS_FILE_ID_USIMM_VSIM_AUTH_C,
/* 18618 */      PS_FILE_ID_USIMM_APDU_BASE_C,
/* 18619 */      PS_FILE_ID_USIMM_APDU_FILL_C,
/* 18620 */      PS_FILE_ID_USIMM_APDU_FILL_CDMA_C,
/* 18621 */      PS_FILE_ID_USIMM_AUTH_BASE_C,
/* 18622 */      PS_FILE_ID_USIMM_AUTH_CDMA_C,
/* 18623 */      PS_FILE_ID_USIMM_AUTH_ISIM_C,
/* 18624 */      PS_FILE_ID_USIMM_AUTH_MSG_C,
/* 18625 */      PS_FILE_ID_USIMM_AUTH_SIM_C,
/* 18626 */      PS_FILE_ID_USIMM_AUTH_USIM_C,
/* 18627 */      PS_FILE_ID_USIMM_CHANNEL_BASE_C,
/* 18628 */      PS_FILE_ID_USIMM_CHANNEL_CGLA_C,
/* 18629 */      PS_FILE_ID_USIMM_CHANNEL_MANAGE_C,
/* 18630 */      PS_FILE_ID_USIMM_CHANNEL_PRIVATE_CGLA_C,
/* 18631 */      PS_FILE_ID_USIMM_DL_CGLA_C,
/* 18632 */      PS_FILE_ID_USIMM_DL_PASS_THROUGH_C,
/* 18633 */      PS_FILE_ID_USIMM_RESERVE_C,
/* 18634 */      PS_FILE_ID_USIMM_FILE_BASE_C,
/* 18635 */      PS_FILE_ID_USIMM_FILE_FCP_DECODE_C,
/* 18636 */      PS_FILE_ID_USIMM_FILE_READ_C,
/* 18637 */      PS_FILE_ID_USIMM_FILE_SELECT_C,
/* 18638 */      PS_FILE_ID_USIMM_FILE_UPDATE_C,
/* 18639 */      PS_FILE_ID_USIMM_FILESYS_API_C,
/* 18640 */      PS_FILE_ID_USIMM_INIT_COMM_C,
/* 18641 */      PS_FILE_ID_USIMM_INIT_CSIM_C,
/* 18642 */      PS_FILE_ID_USIMM_INIT_ICC_C,
/* 18643 */      PS_FILE_ID_USIMM_INIT_ISIM_C,
/* 18644 */      PS_FILE_ID_USIMM_INIT_MSG_C,
/* 18645 */      PS_FILE_ID_USIMM_INIT_SIM_C,
/* 18646 */      PS_FILE_ID_USIMM_INIT_UICC_C,
/* 18647 */      PS_FILE_ID_USIMM_INIT_UIM_C,
/* 18648 */      PS_FILE_ID_USIMM_INIT_USIM_C,
/* 18649 */      PS_FILE_ID_USIMM_LOG_DATA_C,
/* 18650 */      PS_FILE_ID_USIMM_LOG_FILE_C,
/* 18651 */      PS_FILE_ID_USIMM_NV_C,
/* 18652 */      PS_FILE_ID_USIMM_PASS_THROUGH_C,
/* 18653 */      PS_FILE_ID_USIMM_PIN_INIT_C,
/* 18654 */      PS_FILE_ID_USIMM_PROTECT_RESET_ICC_C,
/* 18655 */      PS_FILE_ID_USIMM_PROTECT_RESET_UICC_C,
/* 18656 */      PS_FILE_ID_USIMM_RESTRICT_ACCESS_C,
/* 18657 */      PS_FILE_ID_USIMM_SAT_REFRESH_C,
/* 18658 */      PS_FILE_ID_USIMM_SERVICE_TABLE_C,
/* 18659 */      PS_FILE_ID_USIMM_SIMULATE_ISIM_C,
/* 18660 */      PS_FILE_ID_USIMM_SUCI_C,
/* 18661 */      PS_FILE_ID_USIMM_TASK_INIT_C,
/* 18662 */      PS_FILE_ID_USIMM_TEST_CARD_C,
/* 18663 */      PS_FILE_ID_USIMM_TLV_C,

/* 18700 */      PS_FILE_ID_SI_ACORE_API_C = 18700,
/* 18701 */      PS_FILE_ID_SI_CCORE_API_C,
/* 18702 */      PS_FILE_ID_STK_API_C,
/* 18703 */      PS_FILE_ID_STK_ATPRINTF_C,
/* 18704 */      PS_FILE_ID_STK_CALL_C,
/* 18705 */      PS_FILE_ID_STK_CMD_PROC_C,
/* 18706 */      PS_FILE_ID_STK_CODEC_C,
/* 18707 */      PS_FILE_ID_STK_CUSTOM_IMSI_CHANGE_C,
/* 18708 */      PS_FILE_ID_STK_ENVELOPE_C,
/* 18709 */      PS_FILE_ID_STK_EXTERNAL_API_C,
/* 18710 */      PS_FILE_ID_STK_MENU_C,
/* 18711 */      PS_FILE_ID_STK_MSG_PROC_C,
/* 18712 */      PS_FILE_ID_STK_PROVIDE_LOCATION_C,
/* 18713 */      PS_FILE_ID_STK_REFRESH_C,
/* 18714 */      PS_FILE_ID_STK_SMS_C,
/* 18715 */      PS_FILE_ID_STK_SS_C,
/* 18716 */      PS_FILE_ID_STK_SYSINFO_C,
/* 18717 */      PS_FILE_ID_STK_TIMER_C,
/* 18718 */      PS_FILE_ID_STK_CUSTOM_ATT_C,
/* 18719 */      PS_FILE_ID_STK_COMM_C,
/* 18720 */      PS_FILE_ID_STK_SMS_CDMA_C,
/* 18721 */      PS_FILE_ID_STK_USSD_C,
/* 18722 */      PS_FILE_ID_STK_EVENT_C,
/* 18723 */      PS_FILE_ID_STK_CMD_REPORT_CFG_C,
/* 18724 */      PS_FILE_ID_STK_CALL_DTMF_C,
/* 18725 */      PS_FILE_ID_STK_TASK_INIT_C,
/* 18726 */      PS_FILE_ID_STK_PROVIDE_LOCATION_DATA_C,
/* 18727 */      PS_FILE_ID_STK_AT_PRINTF_API_C,
/* 18728 */      PS_FILE_ID_STK_EXTERNAL_AT_API_C,

/* 18750 */      PS_FILE_ID_PIH_API_C = 18750,
/* 18751 */      PS_FILE_ID_PIH_AT_CNF_C,
/* 18752 */      PS_FILE_ID_PIH_AT_REQ_C,
/* 18753 */      PS_FILE_ID_PIH_CHANNEL_C,
/* 18754 */      PS_FILE_ID_PIH_CHR_PROC_C,
/* 18755 */      PS_FILE_ID_PIH_EXTERNAL_API_C,
/* 18756 */      PS_FILE_ID_PIH_IND_REPORT_C,
/* 18757 */      PS_FILE_ID_PIH_KEY_FILE_C,
/* 18758 */      PS_FILE_ID_PIH_LIST_C,
/* 18759 */      PS_FILE_ID_PIH_MSG_PROC_C,
/* 18760 */      PS_FILE_ID_PIH_PROTECT_RESET_C,
/* 18761 */      PS_FILE_ID_PIH_SCI_CHG_C,
/* 18762 */      PS_FILE_ID_PIH_VSIM_ICC_C,
/* 18763 */      PS_FILE_ID_PIH_TASK_INIT_C,
/* 18764 */      PS_FILE_ID_PIH_PRIVAET_AT_API_C,
/* 18765 */      PS_FILE_ID_PIH_PRIVATE_AT_REQ_C,
/* 18766 */      PS_FILE_ID_PIH_STANDARD_AT_API_C,
/* 18767 */      PS_FILE_ID_PIH_STANDARD_AT_REQ_C,

/* 18800 */      PS_FILE_ID_SI_EMAT_API_C = 18800,
/* 18801 */      PS_FILE_ID_SI_EMAT_PROC_C,
/* 18802 */      PS_FILE_ID_SI_EMAT_EXTERNAL_API_C,
/* 18803 */      PS_FILE_ID_SI_EMAT_MSG_PROC_C,
/* 18804 */      PS_FILE_ID_SI_EMAT_DATA_C,
/* 18805 */      PS_FILE_ID_SI_EMAT_STATUS_C,
/* 18806 */      PS_FILE_ID_SI_EMAT_TASK_INIT_C,

/* 18820 */      PS_FILE_ID_PBAPI_C = 18820,
/* 18821 */      PS_FILE_ID_PB_PHONE_API_C,
/* 18822 */      PS_FILE_ID_PB_PHONE_DATA_C,
/* 18823 */      PS_FILE_ID_PB_PHONE_AT_MSG_C,
/* 18824 */      PS_FILE_ID_PB_PHONE_FILE_INIT_C,
/* 18825 */      PS_FILE_ID_PB_PHONE_MSG_PROC_C,
/* 18826 */      PS_FILE_ID_PB_PHONE_REFRESH_C,
/* 18827 */      PS_FILE_ID_PB_PHONE_TASK_INIT_C,
/* 18828 */      PS_FILE_ID_PB_PHONE_SEND_MSG_C,
/* 18829 */      PS_FILE_ID_PB_PHONE_RESERVE_C,
/* 18830 */      PS_FILE_ID_PB_DATA_C,
/* 18831 */      PS_FILE_ID_PB_BASE_C,
/* 18832 */      PS_FILE_ID_PB_MSG_C,
/* 18900 */      PS_FILE_ID_STK_GOBAL_C = 18900, /* clean code整改前文件ID，转为llt保留 */
/* 18901 */      PS_FILE_ID_PIH_PROC_C,
/* 18902 */      PS_FILE_ID_STK_PROC_C,
/* 18903 */      PS_FILE_ID_USIMM_APDUMODULE_C,
/* 18904 */      PS_FILE_ID_USIMM_GLOBAL_C,
/* 18905 */      PS_FILE_ID_USIMM_DLMODULE_C,
/* 18906 */      PS_FILE_ID_USIMM_APICNF_C,
/* 18907 */      PS_FILE_ID_USIMM_BASE_C,
/* 18908 */      PS_FILE_ID_USIMM_COMMON_C,
/* 18909 */      PS_FILE_ID_USIMM_POOL_C,
/* 18910 */      PS_FILE_ID_USIMM_INIT_C,
/* 18911 */      PS_FILE_ID_V_PRIVATE_C,
/* 18912 */      PS_FILE_ID_OBSERVE_ANTEN_C,
/* 18913 */      PS_FILE_ID_OBSERVE_TEMPERATURE_C,
/* 18914 */      PS_FILE_ID_OBSERVE_INIT_C,
/* 18915 */      PS_FILE_ID_SPYSYSTEM_C,
/* 18916 */      PS_FILE_ID_PAM_OM_C,
/* 18917 */      PS_FILE_ID_SC_PERSONALIZATION_C,
/* 18918 */      PS_FILE_ID_SC_FACTORY_C,
/* 18919 */      PS_FILE_ID_SC_COMM_C,
/* 18920 */      PS_FILE_ID_SC_CONTEXT_C,

/* 18921 */      PS_FILE_ID_ALG_DCXO_C,
/* 18922 */      PS_FILE_ID_ALG_EDGEDPD_C,
/* 18923 */      PS_FILE_ID_ALG_VCC_LUT_C,
/* 18924 */      PS_FILE_ID_APP_CBT_FUNC_C,
/* 18925 */      PS_FILE_ID_BBA_CAL_PA_VCC_C,
/* 18926 */      PS_FILE_ID_BBA_CAL_PA_VCC_COMP_C,
/* 18927 */      PS_FILE_ID_CBT_FSM_C,
/* 18928 */      PS_FILE_ID_CBT_NV_INFO_C,
/* 18929 */      PS_FILE_ID_CBT_RL_C,
/* 18930 */      PS_FILE_ID_CBT_SCM_SOFT_DECODE_C,
/* 18931 */      PS_FILE_ID_CT_DPD_C,
/* 18932 */      PS_FILE_ID_CT_FESS_TX_HF_C,
/* 18933 */      PS_FILE_ID_CT_FESS_TX_LF_C,
/* 18934 */      PS_FILE_ID_CT_HF_CTRL_C,
/* 18935 */      PS_FILE_ID_CT_HF_DPD_C,
/* 18936 */      PS_FILE_ID_CT_PHY_BASE_C,
/* 18937 */      PS_FILE_ID_CT_PHY_LTE_C,
/* 18938 */      PS_FILE_ID_CT_PHY_NR_C,
/* 18939 */      PS_FILE_ID_CT_PHY_WCDMA_C,
/* 18940 */      PS_FILE_ID_DCXO_SELFCAL_C,
/* 18941 */      PS_FILE_ID_DCXO_SELFCAL_CTRL_C,
/* 18942 */      PS_FILE_ID_HF_TX_C,
/* 18943 */      PS_FILE_ID_LBT_CALL_C,
/* 18944 */      PS_FILE_ID_MFG_CHIP_TEST_C,
/* 18945 */      PS_FILE_ID_MFG_DIE_ID_QUERY_C,
/* 18946 */      PS_FILE_ID_MFG_RFIC_TEST_C,
/* 18947 */      PS_FILE_ID_MT_TRX_C,
/* 18948 */      PS_FILE_ID_NBT_CALL_C,
/* 18949 */      PS_FILE_ID_NBT_CDMA_C,
/* 18950 */      PS_FILE_ID_NBT_CSM_C,
/* 18951 */      PS_FILE_ID_NBT_CUC_COMM_C,
/* 18952 */      PS_FILE_ID_NBT_CUC_CTRL_C,
/* 18953 */      PS_FILE_ID_NBT_DDRMSG_C,
/* 18954 */      PS_FILE_ID_NBT_GSM_C,
/* 18955 */      PS_FILE_ID_NBT_GU_COMM_C,
/* 18956 */      PS_FILE_ID_NBT_GU_LISTMODE_C,
/* 18957 */      PS_FILE_ID_NBT_GU_MSG_C,
/* 18958 */      PS_FILE_ID_NBT_GU_NORMAL_C,
/* 18959 */      PS_FILE_ID_NBT_ISOLATEDFUNC_C,
/* 18960 */      PS_FILE_ID_NBT_LISTMODE_C,
/* 18961 */      PS_FILE_ID_NBT_LTEV_CALL_C,
/* 18962 */      PS_FILE_ID_NBT_TASK_LTE_C,
/* 18963 */      PS_FILE_ID_NBT_TASK_NR_C,
/* 18964 */      PS_FILE_ID_NBT_WCDMA_C,
/* 18965 */      PS_FILE_ID_NSM_BU_C,
/* 18966 */      PS_FILE_ID_NSM_BU_LOADFESS_C,
/* 18967 */      PS_FILE_ID_NSM_FTM_C,
/* 18968 */      PS_FILE_ID_NSM_LLC_C,
/* 18969 */      PS_FILE_ID_NSM_MISC_C,
/* 18970 */      PS_FILE_ID_NSM_NV_C,
/* 18971 */      PS_FILE_ID_PS_REG_RPT_C,
/* 18972 */      PS_FILE_ID_RFDEBUG_COMM_C,
/* 18973 */      PS_FILE_ID_RFDEBUG_CT_C,
/* 18974 */      PS_FILE_ID_RFDEBUG_CTRL_C,
/* 18975 */      PS_FILE_ID_RFDEBUG_DB_C,
/* 18976 */      PS_FILE_ID_RFDEBUG_FEIC_C,
/* 18977 */      PS_FILE_ID_RFDEBUG_GSM_TX_MODU_C,
/* 18978 */      PS_FILE_ID_RFDEBUG_HFRX_C,
/* 18979 */      PS_FILE_ID_RFDEBUG_HFTX_C,
/* 18980 */      PS_FILE_ID_RFDEBUG_RM_C,
/* 18981 */      PS_FILE_ID_RFDEBUG_TRX_C,
/* 18982 */      PS_FILE_ID_RFDEBUG_TX_CFG_C,
/* 18983 */      PS_FILE_ID_UECBT_CBT_DCOFFSET_C,
/* 18984 */      PS_FILE_ID_UECBT_CBT_NV_C,
/* 18985 */      PS_FILE_ID_UECBT_CT_ADAPTER_C,
/* 18986 */      PS_FILE_ID_UECBT_CT_COMM_C,
/* 18987 */      PS_FILE_ID_UECBT_CT_ET_DELAY_C,
/* 18988 */      PS_FILE_ID_UECBT_CT_GETX_CAL_C,
/* 18989 */      PS_FILE_ID_UECBT_CT_GETX_COMM_C,
/* 18990 */      PS_FILE_ID_UECBT_CT_GETX_CTRL_C,
/* 18991 */      PS_FILE_ID_UECBT_CT_GETX_MSG_C,
/* 18992 */      PS_FILE_ID_UECBT_CT_HFRX_C,
/* 18993 */      PS_FILE_ID_UECBT_CT_MNTN_C,
/* 18994 */      PS_FILE_ID_UECBT_CT_PAGAIN_DELTA_C,
/* 18995 */      PS_FILE_ID_UECBT_CT_RAT_SELF_C,
/* 18996 */      PS_FILE_ID_UECBT_CT_RAT_SELF_CTRL_C,
/* 18997 */      PS_FILE_ID_UECBT_CT_RX_C,
/* 18998 */      PS_FILE_ID_UECBT_CT_TX_C,
/* 18999 */      PS_FILE_ID_UECBT_CT_TX_LIB_C,
/* 19000 */      PS_FILE_ID_UECBT_FSM_C,
/* 19001 */      PS_FILE_ID_UECBT_PA_VCC_UTILITY_C,
/* 19002 */      PS_FILE_ID_UECBT_QUEUE_C,
/* 19003 */      PS_FILE_ID_UECBT_RFIC_SELFCAL_C,
/* 19004 */      PS_FILE_ID_NRCBT_FUNC_C,
/* 19005 */      PS_FILE_ID_CBT_PPM_C,
/* 19006 */      PS_FILE_ID_NBT_TEST_C,
/* 19007 */      PS_FILE_ID_PAM_APP_OM_C,


/* 文件ID请在上面添加 */
/* 19449  */     PS_FILE_ID_OAM_DMT_STUB_C = 19449,
                 PAM_FILE_ID_BUTT = 19450,
}PAM_FILE_ID_DEFINE_ENUM;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif


