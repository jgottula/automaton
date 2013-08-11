/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_TIME_TIMER1_H
#define AUTOMATON_TIME_TIMER1_H


#include "std.h"


#define TIMER1_USEC 1000
_Static_assert(F_CPU / TIMER1_USEC < UINT16_MAX,
	"timer1 overflow value will cause integer overflow");


void timer1_start(void);
void timer1_stop(void);

uint32_t timer1_count32(void);
uint16_t timer1_count16(void);
uint8_t timer1_count8(void);


#endif
