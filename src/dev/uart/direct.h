/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_UART_DIRECT_H
#define AUTOMATON_DEV_UART_DIRECT_H


#include "std.h"


void uart_direct_write_chr(uint8_t dev, uint16_t divisor, char chr);
void uart_direct_write_str(uint8_t dev, uint16_t divisor, const char *str);
void uart_direct_write_pstr(uint8_t dev, uint16_t divisor, const char *pstr);


#endif
