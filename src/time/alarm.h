/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_TIME_ALARM_H
#define AUTOMATON_TIME_ALARM_H


#include "std.h"


struct alarm {
	volatile struct {
		uint8_t ticking : 1; // actively ticking
		uint8_t expired : 1; // was ticking, now expired
	};
	
	volatile uint16_t ticks;
	
	struct alarm *next;
};


void alarm_tick(void);

void alarm_register(struct alarm *alarm);
void alarm_unregister(struct alarm *alarm);

void alarm_start(struct alarm *alarm, uint16_t duration);
void alarm_stop(struct alarm *alarm);

bool alarm_ticking(struct alarm *alarm);
bool alarm_expired(struct alarm *alarm);


#endif
