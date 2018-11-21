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

#define GREEN_LED				129		// GPIO1
#define RED_LED					128		// GPIO0
#define LED_ON					1		// High
#define LED_OFF					0		// Low

static peripheral_gpio_h green_led_h = NULL;
static peripheral_gpio_h red_led_h = NULL;

int resource_close_led(peripheral_gpio_h handle)
{
	int ret = PERIPHERAL_ERROR_NONE;

	INFO("LED is finishing...");
	ret = peripheral_gpio_close(handle);
	if (ret != PERIPHERAL_ERROR_NONE) {
		ERR("peripheral_gpio_close failed");
	}
	return ret;
}

int resource_open_led(int gpio_pin, peripheral_gpio_h *handle)
{
	int ret = PERIPHERAL_ERROR_NONE;

	INFO("LED(gpio_pin:%d) is opening...", gpio_pin);
	ret = peripheral_gpio_open(gpio_pin, handle);
	if (ret != PERIPHERAL_ERROR_NONE) {
		ERR("peripheral_gpio_open failed, ret=[%d]", ret);
	}

	ret = peripheral_gpio_set_direction(*handle, PERIPHERAL_GPIO_DIRECTION_OUT_INITIALLY_LOW);
	if (ret != PERIPHERAL_ERROR_NONE) {
		ERR("peripheral_gpio_set_direction failed, ret=[%d]", ret);
	}

	return ret;
}

int resource_write_led(peripheral_gpio_h handle, int write_value)
{
	int ret = PERIPHERAL_ERROR_NONE;

	if (!handle) {
		ERR("peripheral_gpio_open failed, ret=[%d]", ret);
		return -1;
	}

	ret = peripheral_gpio_write(handle, write_value);
	if (ret != PERIPHERAL_ERROR_NONE) {
		ERR("peripheral_gpio_write failed, ret=[%d]", ret);
		return -1;
	}

	//INFO("LED Value : %s", write_value ? "ON":"OFF");

	return 0;
}

void write_led(bool on)
{
	//INFO("write_led [%d]", on);
	if (on == true) {
		resource_write_led(green_led_h, LED_ON);
		resource_write_led(red_led_h, LED_OFF);
	} else {
		resource_write_led(green_led_h, LED_OFF);
		resource_write_led(red_led_h, LED_ON);
	}
}

int open_led_dev(void)
{
	int ret = PERIPHERAL_ERROR_NONE;

	ret = resource_open_led(GREEN_LED, &green_led_h);
	if (ret != PERIPHERAL_ERROR_NONE) {
		ERR("resource_open_led failed, ret=[%d]", ret);
		return ret;
	}

	ret = resource_open_led(RED_LED, &red_led_h);
	if (ret != PERIPHERAL_ERROR_NONE) {
		ERR("resource_open_led failed, ret=[%d]", ret);
		return ret;
	}

	return ret;
}

int close_led_dev(void)
{
	int ret = PERIPHERAL_ERROR_NONE;

	if (green_led_h != NULL) {
		ret = resource_close_led(green_led_h);
		if (ret != PERIPHERAL_ERROR_NONE) {
			ERR("resource_close_led failed, ret=[%d]", ret);
			return ret;
		}
	}
	if (red_led_h != NULL) {
		ret = resource_close_led(red_led_h);
		if (ret != PERIPHERAL_ERROR_NONE) {
			ERR("resource_close_led failed, ret=[%d]", ret);
			return ret;
		}
	}

	return ret;
}
