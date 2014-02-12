/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_IO_LED_H
#define AUTOMATON_IO_LED_H


#include "std.h"


#define LED_BLOCK \
	for (bool __led_done = (led_set(1), false); \
		(__led_done ? (led_set(0), false) : true); \
		__led_done = true)


void led_set(uint8_t mask);


#endif
