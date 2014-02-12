/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "mcu/die.h"
//#include "uart/uart.h"


noreturn void die(void) {
#warning TODO: mcu/die.c
	//uart_flush(UART_PC, 1000);
	
	cli();
	//BREAK();
	for ( ; ; );
}
