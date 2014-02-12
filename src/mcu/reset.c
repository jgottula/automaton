/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "mcu/reset.h"


noreturn void reset(void) {
	cli();
	
	/* invoke a software reset */
	CCP = CCP_IOREG_gc;
	RST.CTRL |= RST_SWRST_bm;
	
#warning TODO: mcu/reset.c
	//BREAK();
	for ( ; ; );
}
