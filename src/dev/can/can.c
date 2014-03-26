/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/can/can.h"
#include "dev/can/spi.h"


ISR(PORTB_INT0_vect) {
#warning REMOVE ME!!
	_delay_ms(100);
	
	uint8_t intf = can_cmd_read(C_REG_CANINTF);
	
	for (uint8_t i = 0; i < 8; ++i) {
		uint8_t flag = (1 << i);
		
		if (intf & flag) {
			if (flag == C_CANINTF_MERRF) {
				fputs_P(PSTR("MERRF\r\n"), lcd);
			} else if (flag == C_CANINTF_WAKIF) {
				fputs_P(PSTR("WAKIF\r\n"), lcd);
			} else if (flag == C_CANINTF_ERRIF) {
				fputs_P(PSTR("ERRIF\r\n"), lcd);
			} else if (flag == C_CANINTF_TX2IF) {
				fputs_P(PSTR("TX2IF\r\n"), lcd);
			} else if (flag == C_CANINTF_TX1IF) {
				fputs_P(PSTR("TX1IF\r\n"), lcd);
			} else if (flag == C_CANINTF_TX0IF) {
				fputs_P(PSTR("TX0IF\r\n"), lcd);
			} else if (flag == C_CANINTF_RX1IF) {
				fputs_P(PSTR("RX1IF\r\n"), lcd);
			} else if (flag == C_CANINTF_RX0IF) {
				fputs_P(PSTR("RX0IF\r\n"), lcd);
			}
			
			/* acknowlegde interrupt */
			can_cmd_modify(C_REG_CANINTF, flag, 0);
		}
	}
	
	uint8_t eflg = can_cmd_read(C_REG_EFLG);
	uint8_t tec = can_cmd_read(C_REG_TEC);
	uint8_t rec = can_cmd_read(C_REG_REC);
	fprintf_P(lcd, PSTR("EFLG: %02x TEC: %02x REC: %02x\r\n"), eflg, tec, rec);
	uint8_t txb0ctrl = can_cmd_read(C_REG_TXB0CTRL);
	fprintf_P(lcd, PSTR("TXB0CTRL: %02x\r\n"), txb0ctrl);
	uint8_t rxb0ctrl = can_cmd_read(C_REG_RXB0CTRL);
	fprintf_P(lcd, PSTR("RXB0CTRL: %02x\r\n"), rxb0ctrl);
	uint8_t rxb0sidh = can_cmd_read(C_REG_RXB0SIDH);
	uint8_t rxb0sidl = can_cmd_read(C_REG_RXB0SIDL);
	fprintf_P(lcd, PSTR("RXB0SID[H:L]: %02x %02x\r\n"), rxb0sidh, rxb0sidl);
	uint8_t rxb0eid8 = can_cmd_read(C_REG_RXB0EID8);
	uint8_t rxb0eid0 = can_cmd_read(C_REG_RXB0EID0);
	fprintf_P(lcd, PSTR("RXB0EID: %02x %02x\r\n"), rxb0eid8, rxb0eid0);
	uint8_t rxb0dlc = can_cmd_read(C_REG_RXB0DLC);
	fprintf_P(lcd, PSTR("RXB0DLC: %02x\r\n"), rxb0dlc);
	uint8_t rxb0d0 = can_cmd_read(C_REG_RXB0D0);
	uint8_t rxb0d1 = can_cmd_read(C_REG_RXB0D1);
	uint8_t rxb0d2 = can_cmd_read(C_REG_RXB0D2);
	uint8_t rxb0d3 = can_cmd_read(C_REG_RXB0D3);
	fprintf_P(lcd, PSTR("RXB0D[0:3]: %02x %02x %02x %02x\r\n"),
		rxb0d0, rxb0d1, rxb0d2, rxb0d3);
	uint8_t rxb0d4 = can_cmd_read(C_REG_RXB0D4);
	uint8_t rxb0d5 = can_cmd_read(C_REG_RXB0D5);
	uint8_t rxb0d6 = can_cmd_read(C_REG_RXB0D6);
	uint8_t rxb0d7 = can_cmd_read(C_REG_RXB0D7);
	fprintf_P(lcd, PSTR("RXB0D[4:7]: %02x %02x %02x %02x\r\n"),
		rxb0d4, rxb0d5, rxb0d6, rxb0d7);
	
#warning REMOVE STOP CONDITION IN CAN INTERRUPT
	lcd_update(false);
	for(;;);
}


