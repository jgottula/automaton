/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/stdfile.h"
#include "lcd/lcd.h"
#include "uart/uart.h"


FILE *lcd = NULL;
FILE *stn1110 = NULL;


/* write to pc uart */
static int _put_pc(char c, FILE *f) {
	(void)f;
	
	/* convert LF to CRLF */
	if (c == '\n' && !uart_write(UART_PC, '\r')) {
		return _FDEV_ERR;
	}
	
	if (uart_write(UART_PC, c)) {
		return 0;
	} else {
		return _FDEV_ERR;
	}
}

/* read from pc uart */
static int _get_pc(FILE *f) {
	(void)f;
	
	char c;
	if (uart_read(UART_PC, &c)) {
		return (int)c;
	} else {
		return _FDEV_ERR;
	}
}


/* write to lcd ddram */
static int _put_lcd(char c, FILE *f) {
	(void)f;
	
	lcd_write(c);
	return 0;
}

/* read from lcd ddram */
static int _get_lcd(FILE *f) {
	(void)f;
	
	return lcd_read();
}


/* write to stn1110 uart */
static int _put_stn1110(char c, FILE *f) {
	(void)f;
	
	if (c == '\n') {
		c = '\r';
	}
	
	if (uart_write(UART_STN1110, c)) {
		return 0;
	} else {
		return _FDEV_ERR;
	}
}

/* read from stn1110 uart */
static int _get_stn1110(FILE *f) {
	(void)f;
	
	char c;
	if (uart_read(UART_STN1110, &c)) {
		if (c == '\r') {
			c = '\n';
		}
		
		return (int)c;
	} else {
		return _FDEV_ERR;
	}
}


/* open std{in,out,err} and other files */
void stdfile_open(void) {
	(void)fdevopen(_put_pc, _get_pc);
	
	lcd = fdevopen(_put_lcd, _get_lcd);
	stn1110 = fdevopen(_put_stn1110, _get_stn1110);
}

/* close all files */
void stdfile_close(void) {
	fclose(stn1110);
	fclose(lcd);
	
	fclose(stdin);
}
