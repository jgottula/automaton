/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/assert.h"
#include "dev/uart/uart.h"
#include "mcu/die.h"
#include "mcu/reset.h"


noreturn void assert_fail(const __flash char *file, uint16_t line) {
	printf_P(PSTR("\nassertion failed: %S:%" PRIu16 "\n"), file, line);
	uart_flush();
	
#if ASSERT_FAIL_BREAK
	BREAK();
#endif
	
#if ASSERT_FAIL_RESET
	reset();
#else
	die();
#endif
}
