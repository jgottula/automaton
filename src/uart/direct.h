/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_UART_DIRECT_H
#define AUTOMATON_UART_DIRECT_H


#include "std.h"
#include "uart/uart.h"


void uart_direct_write(uint8_t dev, uint16_t divisor, const char *str);
void uart_direct_write_P(uint8_t dev, uint16_t divisor, const char *pstr);


#endif
