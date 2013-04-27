/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/assert.h"
#include "debug/die.h"
#include "debug/reset.h"
#include "uart/direct.h"


noreturn void assert_fail(const __flash char *file, uint16_t line) {
	uart_flush(UART_PC, 1000);
	
	char buf[64];
	snprintf_P(buf, sizeof(buf), PSTR("\nassertion failed: %S:%" PRIu16 "\n"),
		file, line);
	uart_direct_write_str(UART_PC, UART_DIV_115200, buf);
	
#if ASSERT_FAIL_RESET
	reset();
#else
	die();
#endif
}
