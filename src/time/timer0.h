/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_TIME_TIMER0_H
#define AUTOMATON_TIME_TIMER0_H


#include "std.h"


#define TIMER0_USEC 1000


void timer0_start(void);
void timer0_stop(void);

uint32_t timer0_count32(void);
uint16_t timer0_count16(void);
uint8_t timer0_count8(void);


#endif
