/*
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
 *
 * Copyright (C) 2016 Richtek Technology Corp.
 * Author: TH <tsunghan_tsai@richtek.com>
 *
 * Power Delivery Policy Engine for DRS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#define LOG_TAG "[PE_DRS]"

#include "include/pd_core.h"
#include "include/pd_tcpm.h"
#include "include/pd_dpm_core.h"
#include "include/tcpci.h"
#include "include/pd_policy_engine.h"

void hisi_pe_drs_dfp_ufp_evaluate_dr_swap_entry(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_dpm_drs_evaluate_swap(pd_port, PD_ROLE_UFP);
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_drs_dfp_ufp_accept_dr_swap_entry(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_set_rx_enable(pd_port, PD_RX_CAP_PE_READY_UFP);
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_ACCEPT);
}

void hisi_pe_drs_dfp_ufp_change_to_ufp_entry(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_dpm_drs_change_role(pd_port, PD_ROLE_UFP);
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_drs_dfp_ufp_send_dr_swap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_set_rx_enable(pd_port, PD_RX_CAP_PE_READY_UFP);
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_DR_SWAP);
}

void hisi_pe_drs_dfp_ufp_reject_dr_swap_entry(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	if (pd_event->msg_sec == PD_DPM_NAK_REJECT)
		hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_REJECT);
	else
		hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_WAIT);
}

/*
 * [PD2.0] Figure 8-50: Type-C UFP to DFP Data Role Swap State Diagram
 */

void hisi_pe_drs_ufp_dfp_evaluate_dr_swap_entry(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_dpm_drs_evaluate_swap(pd_port, PD_ROLE_DFP);
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_drs_ufp_dfp_accept_dr_swap_entry(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_ACCEPT);
}

void hisi_pe_drs_ufp_dfp_change_to_dfp_entry(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_dpm_drs_change_role(pd_port, PD_ROLE_DFP);
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_drs_ufp_dfp_send_dr_swap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_DR_SWAP);
}

void hisi_pe_drs_ufp_dfp_reject_dr_swap_entry(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	if (pd_event->msg_sec == PD_DPM_NAK_REJECT)
		hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_REJECT);
	else
		hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_WAIT);
}
