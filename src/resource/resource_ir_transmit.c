/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "log.h"
#include <peripheral_io.h>

#define ARTIK_PWM_CHIPID	0
#define ARTIK_PWM_PIN		2

#define MICRO_SECOND	(1000)

static peripheral_pwm_h g_pwm_h = NULL;

peripheral_error_e resource_irtx_close(void)
{
	peripheral_error_e ret = PERIPHERAL_ERROR_NONE;

	if (g_pwm_h != NULL) {
		// Closing a PWM Handle : close a PWM handle that is no longer used,
		if ((ret = peripheral_pwm_close(g_pwm_h)) != PERIPHERAL_ERROR_NONE ) {
			ERR("peripheral_pwm_close() failed!![%d]", ret);
			return ret;
		}
		g_pwm_h = NULL;
	}

	return ret;
}

peripheral_error_e resource_irtx_init(void)
{
	peripheral_error_e ret = PERIPHERAL_ERROR_NONE;
	INFO("resource_irtx_init...");

	int chip = ARTIK_PWM_CHIPID;	// Chip 0
	int pin  = ARTIK_PWM_PIN;		// Pin 2

	int period = 26 * MICRO_SECOND;	// micro - 38kHz
	int duty_cycle = period / 2;	// 50% duty

	if (g_pwm_h == NULL){
		// Opening a PWM Handle : The chip and pin parameters required for this function must be set
		if ((ret = peripheral_pwm_open(chip, pin, &g_pwm_h)) != PERIPHERAL_ERROR_NONE ) {
			ERR("peripheral_pwm_open() failed!![%d]", ret);
			return ret;
		}
	}

	// Setting the Period
	if ((ret = peripheral_pwm_set_period(g_pwm_h, period)) != PERIPHERAL_ERROR_NONE) {
		ERR("peripheral_pwm_set_period() failed!![%d]", ret);
		return ret;
	}

	// Setting the Duty Cycle
	if ((ret = peripheral_pwm_set_duty_cycle(g_pwm_h, duty_cycle)) != PERIPHERAL_ERROR_NONE) {
		ERR("peripheral_pwm_set_duty_cycle() failed!![%d]", ret);
		return ret;
	}

	// Setting the Polarity
	//if ((ret = peripheral_pwm_set_polarity(g_pwm_h, PERIPHERAL_PWM_POLARITY_ACTIVE_HIGH)) != PERIPHERAL_ERROR_NONE) {
	if ((ret = peripheral_pwm_set_polarity(g_pwm_h, PERIPHERAL_PWM_POLARITY_ACTIVE_LOW)) != PERIPHERAL_ERROR_NONE) {
		ERR("peripheral_pwm_set_polarity() failed!![%d]", ret);
		return ret;
	}

	return ret;
}

peripheral_error_e resource_transmit_data(bool enable)
{
	peripheral_error_e ret = PERIPHERAL_ERROR_NONE;

	if (g_pwm_h == NULL){
		ERR("resource_transmit_data() failed!![%d]", ret);
		return ret;
	}

	// Enabling
	if ((ret = peripheral_pwm_set_enabled(g_pwm_h, enable)) != PERIPHERAL_ERROR_NONE) {
		ERR("peripheral_pwm_set_enabled() failed!![%d]", ret);
		return ret;
	}

	return ret;
}
