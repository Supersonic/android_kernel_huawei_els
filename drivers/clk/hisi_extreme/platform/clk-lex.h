/*
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __CLK_LEX_H
#define __CLK_LEX_H
/* clk_crgctrl */
#define CLKIN_SYS		0
#define CLKIN_REF		1
#define CLK_FLL_SRC		2
#define CLK_PPLL0		3
#define CLK_PPLL1		4
#define CLK_PPLL2		5
#define CLK_PPLL3		6
#define CLK_PPLL7		7
#define CLK_SPLL		8
#define CLK_MODEM_BASE		9
#define CLK_AUPLL		10
#define CLK_PPLL_PCIE		11
#define PCLK		12
#define CLK_GATE_PPLL0		13
#define CLK_GATE_PPLL1		14
#define CLK_GATE_PPLL2		15
#define CLK_GATE_PPLL3		16
#define CLK_GATE_PPLL7		17
#define CLK_GATE_PPLL0_MEDIA		18
#define CLK_GATE_PPLL2_MEDIA		19
#define CLK_GATE_PPLL3_MEDIA		20
#define CLK_GATE_PPLL7_MEDIA		21
#define CLK_GATE_PPLL0_M2		22
#define CLK_GATE_PPLL2_M2		23
#define CLK_GATE_PPLL3_M2		24
#define CLK_PPLL_EPS		25
#define CLK_SYS_INI		26
#define CLK_DIV_SYSBUS		27
#define CLK_DIV_CFGBUS		28
#define PCLK_GPIO0		29
#define PCLK_GPIO1		30
#define PCLK_GPIO2		31
#define PCLK_GPIO3		32
#define PCLK_GPIO4		33
#define PCLK_GPIO5		34
#define PCLK_GPIO6		35
#define PCLK_GPIO7		36
#define PCLK_GPIO8		37
#define PCLK_GPIO9		38
#define PCLK_GPIO10		39
#define PCLK_GPIO11		40
#define PCLK_GPIO12		41
#define PCLK_GPIO13		42
#define PCLK_GPIO14		43
#define PCLK_GPIO15		44
#define PCLK_GPIO16		45
#define PCLK_GPIO17		46
#define PCLK_GPIO18		47
#define PCLK_GPIO19		48
#define PCLK_GATE_WD0_HIGH		49
#define CLK_GATE_WD0_HIGH		50
#define PCLK_GATE_WD0		51
#define PCLK_GATE_WD1		52
#define CLK_MUX_WD0		53
#define CODECCSSI_MUX		54
#define CLK_GATE_CODECSSI		55
#define PCLK_GATE_CODECSSI		56
#define CLK_FACTOR_TCXO		57
#define CLK_GATE_TIMER5_A		58
#define CLK_ANDGT_HSDT1_USBDP		59
#define CLK_DIV_HSDT1_USBDP		60
#define CLK_GATE_HSDT1_USBDP		61
#define AUTODIV_SYSBUS		62
#define AUTODIV_HSDT1BUS		63
#define PCLK_GATE_IOC		64
#define CLK_ATDVFS		65
#define ATCLK		66
#define TRACKCLKIN		67
#define PCLK_DBG		68
#define PCLK_DIV_DBG		69
#define TRACKCLKIN_DIV		70
#define ACLK_GATE_PERF_STAT		71
#define PCLK_GATE_PERF_STAT		72
#define CLK_DIV_PERF_STAT		73
#define CLK_PERF_DIV_GT		74
#define CLK_GATE_PERF_STAT		75
#define CLK_DIV_CSSYSDBG		76
#define CLK_GATE_CSSYSDBG		77
#define CLK_DIV_DMABUS		78
#define CLK_GATE_DMAC		79
#define CLK_GATE_DMA_IOMCU		80
#define CLK_GATE_CSSYS_ATCLK		81
#define CLK_GATE_SOCP_ACPU		82
#define CLK_GATE_TCLK_SOCP		83
#define CLK_GATE_TIME_STAMP_GT		84
#define CLK_DIV_TIME_STAMP		85
#define CLK_GATE_TIME_STAMP		86
#define CLK_GATE_IPF		87
#define CLK_SDIO_PERI_GT		88
#define CLK_SDIO_PERI_DIV		89
#define CLK_SDIO_SYS		90
#define PCLK_GATE_DSI0		91
#define PCLK_GATE_DSI1		92
#define CLK_GATE_LDI0		93
#define CLK_GATE_FD_FUNC		94
#define CLK_GATE_SD		95
#define CLK_DIV_HSDT1BUS		96
#define CLK_MUX_SD_SYS		97
#define CLK_MUX_SD_PLL		98
#define CLK_DIV_SD		99
#define CLK_ANDGT_SD		100
#define CLK_SD_SYS		101
#define CLK_SD_SYS_GT		102
#define CLK_MUX_A53HPM		103
#define CLK_A53HPM_ANDGT		104
#define CLK_DIV_A53HPM		105
#define CLK_MUX_320M		106
#define CLK_320M_PLL_GT		107
#define CLK_DIV_320M		108
#define CLK_GATE_UART1		109
#define CLK_GATE_UART4		110
#define PCLK_GATE_UART1		111
#define PCLK_GATE_UART4		112
#define CLK_MUX_UARTH		113
#define CLK_DIV_UARTH		114
#define CLK_ANDGT_UARTH		115
#define CLK_GATE_UART2		116
#define CLK_GATE_UART5		117
#define PCLK_GATE_UART2		118
#define PCLK_GATE_UART5		119
#define CLK_MUX_UARTL		120
#define CLK_DIV_UARTL		121
#define CLK_ANDGT_UARTL		122
#define CLK_GATE_UART0		123
#define PCLK_GATE_UART0		124
#define CLK_MUX_UART0		125
#define CLK_DIV_UART0		126
#define CLK_ANDGT_UART0		127
#define CLK_FACTOR_UART0		128
#define CLK_UART0_DBG		129
#define CLK_GATE_I2C3		130
#define CLK_GATE_I2C4		131
#define CLK_GATE_I2C6_ACPU		132
#define CLK_GATE_I2C7		133
#define PCLK_GATE_I2C3		134
#define PCLK_GATE_I2C4		135
#define PCLK_GATE_I2C6_ACPU		136
#define PCLK_GATE_I2C7		137
#define CLK_DIV_I2C		138
#define CLK_MUX_I2C		139
#define CLK_ANDGT_I3C4		140
#define CLK_DIV_I3C4		141
#define CLK_GATE_I3C4		142
#define PCLK_GATE_I3C4		143
#define CLK_GATE_SPI1		144
#define CLK_GATE_SPI4		145
#define PCLK_GATE_SPI1		146
#define PCLK_GATE_SPI4		147
#define CLK_MUX_SPI		148
#define CLK_DIV_SPI		149
#define CLK_ANDGT_SPI		150
#define CLK_FACTOR_USB3PHY_PLL		151
#define CLK_USB2PHY_REF_DIV		152
#define CLK_GATE_ABB_USB		153
#define CLK_GATE_UFSPHY_REF		154
#define CLK_GATE_UFSIO_REF		155
#define CLK_GATE_AO_ASP		156
#define CLK_DIV_AO_ASP		157
#define CLK_MUX_AO_ASP		158
#define CLK_DIV_AO_ASP_GT		159
#define PCLK_GATE_PCTRL		160
#define CLK_ANDGT_PTP		161
#define CLK_DIV_PTP		162
#define CLK_GATE_PWM		163
#define CLK_GATE_BLPWM		164
#define CLK_SYSCNT_DIV		165
#define CLK_GATE_GPS_REF		166
#define CLK_MUX_GPS_REF		167
#define CLK_GATE_MDM2GPS0		168
#define CLK_GATE_MDM2GPS1		169
#define CLK_GATE_MDM2GPS2		170
#define PERI_VOLT_HOLD		171
#define PERI_VOLT_MIDDLE		172
#define PERI_VOLT_LOW		173
#define EPS_VOLT_HIGH		174
#define EPS_VOLT_MIDDLE		175
#define EPS_VOLT_LOW		176
#define VENC_VOLT_HOLD		177
#define VDEC_VOLT_HOLD		178
#define EDC_VOLT_HOLD		179
#define EFUSE_VOLT_HOLD		180
#define LDI0_VOLT_HOLD		181
#define HISE_VOLT_HOLD		182
#define CLK_GT_DPCTRL_16M		183
#define CLK_FIX_DIV_DPCTRL		184
#define CLK_DIV_DPCTRL_16M		185
#define CLK_GATE_DPCTRL_16M		186
#define CLK_GT_ISP_I2C		187
#define CLK_DIV_ISP_I2C		188
#define CLK_GATE_ISP_I2C_MEDIA		189
#define CLK_GATE_ISP_SNCLK0		190
#define CLK_GATE_ISP_SNCLK1		191
#define CLK_GATE_ISP_SNCLK2		192
#define CLK_GATE_ISP_SNCLK3		193
#define CLK_ISP_SNCLK_MUX0		194
#define CLK_ISP_SNCLK_DIV0		195
#define CLK_ISP_SNCLK_MUX1		196
#define CLK_ISP_SNCLK_DIV1		197
#define CLK_ISP_SNCLK_MUX2		198
#define CLK_ISP_SNCLK_DIV2		199
#define CLK_ISP_SNCLK_MUX3		200
#define CLK_ISP_SNCLK_DIV3		201
#define CLK_ISP_SNCLK_FAC		202
#define CLK_ISP_SNCLK_ANGT		203
#define CLK_GATE_TXDPHY0_CFG		204
#define CLK_GATE_TXDPHY0_REF		205
#define CLK_GATE_TXDPHY1_CFG		206
#define CLK_GATE_TXDPHY1_REF		207
#define PCLK_GATE_LOADMONITOR		208
#define CLK_GATE_LOADMONITOR		209
#define CLK_DIV_LOADMONITOR		210
#define CLK_GT_LOADMONITOR		211
#define PCLK_GATE_LOADMONITOR_1		212
#define CLK_GATE_LOADMONITOR_1		213
#define PCLK_GATE_LOADMONITOR_2		214
#define CLK_GATE_LOADMONITOR_2		215
#define CLK_GATE_MEDIA_TCXO		216
#define CLK_GATE_ADE_FUNC		217
#define CLK_AO_HIFD_MUX		218
#define CLK_ANDGT_AO_HIFD		219
#define CLK_DIV_AO_HIFD		220
#define CLK_GATE_AO_HIFD		221
#define CLK_GATE_SPE_GT		222
#define CLK_DIV_SPE		223
#define CLK_GATE_SPE_REF		224
#define HCLK_GATE_SPE		225
#define CLK_GATE_SPE		226
#define CLK_GATE_AXI_MEM_GS		227
#define CLK_GATE_AXI_MEM		228
#define ACLK_GATE_AXI_MEM		229
#define CLK_GATE_MAA_REF		230
#define ACLK_GATE_MAA		231
#define CLK_UART6		232
#define CLK_GATE_I2C0		233
#define CLK_GATE_I2C1		234
#define CLK_GATE_I2C2		235
#define CLK_GATE_SPI0		236
#define CLK_FAC_180M		237
#define CLK_GATE_IOMCU_PERI0		238
#define CLK_GATE_SPI2		239
#define CLK_GATE_UART3		240
#define CLK_GATE_UART8		241
#define CLK_GATE_UART7		242
#define OSC32K		243
#define OSC19M		244
#define CLK_480M		245
#define CLK_INVALID		246
#define AUTODIV_CFGBUS		247
#define AUTODIV_DMABUS		248
#define AUTODIV_ISP_DVFS		249
#define AUTODIV_ISP		250
#define CLK_GATE_ATDIV_HSDT1BUS		251
#define CLK_GATE_ATDIV_DMA		252
#define CLK_GATE_ATDIV_CFG		253
#define CLK_GATE_ATDIV_SYS		254
#define CLK_FPGA_1P92		255
#define CLK_FPGA_2M		256
#define CLK_FPGA_10M		257
#define CLK_FPGA_19M		258
#define CLK_FPGA_20M		259
#define CLK_FPGA_24M		260
#define CLK_FPGA_26M		261
#define CLK_FPGA_27M		262
#define CLK_FPGA_32M		263
#define CLK_FPGA_40M		264
#define CLK_FPGA_48M		265
#define CLK_FPGA_50M		266
#define CLK_FPGA_57M		267
#define CLK_FPGA_60M		268
#define CLK_FPGA_64M		269
#define CLK_FPGA_80M		270
#define CLK_FPGA_100M		271
#define CLK_FPGA_160M		272

