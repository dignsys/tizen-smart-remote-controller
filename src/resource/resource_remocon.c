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

#include <Ecore.h>
#include <peripheral_io.h>
#include <unistd.h>
#include "remote_key.h"
#include "log.h"

#define PRE_DATA_BITS		16
#define PRE_DATA			0xE0E0
#define BITS_NUM			16

// for BN59-01180A
#define HEADER_MARK			4443	//4900
#define HEADER_SPACE		4569	//4900

#define MARK_ONE			492		//590
#define SPACE_ONE			1749	//1690
#define MARK_ZERO			492		//590
#define SPACE_ZERO			630		//590

#define STOP_MARK			493		//590
#define STOP_SPACE			590

cmd_t cmd_table[] = {
	{0, "TV_KEY_POWER",       TV_KEY_POWER},
	{1, "TV_KEY_CHANNELUP",   TV_KEY_CHANNELUP},
	{2, "TV_KEY_CHANNELDOWN", TV_KEY_CHANNELDOWN},
	{3, "TV_KEY_VOLUMEUP",    TV_KEY_VOLUMEUP},
	{4, "TV_KEY_VOLUMEDOWN",  TV_KEY_VOLUMEDOWN},
	// not used keys
	{5, "TV_KEY_MENU",       TV_KEY_MENU},
	{6, "TV_KEY_UP",         TV_KEY_UP},
	{7, "TV_KEY_DOWN",       TV_KEY_DOWN},
	{8, "TV_KEY_LEFT",       TV_KEY_LEFT},
	{9, "TV_KEY_RIGHT",      TV_KEY_RIGHT},

	// Robot Vacuum cleaner
	{10, "VA_KEY_UP",        VA_KEY_UP},
	{11, "VA_KEY_DOWN",      VA_KEY_DOWN},
	{12, "VA_KEY_RIGHT",     VA_KEY_RIGHT},
	{13, "VA_KEY_LEFT",      VA_KEY_LEFT},
	{14, "VA_KEY_START",     VA_KEY_START},
	{15, "VA_KEY_RANDOM",    VA_KEY_RANDOM},
	{16, "VA_KEY_CIRCLE",    VA_KEY_CIRCLE},
	{17, "VA_KEY_WALL",      VA_KEY_WALL},
	{18, "VA_KEY_SCHED",     VA_KEY_SCHED},
	{19, "VA_KEY_HOME",      VA_KEY_HOME},
};

#define ROBOT_VACUUM_KEY_INDEX 10

extern peripheral_error_e resource_transmit_data(bool enable);
extern void write_led(bool on);
extern bool process_vacuum_key(int index);

void mysleep_microsec(int microsec)
{
	float div;
	div = microsec * 0.88;
	microsec = (int)div;

    struct timespec res;
    res.tv_sec = microsec/1000000;
    res.tv_nsec = (microsec*1000) % 1000000000;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &res, NULL);
}

void mark(unsigned int time)
{
	resource_transmit_data(true);
	//usleep(time);
	mysleep_microsec(time);
}

void space(unsigned int time)
{
	resource_transmit_data(false);
	//usleep(time);
	mysleep_microsec(time);
}

/*
 * IR protocol encodes the keys using a 32bit frame format as shown below.
 *
 * Frame Format
 * Address      | Complement of Address | Command        | Complement of Command
 * LSB-MSB(0-7) | LSB-MSB(8-15)         | LSB-MSB(16-23) | LSB-MSB(24-31)
 *
 * https://www.vishay.com/docs/80071/dataform.pdf
 */
void send_key_data(uint16_t key_value)
{
	int nbits = PRE_DATA_BITS + BITS_NUM;
	unsigned long data = 0;

	// add address and command into data
	data = (PRE_DATA << PRE_DATA_BITS) + key_value;

	// Data processing
	for (unsigned long  mask = 1UL << (nbits - 1);  mask;  mask >>= 1) {
		if (data & mask) {
			// for bit 1
			mark(MARK_ONE);
			space(SPACE_ONE);
		} else {
			// for bit 0
			mark(MARK_ZERO);
			space(SPACE_ZERO);
		}
	}
}

static void send_key_once(uint16_t key_value)
{
	// send_header_bit
	mark(HEADER_MARK);
	space(HEADER_SPACE);

	// send_data
	send_key_data(key_value);

	// send_stop_bit
	mark(STOP_MARK);
	space(STOP_SPACE);
}

bool send_remote_key_data(int index)
{
	INFO("%d : %s : 0x%04x", index, cmd_table[index].cmd, cmd_table[index].key_value);

	if (index >= ROBOT_VACUUM_KEY_INDEX) {
		// defined for AC Remote
		//if (process_vacuum_key(cmd_table[index].key_value))
		if (process_vacuum_key(index))
			return true;
		else
			return false;
	}


	// Turn ON led to indicate ir transmit is activated
	write_led(true);

	// samsung TV remote controller send key data twice
	// send first key data
	send_key_once(cmd_table[index].key_value);

	// wait for a while
	mysleep_microsec(52*1000);

	// send again same key data
	send_key_once(cmd_table[index].key_value);

	// Turn OFF led to indicate ir transmit is ended
	write_led(false);

	return true;
}

bool process_command(int length, char *cmd)
{
	int index;
	size_t size = sizeof(cmd_table) / sizeof(cmd_table[0]);
	//INFO("size of cmd_table = [%d]", size);

	for (index = 0; index < size; index++) {
		if (0 == strcmp(cmd, cmd_table[index].cmd)) {
			INFO("cmd [%s] : index [%d] - key [%s]", cmd, index, cmd_table[index].cmd);
			return send_remote_key_data(index);
		}
	}

	return false;
}
