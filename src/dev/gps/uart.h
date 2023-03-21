/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_GPS_UART_H
#define AUTOMATON_DEV_GPS_UART_H


#include "std.h"


void gps_uart_init(void);

bool gps_uart_read(char *chr);
bool gps_uart_write(char chr);


#endif
