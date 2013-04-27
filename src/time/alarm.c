/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "time/alarm.h"


static struct alarm *alarms = NULL;


/* call from timer ISR so alarms will tick */
void alarm_tick(void) {
	struct alarm *this = alarms;
	while (this != NULL) {
		if (this->ticking) {
			if (--this->ticks == 0) {
				this->ticking = false;
				this->expired = true;
			}
		}
		
		this = this->next;
	}
}


/* initializes an alarm and adds it to the linked list of registered alarms */
void alarm_register(struct alarm *alarm) {
	alarm->ticking = false;
	alarm->expired = false;
	
	alarm->next = alarms;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		alarms = alarm;
	}
}

/* removes an alarm from the linked list of registered alarms */
void alarm_unregister(struct alarm *alarm) {
	struct alarm *this = alarms, **prev_next = &alarms;
	while (this != NULL) {
		if (this == alarm) {
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				*prev_next = this->next;
			}
			
			alarm->ticking = false;
			alarm->expired = false;
			
			return;
		}
		
		prev_next = &this->next;
		this = this->next;
	}
	
	ASSERT(false);
}


/* starts the alarm ticking with the requested duration (milliseconds) */
void alarm_start(struct alarm *alarm, uint16_t duration) {
	alarm->ticks = duration;
	
	alarm->ticking = true;
	alarm->expired = false;
}

/* stops the alarm from ticking; this reduces the ISR execution time */
void alarm_stop(struct alarm *alarm) {
	alarm->ticking = false;
}


/* is the alarm currently ticking? */
bool alarm_ticking(struct alarm *alarm) {
	return alarm->ticking;
}

/* has the alarm expired? */
bool alarm_expired(struct alarm *alarm) {
	return alarm->expired;
}
