/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_UART_H
#define AUTOMATON_UART_H


#include "std.h"
#include "lib.h"


#define uart_write_pstr(_str) \
	uart_write_pstr_func(PSTR(_str))


void uart_init(void);

void uart_write_chr(char chr);
void uart_write_str(const char *str);
void uart_write_pstr_func(const __flash char *str);

void uart_write_hex8(uint8_t hex);
void uart_write_hex16(uint16_t hex);

bool uart_read_chr(char *chr);


#endif
