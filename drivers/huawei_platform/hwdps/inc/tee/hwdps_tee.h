/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: This file contains the function required for communicating witt
 *              TEE
 * Create: 2020-11-10
 */

#ifndef _HWDPS_TEE_H
#define _HWDPS_TEE_H
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include "teek_client_api.h"

#define HWDPS_TA_UUID { 0xb0b71695, 0x2913, 0x4fc1, \
	{ 0x8e, 0x7f, 0x42, 0x7d, 0x92, 0x21, 0x22, 0x47 } }

typedef enum {
	HWDPS_INIT_USER = 10,
	SET_CREDENTIAL,
	CLEAR_CREDENTIAL,
} hwdps_ta_cmd;

struct async_send_work_t {
	struct work_struct work;
	wait_queue_head_t notify_waitq;
	TEEC_Operation *op;
	TEEC_Session *sess;
	hwdps_ta_cmd cmd;
	TEEC_Result result;
	s32 response;
};

s32 hwdps_init_tee(void);

s32 send_credential_request(u64 profile_id, const u8 *cred,
	u32 cred_length, hwdps_ta_cmd cmd, u8 **phase1_key,
	u32 *phase1_key_size);

bool init_hwdps_work_queue(void);

void destory_hwdps_work_queue(void);
#endif
