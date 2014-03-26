/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "obd/obd.h"
#include "dev/can/can.h"


void obd_test(void) {
	printf_P(PSTR("obd_test()\n"));
	
	uint8_t txb0ctrl = can_cmd_read(C_REG_TXB0CTRL);
	printf_P(PSTR("TXB0CTRL: %02x\n"), txb0ctrl);
	uint8_t canstat = can_cmd_read(C_REG_CANSTAT);
	printf_P(PSTR("CANSTAT: %02x\n"), canstat);
	
	uart_flush();
	
	/* use one-shot mode */
	//can_cmd_modify(C_REG_CANCTRL, 0b00001000, 0b00001000);
	
	/* enter loopback mode */
	//can_mode(0b01000000);
	
	/* set up a frame to transmit */
	can_cmd_write(C_REG_TXB0SIDH, 0b11111011); // ID 0x7df
	can_cmd_write(C_REG_TXB0SIDL, 0b11100000);
	//can_cmd_write(C_REG_TXB0DLC,  0b00000000); // 0 bytes
	
	can_cmd_write(C_REG_TXB0DLC,  0b00001000); // 8 bytes
	
	can_cmd_write(C_REG_TXB0D0,   0b00000010); // 2 data bytes
	can_cmd_write(C_REG_TXB0D1,   0b00000001); // OBD mode: 01
	can_cmd_write(C_REG_TXB0D2,   0b00000000); // OBD PID:  00
	can_cmd_write(C_REG_TXB0D3,   0b00000000); // padding
	can_cmd_write(C_REG_TXB0D4,   0b00000000); // padding
	can_cmd_write(C_REG_TXB0D5,   0b00000000); // padding
	can_cmd_write(C_REG_TXB0D6,   0b00000000); // padding
	can_cmd_write(C_REG_TXB0D7,   0b00000000); // padding
	
	/* request transmission */
	can_cmd_rts(0b00000001);
	
	for ( ;; ) {
		// allow CAN interrupts to fire
		_delay_ms(1);
		
		// manually check for int conditions
		/*uint8_t intf;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			intf = can_cmd_read(C_REG_CANINTF);
		}
		
		if (intf != 0) {
			fprintf_P(lcd, PSTR("intf = 0x%02" PRIx8 "\r\n"), intf);
			ASSERT(0);
		}*/
	}
}