static void _DUMP_ALL_REGS(void) {
	uint8_t addr = 0x00;
	do {
		if (addr % 16 == 0) {
			printf("0x%02x: ", addr);
		}
		
		uint8_t byte = can_cmd_read(addr);
		printf(" %02x", byte);
		
		if (addr % 16 == 3 || addr % 16 == 7 || addr % 16 == 11) {
			printf(" ");
		}
		if (addr % 16 == 15) {
			printf("\n");
		}
	} while (++addr != 0x00);
}


static void _can_setup_bit_time(uint8_t sjw, uint8_t brp, uint8_t prop,
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
	can_cmd_write(C_REG_CNF1, sjw | brp);
	/* CNF2: BTLMODE, SAM, PHSEG1, PRSEG */
	can_cmd_write(C_REG_CNF2, 0b11000000 | ps1 | prop);
	/* CNF3: SOF, WAKFIL, PHSEG2 */
	can_cmd_write(C_REG_CNF3, 0b01000000 | ps2);
}


void can_init(void) {
	can_spi_init();
	
	/* drive CAN_STANDBY low */
	PORTD.OUTCLR = 0b00000010;
	PORTD.DIRSET = 0b00000010;
	
#warning change can reset delay to 1 microsecond
	can_cmd_reset();
	_delay_ms(100);
	
	/* enter configuration mode */
	can_mode(0b10000000);
	
	/* receive all messages (no filter/mask) */
	can_cmd_write(C_REG_RXB0CTRL, 0b01100000);
	can_cmd_write(C_REG_RXB1CTRL, 0b01100000);
	
	/* 500 kbps: BRP = 0 */
	_can_setup_bit_time(2, 0, 8, 8, 8);
	
#if 0
	/* SJW = 1T_Q, BRP = 4 (125 kHz) */
	can_cmd_write(C_REG_CNF1, 0b00000100);
	/* SAM = 3x, PS1 = 7T_Q, PSL = 2T_Q */
	can_cmd_write(C_REG_CNF2, 0b11110001);
	/* SOF = clkout, WAKFIL = on, PS2 = 6T_Q */
	can_cmd_write(C_REG_CNF3, 0b01000101);
#endif
	
	/* set up pin-change interrupt for CAN_INT with high priority */
	PORTB.PIN2CTRL |= PORT_ISC_LEVEL_gc;
	PORTB.INTCTRL   = PORT_INT0LVL_HI_gc;
	PORTB.INT0MASK  = 0b00000100;
	
	/* enable all interrupts */
	can_cmd_write(C_REG_CANINTE, 0b11111111);
	
	/* enter normal mode */
	can_mode(0b00000000);
	
	_DUMP_ALL_REGS();
}


void can_mode(uint8_t mode) {
	mode &= 0b11100000;
	
	/* change mode and verify that the change took effect */
	can_cmd_modify(C_REG_CANCTRL, 0b11100000, mode);
	while ((can_cmd_read(C_REG_CANSTAT) & 0b11100000) != mode);
}


void can_cmd_reset(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b11000000);
		can_spi_deselect();
	}
}


uint8_t can_cmd_status(void) {
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

uint8_t can_cmd_rx_status(void) {
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


void can_cmd_rts(uint8_t mask) {
	mask &= 0b00000111;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b10000000 | mask);
		can_spi_deselect();
	}
}


uint8_t can_cmd_read(uint8_t addr) {
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

void can_cmd_write(uint8_t addr, uint8_t data) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b00000010);
		(void)can_spi_xfer(addr);
		(void)can_spi_xfer(data);
		can_spi_deselect();
	}
}

void can_cmd_modify(uint8_t addr, uint8_t mask, uint8_t data) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b00000101);
		(void)can_spi_xfer(addr);
		(void)can_spi_xfer(mask);
		(void)can_spi_xfer(data);
		can_spi_deselect();
	}
}


void can_cmd_read_many(uint8_t addr, uint8_t len, uint8_t data[static len]) {
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

void can_cmd_write_many(uint8_t addr, uint8_t len,
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


void can_cmd_read_rx_buf(uint8_t start, uint8_t len, uint8_t data[static len]) {
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

void can_cmd_load_tx_buf(uint8_t start, uint8_t len,
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
