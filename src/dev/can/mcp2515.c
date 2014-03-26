/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/can/mcp2515.h"
#include "dev/can/spi.h"


ISR(PORTB_INT0_vect) {
	uint8_t intf = mcp2515_cmd_read(MCP_REG_CANINTF);
	
	for (uint8_t i = 0; i < 8; ++i) {
		uint8_t flag = (1 << i);
		
		if (intf & flag) {
			if (flag == MCP_CANINTF_MERRF) {
				fputs_P(PSTR("MERRF\r\n"), lcd);
			} else if (flag == MCP_CANINTF_WAKIF) {
				fputs_P(PSTR("WAKIF\r\n"), lcd);
			} else if (flag == MCP_CANINTF_ERRIF) {
				fputs_P(PSTR("ERRIF\r\n"), lcd);
			} else if (flag == MCP_CANINTF_TX2IF) {
				fputs_P(PSTR("TX2IF\r\n"), lcd);
			} else if (flag == MCP_CANINTF_TX1IF) {
				fputs_P(PSTR("TX1IF\r\n"), lcd);
			} else if (flag == MCP_CANINTF_TX0IF) {
				fputs_P(PSTR("TX0IF\r\n"), lcd);
			} else if (flag == MCP_CANINTF_RX1IF) {
				fputs_P(PSTR("RX1IF\r\n"), lcd);
			} else if (flag == MCP_CANINTF_RX0IF) {
				fputs_P(PSTR("RX0IF\r\n"), lcd);
			}
			
			/* acknowlegde interrupt */
			mcp2515_cmd_modify(MCP_REG_CANINTF, flag, 0);
		}
	}
	
	uint8_t eflg = mcp2515_cmd_read(MCP_REG_EFLG);
	uint8_t tec = mcp2515_cmd_read(MCP_REG_TEC);
	uint8_t rec = mcp2515_cmd_read(MCP_REG_REC);
	fprintf_P(lcd, PSTR("EFLG: %02x TEC: %02x REC: %02x\r\n"), eflg, tec, rec);
	uint8_t txb0ctrl = mcp2515_cmd_read(MCP_REG_TXB0CTRL);
	fprintf_P(lcd, PSTR("TXB0CTRL: %02x\r\n"), txb0ctrl);
	uint8_t rxb0ctrl = mcp2515_cmd_read(MCP_REG_RXB0CTRL);
	fprintf_P(lcd, PSTR("RXB0CTRL: %02x\r\n"), rxb0ctrl);
	uint8_t rxb0sidh = mcp2515_cmd_read(MCP_REG_RXB0SIDH);
	uint8_t rxb0sidl = mcp2515_cmd_read(MCP_REG_RXB0SIDL);
	fprintf_P(lcd, PSTR("RXB0SID[H:L]: %02x %02x\r\n"), rxb0sidh, rxb0sidl);
	uint8_t rxb0eid8 = mcp2515_cmd_read(MCP_REG_RXB0EID8);
	uint8_t rxb0eid0 = mcp2515_cmd_read(MCP_REG_RXB0EID0);
	fprintf_P(lcd, PSTR("RXB0EID: %02x %02x\r\n"), rxb0eid8, rxb0eid0);
	uint8_t rxb0dlc = mcp2515_cmd_read(MCP_REG_RXB0DLC);
	fprintf_P(lcd, PSTR("RXB0DLC: %02x\r\n"), rxb0dlc);
	uint8_t rxb0d0 = mcp2515_cmd_read(MCP_REG_RXB0D0);
	uint8_t rxb0d1 = mcp2515_cmd_read(MCP_REG_RXB0D1);
	uint8_t rxb0d2 = mcp2515_cmd_read(MCP_REG_RXB0D2);
	uint8_t rxb0d3 = mcp2515_cmd_read(MCP_REG_RXB0D3);
	fprintf_P(lcd, PSTR("RXB0D[0:3]: %02x %02x %02x %02x\r\n"),
		rxb0d0, rxb0d1, rxb0d2, rxb0d3);
	uint8_t rxb0d4 = mcp2515_cmd_read(MCP_REG_RXB0D4);
	uint8_t rxb0d5 = mcp2515_cmd_read(MCP_REG_RXB0D5);
	uint8_t rxb0d6 = mcp2515_cmd_read(MCP_REG_RXB0D6);
	uint8_t rxb0d7 = mcp2515_cmd_read(MCP_REG_RXB0D7);
	fprintf_P(lcd, PSTR("RXB0D[4:7]: %02x %02x %02x %02x\r\n"),
		rxb0d4, rxb0d5, rxb0d6, rxb0d7);
	
#warning REMOVE STOP CONDITION IN CAN INTERRUPT
	lcd_update(false);
	for(;;);
}


static void _mcp2515_dump_regs(void) {
	uint8_t addr = 0x00;
	do {
		if (addr % 16 == 0) {
			printf("0x%02x: ", addr);
		}
		
		uint8_t byte = mcp2515_cmd_read(addr);
		printf(" %02x", byte);
		
		if (addr % 16 == 3 || addr % 16 == 7 || addr % 16 == 11) {
			printf(" ");
		}
		if (addr % 16 == 15) {
			printf("\n");
		}
	} while (++addr != 0x00);
}


