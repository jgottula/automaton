/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "time/timer1.h"
#include "button/button.h"
#include "time/alarm.h"
#include "time/clock.h"


static volatile uint16_t timer1_counter = 0;


ISR(TIMER1_COMPA_vect) {
	/* we need only the low 8 bits of the counter */
	uint8_t c = ++timer1_counter;
	
	clock_timer1_hook();
	alarm_timer1_hook();
	if (c % 8 == 1) {
		button_timer1_hook();
	}
}


void timer1_start(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TCCR1A = 0;
		TCCR1B = _BV(WGM12) | _BV(CS10);
		TCCR1C = 0;
		
		TCNT1 = 0;
		OCR1A = (F_CPU / TIMER1_USEC);
		
		TIMSK1 = _BV(OCIE1A);
		
		
		timer1_counter = 0;
	}
}

void timer1_stop(void) {
	TIMSK1 = 0;
}


uint32_t timer1_count32(void) {
	uint32_t count;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		count = timer1_counter;
	}
	return count;
}

uint16_t timer1_count16(void) {
	uint16_t count;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		count = (uint16_t)timer1_counter;
	}
	return count;
}

uint8_t timer1_count8(void) {
	/* 8-bit reads are atomic */
	return (uint8_t)timer1_counter;
}