/* clk_hsdt_crg */
#define CLK_GATE_PCIEPLL		0
#define CLK_GATE_PCIEPHY_REF		1
#define CLK_GATE_PCIE1PHY_REF		2
#define PCLK_GATE_PCIE_SYS		3
#define PCLK_GATE_PCIE1_SYS		4
#define PCLK_GATE_PCIE_PHY		5
#define PCLK_GATE_PCIE1_PHY		6
#define ACLK_GATE_PCIE		7
#define CLK_GATE_HSDT_TCU		8
#define CLK_GATE_HSDT_TBU		9
#define ACLK_GATE_PCIE1		10
#define CLK_GATE_HSDT_TBU1		11
#define CLK_GATE_HSDT_TCU1		12
#define CLK_GATE_PCIE0MCU		13
#define CLK_GATE_PCIE0MCU_BUS		14
#define CLK_GATE_PCIE0MCU_19P2		15
#define CLK_GATE_PCIE0MCU_32K		16
#define CLK_GATE_PCIE1MCU		17
#define CLK_GATE_PCIE1MCU_BUS		18
#define CLK_GATE_PCIE1MCU_19P2		19
#define CLK_GATE_PCIE1MCU_32K		20
#define CLK_GATE_SDIO		21
#define CLK_MUX_SDIO_SYS		22
#define CLK_DIV_SDIO		23
#define CLK_ANDGT_SDIO		24
#define CLK_SDIO_SYS_GT		25
#define HCLK_GATE_SDIO		26

