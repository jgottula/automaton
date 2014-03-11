/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_CAN_CAN_H
#define AUTOMATON_DEV_CAN_CAN_H


#include "std.h"


enum {
	/*C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,
	C_REG_ = 0x,*/
	C_REG_CANSTAT = 0xee,
	C_REG_CANCTRL = 0xff,
};


void can_init(void);

void can_cmd_reset(void);

uint8_t can_cmd_status(void);
uint8_t can_cmd_rx_status(void);

void can_cmd_rts(uint8_t mask);

uint8_t can_cmd_read(uint8_t addr);
void can_cmd_write(uint8_t addr, uint8_t data);
void can_cmd_bit_modify(uint8_t addr, uint8_t mask, uint8_t data);

void can_cmd_read_many(uint8_t addr, uint8_t len, uint8_t data[static len]);
void can_cmd_write_many(uint8_t addr, uint8_t len,
	const uint8_t data[static len]);

void can_cmd_read_rx_buf(uint8_t start, uint8_t len, uint8_t data[static len]);
void can_cmd_load_tx_buf(uint8_t start, uint8_t len,
	const uint8_t data[static len]);


#endif
