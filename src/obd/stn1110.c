/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "obd/stn1110.h"
#include "io/uart.h"


FILE *stn1110 = NULL;


static int stn1110_file_get(FILE *f) {
	(void)f;
	
	char c;
	if (uart_read(UART_STN1110, &c)) {
		return (int)c;
	} else {
		return _FDEV_ERR;
	}
}

static int stn1110_file_put(char c, FILE *f) {
	(void)f;
	
	if (uart_write(UART_STN1110, c)) {
		return 0;
	} else {
		return _FDEV_ERR;
	}
}


void stn1110_init(void) {
	uart_init(UART_STN1110, UART_DIV_115200, 0, 0);
	
	stn1110 = fdevopen(stn1110_file_put, stn1110_file_get);
}