/* clk_hsdt1_crg */
#define HCLK_GATE_USB3OTG		0
#define ACLK_GATE_USB3OTG		1
#define HCLK_GATE_SD		2
#define CLK_GATE_USB3OTG_REF		3
#define CLK_ANDGT_USB2PHY_REF		4
#define CLK_GATE_USB2PHY_REF		5
#define CLK_MUX_ULPI		6
#define CLK_GATE_ULPI_REF		7
#define CLK_HSDT1_EUSB_MUX		8
#define CLK_GATE_HSDT1_EUSB		9
#define CLK_GATE_USB2_ULPI		10
#define PCLK_GATE_USB_SYSCTRL		11
#define PCLK_GATE_DP_SYSCTRL		12
#define PCLK_GATE_USB2PHY		13
#define CLK_GATE_USBDPMCU_32K		14
#define CLK_GATE_USBDPMCU		15
#define CLK_GATE_USBDPMCU_BUS		16
#define CLK_GATE_USBDPMCU_19P2		17
#define CLK_GATE_USB31PHY_APB		18
#define PCLK_GATE_DPCTRL		19
#define ACLK_GATE_DPCTRL		20

/* clk_sctrl */
#define CLK_GATE_AUPLL		0
#define PCLK_GPIO20		1
#define PCLK_GPIO21		2
#define CLK_DIV_AOBUS		3
#define CLK_GATE_TIMER5_B		4
#define CLK_MUX_TIMER5_A		5
#define CLK_MUX_TIMER5_B		6
#define CLK_GATE_TIMER5		7
#define CLK_ANDGT_32KPLL_PCIEAUX		8
#define CLK_DIV_32KPLL_PCIEAUX		9
#define CLK_MUX_PCIEAUX		10
#define CLK_GATE_PCIEAUX		11
#define CLK_GATE_PCIEAUX1		12
#define CLK_GATE_SPI		13
#define PCLK_GATE_SPI		14
#define CLK_ANDGT_SPI3		15
#define CLK_DIV_SPI3		16
#define CLK_ANDGT_SPI3_FLL		17
#define DIV_SPI3_FLL		18
#define CLK_MUX_SPI3		19
#define CLK_GATE_SPI5		20
#define PCLK_GATE_SPI5		21
#define CLK_ANDGT_SPI5		22
#define CLK_DIV_SPI5		23
#define CLK_ANDGT_SPI5_FLL		24
#define DIV_SPI5_FLL		25
#define CLK_MUX_SPI5		26
#define PCLK_GATE_RTC		27
#define PCLK_AO_GPIO0		28
#define PCLK_AO_GPIO1		29
#define PCLK_AO_GPIO2		30
#define PCLK_AO_GPIO3		31
#define PCLK_AO_GPIO4		32
#define PCLK_AO_GPIO5		33
#define PCLK_AO_GPIO6		34
#define PCLK_AO_GPIO29		35
#define PCLK_AO_GPIO30		36
#define PCLK_AO_GPIO31		37
#define PCLK_AO_GPIO32		38
#define PCLK_AO_GPIO33		39
#define PCLK_AO_GPIO34		40
#define PCLK_AO_GPIO35		41
#define PCLK_AO_GPIO36		42
#define PCLK_GATE_SYSCNT		43
#define CLK_GATE_SYSCNT		44
#define CLKMUX_SYSCNT		45
#define CLK_ASP_BACKUP		46
#define CLKGT_ASP_CODEC		47
#define CLKDIV_ASP_CODEC		48
#define CLK_MUX_ASP_CODEC		49
#define CLK_ASP_CODEC		50
#define CLK_GATE_ASP_SUBSYS		51
#define CLK_MUX_ASP_PLL		52
#define CLK_AO_ASP_MUX		53
#define CLK_GATE_ASP_TCXO		54
#define CLK_GATE_DP_AUDIO_PLL		55
#define CLKDIV_DP_AUDIO_PLL_AO		56
#define CLKGT_DP_AUDIO_PLL_AO		57
#define CLK_MUX_AO_CAMERA		58
#define CLK_ANDGT_AO_CAMERA		59
#define CLK_DIV_AO_CAMERA		60
#define CLK_GATE_AO_CAMERA		61
#define CLK_GATE_RXDPHY0_CFG		62
#define CLK_GATE_RXDPHY1_CFG		63
#define CLK_GATE_RXDPHY2_CFG		64
#define CLK_GATE_RXDPHY3_CFG		65
#define CLK_GATE_RXDPHY4_CFG		66
#define CLK_GATE_RXDPHY_CFG		67
#define CLK_DIV_RXDPHY_CFG		68
#define CLKANDGT_RXDPHY_CFG		69
#define CLK_MUX_RXDPHY_CFG		70
#define PCLK_GATE_AO_LOADMONITOR		71
#define CLK_GATE_AO_LOADMONITOR		72
#define CLK_DIV_AO_LOADMONITOR		73
#define CLK_GT_AO_LOADMONITOR		74
#define CLK_SW_AO_LOADMONITOR		75
#define CLK_ANDGT_HIFD_FLL		76
#define CLK_DIV_HIFD_FLL		77
#define CLK_HIFD_PLL_MUX		78
#define CLK_ANDGT_HIFD_PLL		79
#define CLK_DIV_HIFD_PLL		80

