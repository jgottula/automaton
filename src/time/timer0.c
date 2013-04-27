/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "time/timer0.h"
#include "button/button.h"
#include "time/alarm.h"


static volatile uint16_t timer0_counter = 0;



ISR(TIMER0_COMPA_vect) {
	uint16_t counter = timer0_counter++;
	
	alarm_timer0_hook();
	if (counter % 8 == 0) {
		button_timer0_hook();
	}
}


void timer0_start(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TCCR0A = _BV(WGM01);
		TCCR0B = _BV(CS02);
		
		TCNT0 = 0;
		OCR0A = (F_CPU / 256) / TIMER0_USEC;
		
		TIMSK0 = _BV(OCIE0A);
		
		
		timer0_counter = 0;
	}
}

void timer0_stop(void) {
	TIMSK0 = 0;
}


uint32_t timer0_count32(void) {
	uint32_t count;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		count = timer0_counter;
	}
	return count;
}

uint16_t timer0_count16(void) {
	uint16_t count;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		count = (uint16_t)timer0_counter;
	}
	return count;
}

uint8_t timer0_count8(void) {
	/* 8-bit reads are atomic */
	return (uint8_t)timer0_counter;
}