void mcp2515_init(void) {
	can_spi_init();
	
	/* drive CAN_STANDBY low */
	PORTD.OUTCLR = 0b00000010;
	PORTD.DIRSET = 0b00000010;
	
	mcp2515_cmd_reset();
	_delay_us(1);
	
	/* enter configuration mode */
	mcp2515_mode(0b10000000);
	
	/* receive all messages (no filter/mask) */
	mcp2515_cmd_write(MCP_REG_RXB0CTRL, 0b01100000);
	mcp2515_cmd_write(MCP_REG_RXB1CTRL, 0b01100000);
	
	/* set up pin-change interrupt for CAN_INT with high priority */
	PORTB.PIN2CTRL |= PORT_ISC_LEVEL_gc;
	PORTB.INTCTRL   = PORT_INT0LVL_HI_gc;
	PORTB.INT0MASK  = 0b00000100;
	
	/* interrupts: MERRE, WAKIE, ERRIE, RX1IE, RX0IE */
	mcp2515_cmd_write(MCP_REG_CANINTE, 0b11100011);
	
	/* enter normal mode */
	mcp2515_mode(0b00000000);
}


void mcp2515_setup_bit_timings(uint8_t sjw, uint8_t brp, uint8_t prop,
	uint8_t ps1, uint8_t ps2) {
	sjw -= 1;
	sjw &= 0b11;
	sjw <<= 6;
	
	brp &= 0b111111;
	
	prop -= 1;
	prop &= 0b111;
	
	ps1 -= 1;
	ps1 &= 0b111;
	ps1 <<= 3;
	
	ps2 -= 1;
	ps2 &= 0b111;
	
	/* CNF1: SJW, BRP */
	mcp2515_cmd_write(MCP_REG_CNF1, sjw | brp);
	/* CNF2: BTLMODE, SAM, PHSEG1, PRSEG */
	mcp2515_cmd_write(MCP_REG_CNF2, 0b11000000 | ps1 | prop);
	/* CNF3: SOF, WAKFIL, PHSEG2 */
	mcp2515_cmd_write(MCP_REG_CNF3, 0b01000000 | ps2);
}


void mcp2515_mode(uint8_t mode) {
	mode &= 0b11100000;
	
	/* change mode and verify that the change took effect */
	mcp2515_cmd_modify(MCP_REG_CANCTRL, 0b11100000, mode);
	while ((mcp2515_cmd_read(MCP_REG_CANSTAT) & 0b11100000) != mode);
}


void mcp2515_cmd_reset(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b11000000);
		can_spi_deselect();
	}
}


uint8_t mcp2515_cmd_status(void) {
	uint8_t status;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b10100000);
		(void)can_spi_xfer(0x00);
		status = can_spi_xfer(0x00);
		can_spi_deselect();
	}
	
	return status;
}

uint8_t mcp2515_cmd_rx_status(void) {
	uint8_t status;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b10110000);
		(void)can_spi_xfer(0x00);
		status = can_spi_xfer(0x00);
		can_spi_deselect();
	}
	
	return status;
}


void mcp2515_cmd_rts(uint8_t mask) {
	mask &= 0b00000111;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b10000000 | mask);
		can_spi_deselect();
	}
}


uint8_t mcp2515_cmd_read(uint8_t addr) {
	uint8_t data;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b00000011);
		(void)can_spi_xfer(addr);
		data = can_spi_xfer(0x00);
		can_spi_deselect();
	}
	
	return data;
}

void mcp2515_cmd_write(uint8_t addr, uint8_t data) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b00000010);
		(void)can_spi_xfer(addr);
		(void)can_spi_xfer(data);
		can_spi_deselect();
	}
}

void mcp2515_cmd_modify(uint8_t addr, uint8_t mask, uint8_t data) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b00000101);
		(void)can_spi_xfer(addr);
		(void)can_spi_xfer(mask);
		(void)can_spi_xfer(data);
		can_spi_deselect();
	}
}


void mcp2515_cmd_read_many(uint8_t addr, uint8_t len,
	uint8_t data[static len]) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b00000011);
		(void)can_spi_xfer(addr);
		
		uint8_t *byte = data;
		while (len-- != 0) {
			*(byte++) = can_spi_xfer(0x00);
		}
		
		can_spi_deselect();
	}
}

void mcp2515_cmd_write_many(uint8_t addr, uint8_t len,
	const uint8_t data[static len]) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b00000010);
		(void)can_spi_xfer(addr);
		
		const uint8_t *byte = data;
		while (len-- != 0) {
			can_spi_xfer(*(byte++));
		}
		
		can_spi_deselect();
	}
}


void mcp2515_cmd_read_rx_buf(uint8_t start, uint8_t len,
	uint8_t data[static len]) {
	start &= 0b00000110;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b10010000 | start);
		
		uint8_t *byte = data;
		while (len-- != 0) {
			*(byte++) = can_spi_xfer(0x00);
		}
		
		can_spi_deselect();
	}
}

void mcp2515_cmd_load_tx_buf(uint8_t start, uint8_t len,
	const uint8_t data[static len]) {
	start &= 0b00000111;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b01000000 | start);
		
		const uint8_t *byte = data;
		while (len-- != 0) {
			can_spi_xfer(*(byte++));
		}
		
		can_spi_deselect();
	}
}