/* clk_iomcu_crgctrl */
#define CLK_I2C1_GATE_IOMCU		0

/* clk_media1_crg */
#define PCLK_GATE_ISP_QIC_SUBSYS		0
#define ACLK_GATE_ISP_QIC_SUBSYS		1
#define ACLK_GATE_QIC_DSS		2
#define PCLK_GATE_QIC_DSS_CFG		3
#define PCLK_GATE_MMBUF_CFG		4
#define PCLK_GATE_DISP_QIC_SUBSYS		5
#define ACLK_GATE_DISP_QIC_SUBSYS		6
#define PCLK_GATE_DSS		7
#define ACLK_GATE_DSS		8
#define ACLK_GATE_ISP		9
#define CLK_MUX_VIVOBUS		10
#define CLK_GATE_VIVOBUS_ANDGT		11
#define CLK_DIV_VIVOBUS		12
#define CLK_GATE_VIVOBUS		13
#define CLK_MUX_EDC0		14
#define CLK_ANDGT_EDC0		15
#define CLK_DIV_EDC0		16
#define CLK_GATE_EDC0FREQ		17
#define CLK_MUX_LDI1		18
#define CLK_ANDGT_LDI1		19
#define CLK_DIV_LDI1		20
#define CLK_GATE_LDI1		21
#define CLK_MUX_ISPI2C		22
#define CLK_GATE_ISPI2C		23
#define CLK_GATE_ISP_SYS		24
#define CLK_MUX_ISPCPU		25
#define CLK_ANDGT_ISPCPU		26
#define CLK_DIV_ISPCPU		27
#define CLK_GATE_ISPCPU		28
#define CLK_ANDGT_ISP_I3C		29
#define CLK_DIV_ISP_I3C		30
#define CLK_GATE_ISP_I3C		31
#define CLK_MUX_ISPFUNC		32
#define CLK_ANDGT_ISPFUNC		33
#define CLK_DIV_ISPFUNC		34
#define CLK_GATE_ISPFUNCFREQ		35
#define CLK_MUX_ISPFUNC2		36
#define CLK_ANDGT_ISPFUNC2		37
#define CLK_DIV_ISPFUNC2		38
#define CLK_GATE_ISPFUNC2FREQ		39
#define CLK_MUX_ISPFUNC5		40
#define CLK_ANDGT_ISPFUNC5		41
#define CLK_DIV_ISPFUNC5		42
#define CLK_GATE_ISPFUNC5FREQ		43
#define PCLK_GATE_MEDIA1_LM		44
#define CLK_GATE_LOADMONITOR_MEDIA1		45
#define ACLK_GATE_ASC		46
#define CLK_GATE_DSS_AXI_MM		47
#define CLK_GATE_MMBUF		48
#define PCLK_GATE_MMBUF		49
#define CLK_SW_MMBUF		50
#define ACLK_DIV_MMBUF		51
#define CLK_MMBUF_PLL_ANDGT		52
#define PCLK_DIV_MMBUF		53
#define PCLK_MMBUF_ANDGT		54
#define CLK_GATE_ATDIV_VIVO		55
#define CLK_GATE_ATDIV_ISPCPU		56

