/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/reset.h"
#include "debug/die.h"


noreturn void reset(void) {
	wdt_enable(WDTO_15MS);
	die();
}
