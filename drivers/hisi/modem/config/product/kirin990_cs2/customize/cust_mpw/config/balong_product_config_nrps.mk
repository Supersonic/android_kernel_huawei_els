# MD5: 3a53ed8ee69a5d2322461b752909e9c2
CFG_FEATURE_ON                                  := 1
CFG_FEATURE_OFF                                 := 0
CFG_FEATURE_MLOG := FEATURE_ON
CFG_FEATURE_DT := FEATURE_ON
ifeq ($(EMU_TYPE_ESLPS),FEATURE_ON)
CFG_FEATURE_NPHY_STUB_ESL := FEATURE_ON
else
CFG_FEATURE_NPHY_STUB_ESL := FEATURE_OFF
endif
ifeq ($(EMU_TYPE_ESL),FEATURE_ON)
CFG_FEATURE_NRRC_EMU_ESL := FEATURE_ON
else
CFG_FEATURE_NRRC_EMU_ESL := FEATURE_OFF
endif
CFG_FEATURE_NL2_MAA_ALLOC := FEATURE_ON
CFG_NR_PROTOL_STACK_ENG := YES
CFG_FEATURE_NR_SEC_DISABLE := FEATURE_ON
CFG_FEATURE_NSSAI_AUTH := FEATURE_ON
CFG_FEATURE_OPRDEF_CATEGORY := FEATURE_ON
CFG_FEATURE_LADN := FEATURE_ON
CFG_NR_MAX_SERVING_CC_NUM    := 1
CFG_NR_MAX_UL_SERVING_CC_NUM := 1
CFG_NR_MAX_CG_NUM    := 1
CFG_FEATURE_MODEM1_SUPPORT_NR := FEATURE_OFF
CFG_FEATURE_NL2_1CC_MEM := FEATURE_ON
CFG_NR_MAX_PER_PLMN_NRSA_BC_NUM := 128
CFG_NR_MAX_PER_PLMN_ENDC_BC_NUM := 512
CFG_NR_MAX_PER_PLMN_NRDC_BC_NUM := 16
CFG_NR_MAX_PER_PLMN_TXSW_NRSA_BC_NUM := 64
CFG_NR_MAX_PER_PLMN_TXSW_ENDC_BC_NUM := 128
CFG_NR_MAX_NRSA_BC_NUM := 512
CFG_NR_MAX_ENDC_BC_NUM := 1024
CFG_NR_MAX_NRDC_BC_NUM := 16
CFG_NR_MAX_TXSW_NRSA_BC_NUM := 64
CFG_NR_MAX_TXSW_ENDC_BC_NUM := 128
CFG_NR_MAX_NR_FSD_OTHER_PARA_NUM:=128
CFG_NR_MAX_NR_FSU_OTHER_PARA_NUM:=128
CFG_NR_MAX_NR_FSD_PARA_NUM:=256
CFG_NR_MAX_NR_FSU_PARA_NUM:=256
CFG_NR_MAX_LTE_FSD_PARA_NUM:=256
CFG_NR_MAX_LTE_FSU_PARA_NUM:=256
CFG_NR_MAX_NR_FSPC_DL_NUM:=128
CFG_NR_MAX_NR_FSPC_UL_NUM:=128
CFG_NR_MAX_SIMPLE_FSC_NUM_PER_SUPER_FSC:=16
CFG_NR_MAX_FSC_NUM:=128
CFG_NR_MAX_PER_BC_NR_BAND_NUM := 2
CFG_NR_MAX_PER_BC_LTE_BAND_NUM := 5
CFG_NR_MAX_PER_BC_BAND_NUM := 6
CFG_NR_MAX_PER_BAND_CC_NUM := 6
CFG_FEATURE_NR_R16 := FEATURE_OFF
CFG_FEATURE_NR_R16_TODO := FEATURE_OFF
CFG_FEATURE_ENDC := FEATURE_ON
CFG_FEATURE_HMS_KIT := FEATURE_ON