/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "io/led.h"


void led_set(uint8_t mask) {
	IO_WRITE(DDR(IO_LED), IO_LED_ALL, IO_LED_ALL);
	
	uint8_t value = 0;
	if (mask & _BV(0)) {
		value |= IO_LED_0;
	}
	
	IO_WRITE(PORT(IO_LED), IO_LED_ALL, value);
}
