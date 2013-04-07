/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_TIME_DELAY_H
#define AUTOMATON_TIME_DELAY_H


#include "std.h"


_Static_assert(F_CPU == 20000000UL, "delay values are wrong");


#define delay_150ns()  __builtin_avr_delay_cycles(2)
#define delay_1200ns() __builtin_avr_delay_cycles(23)


#endif
