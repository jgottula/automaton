/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/stdfile.h"
#include "io/uart.h"


static int stdin_get(FILE *f) {
	(void)f;
	
	char c;
	if (uart_read(UART_PC, &c)) {
		return (int)c;
	} else {
		return _FDEV_ERR;
	}
}

static int stdout_put(char c, FILE *f) {
	(void)f;
	
	if (uart_write(UART_PC, c)) {
		return 0;
	} else {
		return _FDEV_ERR;
	}
}


void stdfile_open(void) {
	(void)fdevopen(stdout_put, stdin_get);
}

void stdfile_close(void) {
	fclose(stdin);
}
