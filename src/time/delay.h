/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_TIME_DELAY_H
#define AUTOMATON_TIME_DELAY_H


#include "std.h"


#if F_CPU == 20000000UL
#define delay_150ns()  __builtin_avr_delay_cycles(2)
#define delay_1200ns() __builtin_avr_delay_cycles(23)
#elif F_CPU == 10000000UL
#define delay_150ns()  __builtin_avr_delay_cycles(1)
#define delay_1200ns() __builtin_avr_delay_cycles(11)
#else
#error delay cycle counts are wrong
#endif


#endif
