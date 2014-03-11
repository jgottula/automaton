/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/can/can.h"
#include "dev/can/spi.h"


ISR(PORTB_INT0_vect) {
	
	
	// query MCP2515 for what the interrupt was for
	// on message received, read these regs:
	// - RXB0SIDH RXB0SIDL RXB0EID8 RXB0EID0 RXB0DLC RXB0D[x]
	// on error, ...
}


static void _can_mode(uint8_t mode) {
	// CANCTRL = mode & 0b11100000
	
	
	// verify that mode has changed, spin while waiting
}


void can_init(void) {
	can_spi_init();
	
	/* set up pin-change interrupt for CAN_INT with high priority */
	PORTB.PIN2CTRL |= PORT_ISC_LEVEL_gc;
	PORTB.INTCTRL   = PORT_INT0LVL_HI_gc;
	PORTB.INT0MASK  = 0b00000100;
	
	/* receive all messages (no filter/mask) */
	// RXB0CTRL = 0b01100000
	// RXB1CTRL = 0b01100000
	
	
	
	/* SJW = 1T_Q, BRP = 4 (125 kHz) */
	// CNF1 = 0b00000100
	/* SAM = 3x, PS1 = 7T_Q, PSL = 2T_Q */
	// CNF2 = 0b11110001
	/* SOF = clkout, WAKFIL = off, PS2 = 6T_Q */
	// CNF3 = 0b00000101
	
	// look up for CNF3: what is WAKFIL, wake-up filter bit?
}


void can_cmd_reset(void) {
	can_spi_select();
	(void)can_spi_xfer(0b11000000);
	can_spi_deselect();
}


uint8_t can_cmd_status(void) {
	can_spi_select();
	(void)can_spi_xfer(0b10100000);
	(void)can_spi_xfer(0x00);
	uint8_t status = can_spi_xfer(0x00);
	can_spi_deselect();
	
	return status;
}

uint8_t can_cmd_rx_status(void) {
	can_spi_select();
	(void)can_spi_xfer(0b10110000);
	(void)can_spi_xfer(0x00);
	uint8_t status = can_spi_xfer(0x00);
	can_spi_deselect();
	
	return status;
}


void can_cmd_rts(uint8_t mask) {
 	can_spi_select();
 	(void)can_spi_xfer(0b10000000 | (mask & 0b00000111));
 	can_spi_deselect();
}


uint8_t can_cmd_read(uint8_t addr) {
	can_spi_select();
	(void)can_spi_xfer(0b00000011);
	(void)can_spi_xfer(addr);
	uint8_t data = can_spi_xfer(0x00);
	can_spi_deselect();
	
	return data;
}

void can_cmd_write(uint8_t addr, uint8_t data) {
	can_spi_select();
	(void)can_spi_xfer(0b00000010);
	(void)can_spi_xfer(addr);
	(void)can_spi_xfer(data);
	can_spi_deselect();
}

void can_cmd_bit_modify(uint8_t addr, uint8_t mask, uint8_t data) {
	can_spi_select();
	(void)can_spi_xfer(0b00000101);
	(void)can_spi_xfer(addr);
	(void)can_spi_xfer(mask);
	(void)can_spi_xfer(data);
	can_spi_deselect();
}


void can_cmd_read_many(uint8_t addr, uint8_t len, uint8_t data[static len]) {
	can_spi_select();
	(void)can_spi_xfer(0b00000011);
	(void)can_spi_xfer(addr);
	
	uint8_t *byte = data;
	while (len-- != 0) {
		*(byte++) = can_spi_xfer(0x00);
	}
	
	can_spi_deselect();
}

void can_cmd_write_many(uint8_t addr, uint8_t len,
	const uint8_t data[static len]) {
	can_spi_select();
	(void)can_spi_xfer(0b00000010);
	(void)can_spi_xfer(addr);
	
	uint8_t *byte = data;
	while (len-- != 0) {
		can_spi_xfer(*(byte++));
	}
	
	can_spi_deselect();
}


void can_cmd_read_rx_buf(uint8_t start, uint8_t len, uint8_t data[static len]) {
	can_spi_select();
	(void)can_spi_xfer(0b10010000 | (start & 0b00000110));
	
	uint8_t *byte = data;
	while (len-- != 0) {
		*(byte++) = can_spi_xfer(0x00);
	}
	
	can_spi_deselect();
}

void can_cmd_load_tx_buf(uint8_t start, uint8_t len,
	const uint8_t data[static len]) {
	can_spi_select();
	(void)can_spi_xfer(0b01000000 | (start & 0b00000111));
	
	uint8_t *byte = data;
	while (len-- != 0) {
		can_spi_xfer(*(byte++));
	}
	
	can_spi_deselect();
}
