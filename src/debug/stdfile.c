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
	if (uart_read_chr(UART_DEBUG, &c) != 0) {
		return (int)c;
	} else {
		return _FDEV_ERR;
	}
}

static int stdout_put(char c, FILE *f) {
	(void)f;
	
	if (uart_write_chr(UART_DEBUG, c) != 0) {
		return 0;
	} else {
		return _FDEV_ERR;
	}
}


void stdfile_open(void) {
	(void)fdevopen(NULL, stdin_get);
	(void)fdevopen(stdout_put, NULL);
}

void stdfile_close(void) {
	fclose(stdout);
	fclose(stdin);
}
