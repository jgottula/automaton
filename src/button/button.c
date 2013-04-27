/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "button/button.h"
#include "algo/fifo.h"


/* TODO: make this a bitfield struct so that bits can be tested instead of
 * making full 8-bit comparisons (but 8-bit assignment can still happen) */
enum button_stage {
	BTN_STAGE_UP       = 0,
	BTN_STAGE_DEBOUNCE = _BV(0),
	BTN_STAGE_DELAY    = _BV(1),
	BTN_STAGE_REPEAT   = _BV(2),
};


static struct {
	bool init;
	
	volatile uint8_t stages[BTN_QTY];
	volatile uint8_t counts[BTN_QTY];
	
	struct fifo events;
} buttons = {
	.init = false,
};


/* directly poll the buttons */
static uint8_t button_poll(void) {
	return io_read(PIN(IO_BTN), IO_BTN_ALL);
}


/* call from ISR to check button states */
void button_update(void) {
	if (!buttons.init) {
		return;
	}
	
	uint8_t poll = button_poll();
	uint8_t mask = IO_BTN_FIRST;
	for (uint8_t i = 0; i < BTN_QTY; ++i) {
		// TODO: move these into the if's as much as possible
		uint8_t stage = buttons.stages[i];
		uint8_t count = buttons.counts[i];
		
		/* button bits are zero when pressed */
		if (!(poll & mask)) {
			struct button_event event = {
				.num    = i,
				.down   = true,
				.repeat = true,
			};
			
			/* these branches are carefully ordered */
			bool down = false;
			if (count >= BTN_TIME_REPEAT &&
				stage == BTN_STAGE_REPEAT) {
				down = true;
			} else if (count >= BTN_TIME_DELAY && 
				stage == BTN_STAGE_DELAY) {
				buttons.stages[i] = BTN_STAGE_REPEAT;
				down = true;
			} else if (count >= BTN_TIME_DEBOUNCE &&
				stage == BTN_STAGE_DEBOUNCE) {
				buttons.stages[i] = BTN_STAGE_DELAY;
				event.repeat = false;
				down = true;
			} else if (stage == BTN_STAGE_UP) {
				buttons.stages[i] = BTN_STAGE_DEBOUNCE;
				count = 1;
			} else {
				++count;
			}
			
			if (down) {
				/* type punning is legal for char type (8-bit) */
				fifo_push_force(&buttons.events, *((uint8_t *)&event));
				
				count = 0;
			}
			
			buttons.counts[i] = count;
		} else {
			if (stage != BTN_STAGE_UP) {
				/* don't generate a button up event if we were still in the
				 * debounce stage */
				if (stage != BTN_STAGE_DEBOUNCE) {
					struct button_event event = {
						.num  = i,
						.down = false,
						.repeat = false,
					};
					/* type punning is legal for char type (8-bit) */
					fifo_push_force(&buttons.events, *((uint8_t *)&event));
				}
				
				buttons.stages[i] = BTN_STAGE_UP;
			}
		}
		
		mask <<= 1;
	}
}


/* initialize the button system (timer0 must be enabled) */
void button_init(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		for (uint8_t i = 0; i < BTN_QTY; ++i) {
			buttons.stages[i] = BTN_STAGE_UP;
			buttons.counts[i] = 0;
		}
		
		/* set all as inputs with pull-up resistors */
		io_write(PORT(IO_BTN), IO_BTN_ALL, IO_BTN_ALL);
		io_write(DDR(IO_BTN), IO_BTN_ALL, 0);
		
		fifo_init(&buttons.events);
		
		buttons.init = true;
	}
}


/* get the number of button events in the fifo */
uint8_t button_avail(void) {
	if (!buttons.init) {
		return 0;
	}
	
	return fifo_count(&buttons.events);
}

/* get the most recent button event */
bool button_event(struct button_event *out) {
	if (!buttons.init || fifo_count(&buttons.events) == 0) {
		return false;
	}
	
	/* type punning is legal for char type (8-bit) */
	return fifo_pop(&buttons.events, (uint8_t *)out);
}


// TODO: test button code using uart and slowed-down timer0