/* clk_media2_crg */
#define CLK_GATE_VCODECBUS		0
#define CLK_GATE_VCODECBUS2DDR		1
#define CLK_DIV_VCODECBUS		2
#define CLK_GATE_VCODECBUS_GT		3
#define CLK_VCODEC_SYSPLL0		4
#define CLK_MUX_VCODECBUS		5
#define CLK_MUX_VDEC		6
#define CLK_ANDGT_VDEC		7
#define CLK_DIV_VDEC		8
#define CLK_GATE_VDECFREQ		9
#define PCLK_GATE_VDEC		10
#define ACLK_GATE_VDEC		11
#define CLK_MUX_VENC		12
#define CLK_ANDGT_VENC		13
#define CLK_DIV_VENC		14
#define CLK_GATE_VENCFREQ		15
#define PCLK_GATE_VENC		16
#define ACLK_GATE_VENC		17
#define PCLK_GATE_MEDIA2_LM		18
#define CLK_GATE_LOADMONITOR_MEDIA2		19
#define CLK_GATE_IVP32DSP_TCXO		20
#define CLK_MUX_IVP32DSP_CORE		21
#define CLK_ANDGT_IVP32DSP_CORE		22
#define CLK_DIV_IVP32DSP_CORE		23
#define CLK_GATE_IVP32DSP_COREFREQ		24
#define CLK_JPG_FUNC_MUX		25
#define CLK_ANDGT_JPG_FUNC		26
#define CLK_DIV_JPG_FUNC		27
#define CLK_GATE_JPG_FUNCFREQ		28
#define ACLK_GATE_JPG		29
#define PCLK_GATE_JPG		30
#define CLK_GATE_AUTODIV_VCODECBUS		31
#define CLK_GATE_ATDIV_VDEC		32
#define CLK_GATE_ATDIV_VENC		33

