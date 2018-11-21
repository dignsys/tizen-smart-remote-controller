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

// Samsung TV Remote code
// Remote Model : BN59-01180A
#define TV_KEY_POWER			0x40BF
#define TV_KEY_VOLUMEUP			0xE01F
#define TV_KEY_VOLUMEDOWN		0xD02F
#define TV_KEY_CHANNELUP		0x48B7
#define TV_KEY_CHANNELDOWN		0x08F7
#define TV_KEY_MENU				0x58A7
#define TV_KEY_UP				0x06F9
#define TV_KEY_DOWN				0x8679
#define TV_KEY_LEFT				0xA659
#define TV_KEY_RIGHT			0x46B9

// Robot Vacuum Cleaner
// model : Philips FC8794
#define VA_KEY_UP		0xC03F
#define VA_KEY_DOWN		0x609F
#define VA_KEY_RIGHT	0xA05F
#define VA_KEY_LEFT		0x20DF
#define VA_KEY_START	0x807F
#define VA_KEY_RANDOM	0xE01F
#define VA_KEY_CIRCLE	0x10EF
#define VA_KEY_WALL		0x50AF
#define VA_KEY_SCHED	0x906F
#define VA_KEY_HOME		0x40BF

typedef struct {
	int index;
	char cmd[20];
	uint16_t key_value;
} cmd_t;
