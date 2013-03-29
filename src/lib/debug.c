/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug.h"


noreturn void die(void) {
	cli();
	for ( ; ; );
}

noreturn void reset(void) {
	cli();
	wdt_enable(WDTO_15MS);
	die();
}


void led_set(bool on) {
	io_write(DDR(IO_DEBUG), IO_DEBUG_LED, IO_DEBUG_LED);
	io_write(PORT(IO_DEBUG), IO_DEBUG_LED, (on ? IO_DEBUG_LED : 0));
}
