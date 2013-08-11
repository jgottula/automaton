/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_BUTTON_BUTTON_H
#define AUTOMATON_BUTTON_BUTTON_H


#include "std.h"


#define BTN_QTY 4


/* periods in milliseconds, divided by eight (must not exceed 255);
 * note that the true DELAY time includes BTN_TIME_DEBOUNCE, since the time
 * between button press and the second DOWN event is the sum of the two */
enum button_timing {
	BTN_TIME_DEBOUNCE = ROUND(50,  8), // time before first DOWN event
	BTN_TIME_DELAY    = ROUND(200, 8), // time before second DOWN event
	BTN_TIME_REPEAT   = ROUND(100, 8), // time between subsequent DOWN events
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
