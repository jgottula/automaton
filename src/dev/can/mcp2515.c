/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/can/mcp2515.h"
#include "algo/fifo.h"
#include "dev/can/spi.h"


static struct fifo rx_bufs;


static void _mcp2515_rx(uint8_t rx_buf_idx) {
	struct mcp_rx_buf *rx_buf = malloc(sizeof(*rx_buf));
	
	/* read rx buffer all at once */
	mcp2515_cmd_read_rx_buf(rx_buf_idx << 2, rx_buf);
	
	/* push onto the rx_bufs fifo, and assert that it was successful */
	ASSERT(fifo_push16(&rx_bufs, (uintptr_t)rx_buf));
}


#warning we are getting two interrupts here for some reason, needs fixing
ISR(PORTB_INT0_vect) {
	uint8_t intf = mcp2515_cmd_read(MCP_REG_CANINTF);
	
	for (uint8_t i = 0; i < 8; ++i) {
		uint8_t flag = (1 << i);
		
		if (intf & flag) {
			if (flag == MCP_CANINTF_MERRF) {
				//fputs_P(PSTR("MERRF\n"), lcd);
			} else if (flag == MCP_CANINTF_WAKIF) {
				//fputs_P(PSTR("WAKIF\n"), lcd);
			} else if (flag == MCP_CANINTF_ERRIF) {
				//fputs_P(PSTR("ERRIF\n"), lcd);
			} else if (flag == MCP_CANINTF_TX2IF) {
				//fputs_P(PSTR("TX2IF\n"), lcd);
				/* ignored */
			} else if (flag == MCP_CANINTF_TX1IF) {
				//fputs_P(PSTR("TX1IF\n"), lcd);
				/* ignored */
			} else if (flag == MCP_CANINTF_TX0IF) {
				//fputs_P(PSTR("TX0IF\n"), lcd);
				/* ignored */
			} else if (flag == MCP_CANINTF_RX1IF) {
				//fputs_P(PSTR("RX1IF\n"), lcd);
				_mcp2515_rx(1);
			} else if (flag == MCP_CANINTF_RX0IF) {
				//fputs_P(PSTR("RX0IF\n"), lcd);
				_mcp2515_rx(0);
			}
			
			/* acknowledge interrupt */
			mcp2515_cmd_modify(MCP_REG_CANINTF, flag, 0);
		}
	}
	
	uint8_t eflg = mcp2515_cmd_read(MCP_REG_EFLG);
	uint8_t tec = mcp2515_cmd_read(MCP_REG_TEC);
	uint8_t rec = mcp2515_cmd_read(MCP_REG_REC);
	//fprintf_P(lcd, PSTR("EFLG: %02x TEC: %02x REC: %02x\n"), eflg, tec, rec);
	uint8_t txb0ctrl = mcp2515_cmd_read(MCP_REG_TXB0CTRL);
	//fprintf_P(lcd, PSTR("TXB0CTRL: %02x\n"), txb0ctrl);
	//for(;;);
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
	fifo_init(&rx_bufs, 16);
	
	can_spi_init();
	
	/* drive CAN_STANDBY low */
	PORTD.OUTCLR = 0b00000010;
	PORTD.DIRSET = 0b00000010;
	
	mcp2515_cmd_reset();
	_delay_us(1);
	
	/* enter configuration mode */
	mcp2515_mode(0b10000000);
	
#warning MCP2515: stop using one-shot mode! implement error recovery/clearing!
	/* TEMPORARY: use one-shot mode */
	mcp2515_cmd_modify(MCP_REG_CANCTRL, 0b00001000, 0b00001000);
	
	/* receive all messages (no filter/mask); enable rollover */
	mcp2515_cmd_write(MCP_REG_RXB0CTRL, 0b01100100);
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
	/* enter configuration mode */
	mcp2515_mode(0b10000000);
	
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
	
	/* return to normal mode */
	mcp2515_mode(0b00000000);
}


uint8_t mcp2515_choose_tx_buf(void) {
	/* wait for a tx buffer to be empty and return its index */
	for ( ;; ) {
		uint8_t txb0ctrl = mcp2515_cmd_read(MCP_REG_TXB0CTRL);
		if (!(txb0ctrl & 0b00001000)) {
			return 0;
		}
		
		uint8_t txb1ctrl = mcp2515_cmd_read(MCP_REG_TXB1CTRL);
		if (!(txb1ctrl & 0b00001000)) {
			return 1;
		}
		
		uint8_t txb2ctrl = mcp2515_cmd_read(MCP_REG_TXB2CTRL);
		if (!(txb2ctrl & 0b00001000)) {
			return 2;
		}
	}
}


struct mcp_rx_buf *mcp2515_get_rx_buf(void) {
	uintptr_t ptr = NULL;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		fifo_pop16(&rx_bufs, &ptr);
	}
	
	return (struct mcp_rx_buf *)ptr;
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


void mcp2515_cmd_read_rx_buf(uint8_t start, struct mcp_rx_buf *rx_buf) {
	start &= 0b00000110;
	
	uint8_t *dst = (uint8_t *)rx_buf;
	uint8_t len = 13;
	
	if (start & 0b00000010) {
		dst += 5;
		len -= 5;
	}
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b10010000 | start);
			
		while (len-- != 0) {
			*(dst++) = can_spi_xfer(0x00);
		}
		
		can_spi_deselect();
	}
}

void mcp2515_cmd_load_tx_buf(uint8_t start, const struct mcp_tx_buf *tx_buf) {
	start &= 0b00000111;
	
	const uint8_t *src = (uint8_t *)tx_buf;
	uint8_t len = 13;
	
	if (start & 0b00000001) {
		src += 5;
		len -= 5;
	}
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		can_spi_select();
		(void)can_spi_xfer(0b01000000 | start);
		
		while (len-- != 0) {
			can_spi_xfer(*(src++));
		}
		
		can_spi_deselect();
	}
}

