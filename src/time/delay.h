/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_TIME_DELAY_H
#define AUTOMATON_TIME_DELAY_H


#include "std.h"


/* nanoseconds in a cpu cycle */
#define CYCLE_NSEC (1000000000ULL / F_CPU)

/* delay for precisely the number of cycles necessary */
#define DELAY_NSEC(_ns) \
	__builtin_avr_delay_cycles(CEIL((_ns), CYCLE_NSEC) - 1)


#endif
