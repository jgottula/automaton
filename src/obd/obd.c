/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "obd/obd.h"
#include "dev/can/can.h"


static uint8_t pid_bitmask[32];


void obd_test(void) {
	printf_P(PSTR("obd_test()\n"));
	uart_flush();
	
	can_init(500);
	
	struct can_msg msg = {
		.id = 0x7df,
		
		.dlc = 8,
		.data = {
			2, // len
			1, // mode
			0, // pid
		},
	};
	
	can_tx(&msg);
	
	/*_delay_ms(1);
	data[2] = 4;
	can_tx(0x7df, 8, data);*/
	
	// TODO: pid scan: chain 00, 20, 40... (until they end) to assemble bitmask
	
	for ( ;; );
}
