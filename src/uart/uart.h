/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_UART_UART_H
#define AUTOMATON_UART_UART_H


#include "std.h"


enum uart_num {
	UART_PC      = 0,
	UART_STN1110 = 1,
};

/* bit 12 is the double-speed bit */
enum uart_divisor {
#if F_CPU == 20000000UL
	UART_DIV_600    = 2082,
	UART_DIV_1200   = 1041,
	UART_DIV_2400   = 520,
	UART_DIV_4800   = 259,
	UART_DIV_9600   = 129,
	UART_DIV_14400  = 86,
	UART_DIV_19200  = 64,
	UART_DIV_28800  = 42,
	UART_DIV_38400  = 32,
	UART_DIV_57600  = 21,
	UART_DIV_76800  = 15,
	UART_DIV_115200 = 10,
	UART_DIV_250K   = 4,
	UART_DIV_1_25M  = 0,
	
	UART_DIV_230400 = _BV(12) | 21,
	UART_DIV_500K   = _BV(12) | 4,
	UART_DIV_2_50M  = _BV(12) | 0,
#elif F_CPU == 10000000UL
	UART_DIV_300    = 2082,
	UART_DIV_600    = 1041,
	UART_DIV_1200   = 520,
	UART_DIV_2400   = 259,
	UART_DIV_4800   = 129,
	UART_DIV_9600   = 64,
	UART_DIV_14400  = 42,
	UART_DIV_19200  = 32,
	UART_DIV_28800  = 21,
	UART_DIV_38400  = 15,
	UART_DIV_57600  = 10,
	UART_DIV_76800  = 7,
	
	UART_DIV_115200 = _BV(12) | 10,
	UART_DIV_250K   = _BV(12) | 4,
	UART_DIV_1_25M  = _BV(12) | 0,
#else 
#error UART baud rate divisors are wrong
#endif
};


void uart_init(uint8_t dev, uint16_t divisor, uint16_t timeout_tx,
	uint16_t timeout_rx);
void uart_stop(uint8_t dev);

bool uart_flush(uint8_t dev, uint16_t timeout);

uint8_t uart_avail(uint8_t dev);

bool uart_write(uint8_t dev, char chr);
bool uart_read(uint8_t dev, char *chr);


#endif
