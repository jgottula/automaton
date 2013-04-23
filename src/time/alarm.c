/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "time/alarm.h"


static uint16_t alarm_target;
static volatile bool alarm_armed = false;

static volatile bool     alarm_ring;
static volatile uint16_t alarm_count;


static void alarm_internal_unset(void) {
	TCCR1B = 0;
	TIMSK1 = 0;
	
	alarm_count = 0;
	alarm_armed = false;
}


ISR(TIMER1_COMPA_vect) {
	if (++alarm_count == alarm_target) {
		alarm_ring = true;
		
		alarm_internal_unset();
	}
}


/* set the alarm */
void alarm_set(uint16_t delay_ms) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		assert(!alarm_armed);
		
		TCCR1A = 0;
		TCCR1B = _BV(WGM12) | _BV(CS10);
		TCCR1C = 0;
		
		TCNT1 = 0;
		OCR1A = F_CPU / 1000;
		
		TIMSK1 = _BV(OCIE1A);
		
		alarm_count = 0;
		alarm_ring  = false;
		
		alarm_target = delay_ms;
		alarm_armed  = true;
	}
}

/* unset the alarm */
void alarm_unset(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		alarm_internal_unset();
	}
}


/* check if the alarm has expired */
bool alarm_check(void) {
	return alarm_ring;
}

/* wait for the alarm to expire */
void alarm_wait(void) {
	while (!alarm_ring);
}
