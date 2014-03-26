/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "obd/obd.h"
#include "dev/can/can.h"


void obd_test(void) {
	printf_P(PSTR("obd_test()\n"));
	uart_flush();
	
	/* use one-shot mode */
	//can_cmd_modify(C_REG_CANCTRL, 0b00001000, 0b00001000);
	
	/* enter loopback mode */
	//can_mode(0b01000000);
	
	uint8_t data[8] = {
		2, 1, 0,
	};
	can_tx(0x7df, 8, &data);
	
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
