/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_TIME_ALARM_H
#define AUTOMATON_TIME_ALARM_H


#include "std.h"


void alarm_set(uint16_t delay_ms);
void alarm_unset(void);

bool alarm_check(void);
void alarm_wait(void);


#endif
