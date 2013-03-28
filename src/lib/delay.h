/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_LIB_DELAY_H
#define AUTOMATON_LIB_DELAY_H


#include "std.h"
#include "lib.h"


#if F_CPU != 20000000UL
#error delay values are wrong!
#endif

#define delay_150ns()  __builtin_avr_delay_cycles(2)
#define delay_1200ns() __builtin_avr_delay_cycles(23)


#endif
