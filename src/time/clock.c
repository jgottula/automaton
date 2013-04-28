/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "time/clock.h"
#include "rtc/rtc.h"


/* call from timer0 ISR so system time will tick */
void clock_timer0_hook(void) {
	static uint16_t tick_next = 1000;
	
	if (--tick_next == 0) {
		system_tick();
		tick_next = 1000;
	}
}


/* initialize the rtc and load the clock from it */
void clock_init(void) {
	rtc_init();
	
	clock_load();
}


/* load the system clock from the rtc */
void clock_load(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		struct tm tm;
		rtc_get_tm(&tm);
		
		time_t t = mktime(&tm);
		
		set_system_time(t);
	}
}

/* store the system clock to the rtc */
void clock_store(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		time_t t = time(NULL);
		
		struct tm tm;
		gmtime_r(&t, &tm);
		
		rtc_set_tm(&tm);
	}
}
