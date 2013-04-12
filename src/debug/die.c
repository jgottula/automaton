/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/die.h"
#include "io/uart.h"


noreturn void die(void) {
	uart_flush(UART_PC, 100);
	
	cli();
	for ( ; ; );
}
