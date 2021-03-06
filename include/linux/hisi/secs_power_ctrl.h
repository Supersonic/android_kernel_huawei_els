/*
 * secs_power_ctrl.h
 *
 * Copyright (c) 2001-2021, Huawei Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __SECS_POWER_CTRL_H__
#define __SECS_POWER_CTRL_H__

int hisi_secs_power_on(void);
int hisi_secs_power_down(void);
unsigned long get_secs_suspend_status(void);

#endif /* end of secs_power_ctrl.h */
