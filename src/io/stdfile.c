/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "io/stdfile.h"
#include "dev/lcd/font.h"
#include "dev/uart/uart.h"


FILE *lcd = NULL;


/* write to ftdi uart */
static int _put_ftdi(char c, FILE *f) {
	(void)f;
	
	/* convert LF to CRLF */
	if (c == '\n' && !uart_write('\r')) {
		return _FDEV_ERR;
	}
	
	if (uart_write(c)) {
		return 0;
	} else {
		return _FDEV_ERR;
	}
}

/* read from ftdi uart */
static int _get_ftdi(FILE *f) {
	(void)f;
	
	char c;
	if (uart_read(&c)) {
		return (int)c;
	} else {
		return _FDEV_ERR;
	}
}


/* write to lcd */
static int _put_lcd(char c, FILE *f) {
	(void)f;
	
	lcd_draw_chr(c);
	return 0;
}


/* open std{in,out,err} and other files */
void stdfile_open(void) {
	(void)fdevopen(_put_ftdi, _get_ftdi);
	lcd = fdevopen(_put_lcd, NULL);
}

/* close all files */
void stdfile_close(void) {
	fclose(stdin);
	fclose(lcd);
}
