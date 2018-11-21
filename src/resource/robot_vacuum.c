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

#include <stdbool.h>
#include <stdint.h>
#include "remote_key.h"
#include "log.h"

#define VA_PRE_DATA				0xA2AA0A
#define VA_BITS_NUM				16

// for Robot Vacuum cleaner
#define VA_HEADER_MARK			9000
#define VA_HEADER_SPACE			4570

#define VA_MARK_ONE				510
#define VA_SPACE_ONE			1745
#define VA_MARK_ZERO			520
#define VA_SPACE_ZERO			630

#define VA_STOP_MARK			510
#define VA_STOP_SPACE			47000

#define VA_EXTRA_MARK			8980
#define VA_EXTRA_SPACE			2298
#define VA_EXTRA_PULSE			570

extern void mark(unsigned int time);
extern void space(unsigned int time);
extern void write_led(bool on);
extern cmd_t cmd_table[];

unsigned char pre_key[3] = { 0xA2, 0xAA, 0x0A };

/*
 * Philips FC8794 Robot vacuum cleaner remote controller
 * remote code is composed with 5 bytes
 */
void send_vacuum_key_data(int index)
{
	int nbits = 8;
	uint8_t data = 0;
	int i=0;

	// send pre data first
	for (i=0; i<3; i++) {
		data = pre_key[i];
		for (uint8_t mask = 1 << (nbits - 1);  mask;  mask >>= 1) {
			if (data & mask) {
				// for bit 1
				mark(VA_MARK_ONE);
				space(VA_SPACE_ONE);
			} else {
				// for bit 0
				mark(VA_MARK_ZERO);
				space(VA_SPACE_ZERO);
			}
		}
	}

	// send key_value next
	uint16_t key_value = cmd_table[index].key_value;
	nbits = VA_BITS_NUM;

	for (uint16_t mask = 1 << (nbits - 1);  mask;  mask >>= 1) {
		if (key_value & mask) {
			// for bit 1
			mark(VA_MARK_ONE);
			space(VA_SPACE_ONE);
		} else {
			// for bit 0
			mark(VA_MARK_ZERO);
			space(VA_SPACE_ZERO);
		}
	}
}

bool process_vacuum_key(int index)
{
	// Turn ON led to indicate ir transmit is activated
	write_led(true);

	// send_header_bit
	mark(VA_HEADER_MARK);
	space(VA_HEADER_SPACE);

	// send_data
	send_vacuum_key_data(index);

	// send_stop_bit
	mark(VA_STOP_MARK);
	space(VA_STOP_SPACE);

	// Turn OFF led to indicate ir transmit is ended
	write_led(false);

	return true;
}



