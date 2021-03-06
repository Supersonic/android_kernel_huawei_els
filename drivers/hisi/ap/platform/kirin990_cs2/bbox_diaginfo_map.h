#ifndef __DIAGINFO_MAP
#define __DIAGINFO_MAP(x,y,z,k) 
#endif
#ifndef __DIAGINFO_MODULE_MAP
#define __DIAGINFO_MODULE_MAP(x) 
#endif
__DIAGINFO_MAP(L3_ECC_CE, SoC_AP, WARNING, HW)
__DIAGINFO_MAP(L3_ECC_UE, SoC_AP, WARNING, HW)
__DIAGINFO_MAP(CPU_UP_FAIL, SoC_AP, CRITICAL, HW)
__DIAGINFO_MAP(CPU_PANIC_INFO, SoC_AP, CRITICAL, HW)
__DIAGINFO_MAP(NOC_FAULT_INFO, SoC_AP, CRITICAL, HW)
__DIAGINFO_MAP(BLOCK_DMD_CP_IO, SoC_AP, WARNING, HW)
__DIAGINFO_MAP(BLOCK_DMD_NORMAL_IO, SoC_AP, WARNING, HW)
__DIAGINFO_MAP(FBE_DIAG_FAIL_ID, SoC_AP, WARNING, SW)
__DIAGINFO_MAP(LPM3_DDR_FAIl, DDR, CRITICAL, HW)
__DIAGINFO_MAP(SWING_DMD_FDUL_PW_ON, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_FDUL_PW_OFF, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_HWTS_PW_ON, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_HWTS_PW_OFF, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_AIC_PW_ON, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_AIC_PW_OFF, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_CAM_PW_ON, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_CAM_PW_OFF, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_CAM_IR_PW, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_CAM_TIMEOUT, FDUL, WARNING, HW)
__DIAGINFO_MAP(SWING_DMD_SLEEP_FUSION, FDUL, WARNING, SW)
__DIAGINFO_MAP(SWING_DMD_NPU_PLL_RETRY, FDUL, WARNING, SW)
__DIAGINFO_MODULE_MAP(SoC_AP)
__DIAGINFO_MODULE_MAP(DDR)
__DIAGINFO_MODULE_MAP(FDUL)
