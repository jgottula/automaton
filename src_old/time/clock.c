/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "time/clock.h"
#include "rtc/rtc.h"


/* call from timer1 ISR so system time will tick */
void clock_timer1_hook(void) {
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
	
#if 0
	/*struct tm tm = {
		.tm_sec  = 0,
		.tm_min  = 30,
		.tm_hour = 20,
		.tm_mday = 27,
		.tm_mon  = 3,
		.tm_year = 113,
		.tm_wday = 6,
	};
	time_t t = mktime(&tm);*/
	set_system_time(/*t*/1367094600-946684800);
	clock_store();
	
	clock_load();
#endif
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
