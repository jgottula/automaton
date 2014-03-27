/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_BUTTON_BUTTON_H
#define AUTOMATON_DEV_BUTTON_BUTTON_H


#include "std.h"


enum {
	BTN_COUNT = 8,
	
	/* periods in milliseconds; note that the true DELAY time includes the
	 * DEBOUNCE time, since the time between the initial button press and the
	 * second DOWN event is the sum of the two */
	BTN_TIME_DEBOUNCE = 50,  // time before first DOWN event
	BTN_TIME_DELAY    = 200, // time before second DOWN event
	BTN_TIME_REPEAT   = 100, // time between subsequent DOWN events
};


#if 0
#define BTN_QTY 4


enum button_names {
	BTN_PAGE   = 0,
	BTN_SELECT = 1,
	BTN_UP     = 2,
	BTN_DOWN   = 3,
};


struct button_event {
	uint8_t num : 2; // button number
	bool down   : 1; // true if DOWN event
	bool repeat : 1; // true if not the first DOWN event
};
_Static_assert(BTN_QTY <= 4,
	"not enough bits in the bitfield");
_Static_assert(sizeof(struct button_event) == 1,
	"struct button_event must be one byte to fit in a fifo");


void button_timer1_hook(void);

void button_init(void);

uint8_t button_avail(void);
bool button_pump(struct button_event *out);
#endif


#endif