/* clk_xfreqclk */
#define CLK_CLUSTER0		0
#define CLK_CLUSTER1		1
#define CLK_G3D		2
#define CLK_DDRC_FREQ		3
#define CLK_DDRC_MAX		4
#define CLK_DDRC_MIN		5

/* clk_pmuctrl */
#define CLK_GATE_ABB_192		0
#define CLK_PMU32KA		1
#define CLK_PMU32KB		2
#define CLK_PMU32KC		3
#define CLK_PMUAUDIOCLK		4
#define CLK_EUSB_38M4		5

/* dvfs_clock */
#define CLK_GATE_EDC0		0
#define CLK_GATE_VDEC		1
#define CLK_GATE_VENC		2
#define CLK_GATE_ISPFUNC		3
#define CLK_GATE_ISPFUNC2		4
#define CLK_GATE_ISPFUNC3		5
#define CLK_GATE_ISPFUNC4		6
#define CLK_GATE_ISPFUNC5		7
#define CLK_GATE_JPG_FUNC		8
#define CLK_GATE_IVP32DSP_CORE		9
#define CLK_GATE_HIFACE		10

/* clk_fast_clock */
#define CLK_GATE_ISPFUNC3FREQ		0
#define CLK_GATE_ISPFUNC4FREQ		1

#endif	/* __CLK_LEX_H */