/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/assert.h"
#include "debug/die.h"
#include "debug/reset.h"


noreturn void assert_fail(const __flash char *file, uint16_t line) {
	printf_P(PSTR("assertion failed: %S:%" PRIu16 "\n"), file, line);
	
#if ASSERT_FAIL_RESET
	reset();
#else
	die();
#endif
}
