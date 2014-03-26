/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_CAN_CAN_H
#define AUTOMATON_DEV_CAN_CAN_H


#include "std.h"


enum {
	C_REG_TEC      = 0b00011100,
	C_REG_REC      = 0b00011101,
	
	C_REG_CNF3     = 0b00101000,
	C_REG_CNF2     = 0b00101001,
	C_REG_CNF1     = 0b00101010,
	C_REG_CANINTE  = 0b00101011,
	C_REG_CANINTF  = 0b00101100,
	C_REG_EFLG     = 0b00101101,
	
	C_REG_TXB0CTRL = 0b00110000,
	C_REG_TXB0SIDH = 0b00110001,
	C_REG_TXB0SIDL = 0b00110010,
	C_REG_TXB0DLC  = 0b00110101,
	C_REG_TXB0D0   = 0b00110110,
	C_REG_TXB0D1   = 0b00110111,
	C_REG_TXB0D2   = 0b00111000,
	C_REG_TXB0D3   = 0b00111001,
	C_REG_TXB0D4   = 0b00111010,
	C_REG_TXB0D5   = 0b00111011,
	C_REG_TXB0D6   = 0b00111100,
	C_REG_TXB0D7   = 0b00111101,
	
	C_REG_RXB0CTRL = 0b01100000,
	C_REG_RXB0SIDH = 0b01100001,
	C_REG_RXB0SIDL = 0b01100010,
	C_REG_RXB0EID8 = 0b01100011,
	C_REG_RXB0EID0 = 0b01100100,
	C_REG_RXB0DLC  = 0b01100101,
	C_REG_RXB0D0   = 0b01100110,
	C_REG_RXB0D1   = 0b01100111,
	C_REG_RXB0D2   = 0b01101000,
	C_REG_RXB0D3   = 0b01101001,
	C_REG_RXB0D4   = 0b01101010,
	C_REG_RXB0D5   = 0b01101011,
	C_REG_RXB0D6   = 0b01101100,
	C_REG_RXB0D7   = 0b01101101,
	
	C_REG_RXB1CTRL = 0b01110000,
	C_REG_RXB1SIDH = 0b01110001,
	C_REG_RXB1SIDL = 0b01110010,
	C_REG_RXB1EID8 = 0b01110011,
	C_REG_RXB1EID0 = 0b01110100,
	C_REG_RXB1DLC  = 0b01110101,
	C_REG_RXB1D0   = 0b01110110,
	C_REG_RXB1D1   = 0b01110111,
	C_REG_RXB1D2   = 0b01111000,
	C_REG_RXB1D3   = 0b01111001,
	C_REG_RXB1D4   = 0b01111010,
	C_REG_RXB1D5   = 0b01111011,
	C_REG_RXB1D6   = 0b01111100,
	C_REG_RXB1D7   = 0b01111101,
	
	C_REG_CANSTAT  = 0b11111110,
	C_REG_CANCTRL  = 0b11111111,
	
	C_CANINTF_MERRF = (1 << 7),
	C_CANINTF_WAKIF = (1 << 6),
	C_CANINTF_ERRIF = (1 << 5),
	C_CANINTF_TX2IF = (1 << 4),
	C_CANINTF_TX1IF = (1 << 3),
	C_CANINTF_TX0IF = (1 << 2),
	C_CANINTF_RX1IF = (1 << 1),
	C_CANINTF_RX0IF = (1 << 0),
};


void can_init(void);

void can_mode(uint8_t mode);

void can_cmd_reset(void);

uint8_t can_cmd_status(void);
uint8_t can_cmd_rx_status(void);

void can_cmd_rts(uint8_t mask);

uint8_t can_cmd_read(uint8_t addr);
void can_cmd_write(uint8_t addr, uint8_t data);
void can_cmd_modify(uint8_t addr, uint8_t mask, uint8_t data);

void can_cmd_read_many(uint8_t addr, uint8_t len, uint8_t data[static len]);
void can_cmd_write_many(uint8_t addr, uint8_t len,
	const uint8_t data[static len]);

void can_cmd_read_rx_buf(uint8_t start, uint8_t len, uint8_t data[static len]);
void can_cmd_load_tx_buf(uint8_t start, uint8_t len,
	const uint8_t data[static len]);


#endif
