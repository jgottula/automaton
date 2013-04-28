/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_TIME_CLOCK_H
#define AUTOMATON_TIME_CLOCK_H


#include "std.h"


void clock_timer0_hook(void);

void clock_init(void);

void clock_load(void);
void clock_store(void);


#endif
