/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_MCU_DEFER_H
#define AUTOMATON_MCU_DEFER_H


#include "std.h"


enum {
	DEFER_GPS,
};


void defer(uint8_t mask);


#endif
