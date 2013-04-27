/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/reset.h"
#include "debug/die.h"


uint8_t mcusr;


noreturn void reset(void) {
	wdt_enable(WDTO_15MS);
	die();
}


void reset_defuse(void) {
	/* store reason for last reset */
	mcusr = MCUSR;
	MCUSR = 0;
	
	/* prevent reset loop caused by watchdog timer */
	wdt_disable();
}
