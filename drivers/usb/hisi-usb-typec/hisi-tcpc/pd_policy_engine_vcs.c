/*
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
 *
 * Copyright (C) 2016 Richtek Technology Corp.
 * Author: TH <tsunghan_tsai@richtek.com>
 *
 * Power Delivery Policy Engine for VCS
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

#define LOG_TAG "[PE_VCS]"

#include "include/pd_core.h"
#include "include/pd_tcpm.h"
#include "include/pd_dpm_core.h"
#include "include/tcpci.h"
#include "include/pd_policy_engine.h"

void hisi_pe_vcs_send_swap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_VCONN_SWAP);
}

void hisi_pe_vcs_evaluate_swap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_dpm_vcs_evaluate_swap(pd_port);
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_vcs_accept_swap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_ACCEPT);
}

void hisi_pe_vcs_reject_vconn_swap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	if (pd_event->msg_sec == PD_DPM_NAK_REJECT)
		hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_REJECT);
	else
		hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_WAIT);
}

void hisi_pe_vcs_wait_for_vconn_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	pd_enable_timer(pd_port, PD_TIMER_VCONN_ON);
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_vcs_wait_for_vconn_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	pd_disable_timer(pd_port, PD_TIMER_VCONN_ON);
}

void hisi_pe_vcs_turn_off_vconn_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_vcs_enable_vconn(pd_port, false);
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_vcs_turn_on_vconn_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_dpm_vcs_enable_vconn(pd_port, true);
	pd_free_pd_event(pd_port, pd_event);
	D("wait 10ms before send ps_rdy\n");
	msleep(10); /* 10: msleep time */
}

void hisi_pe_vcs_send_ps_rdy_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_PS_RDY);
}
