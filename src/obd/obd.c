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
	
	can_init(500);
	
	uint8_t data[8] = {
		2, 1, 0,
	};
	can_tx(0x7df, 8, &data);
	
	for ( ;; );
}
