/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "algo/fifo.h"
#include "time/alarm.h"


/* nonatomic: push without any checks whatsoever */
static void fifo_internal_push(struct fifo *fifo, uint8_t val) {
        fifo->data[fifo->i_push] = val;
        
        ++fifo->i_push;
        fifo->i_push %= FIFO_SIZE;
        
        ++fifo->len;
}

/* nonatomic: pop without any checks whatsoever */
static uint8_t fifo_internal_pop(struct fifo *fifo) {
        uint8_t val = fifo->data[fifo->i_pop];
        
        ++fifo->i_pop;
        fifo->i_pop %= FIFO_SIZE;
        
        --fifo->len;
        
        return val;
}


/* nonatomic: initialize fifo values */
void fifo_init(volatile struct fifo *fifo) {
	fifo->len = 0;
	
	fifo->i_push = 0;
	fifo->i_pop = 0;
}


/* atomic: try to push a value onto a fifo; returns false if full */
bool fifo_push(volatile struct fifo *fifo, uint8_t val) {
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo_free(fifo) != 0) {
			fifo_internal_push((struct fifo *)fifo, val);
			result = true;
		}
	}
	
	return result;
}

/* atomic: push a value onto a fifo and wait until successful; returns false on
 * timeout; a timeout of zero will wait forever */
bool fifo_push_wait(volatile struct fifo *fifo, uint8_t val,
	uint16_t timeout_ms) {
	if (fifo_push(fifo, val)) {
		return true;
	} else {
		if (timeout_ms != 0) {
			alarm_set(timeout_ms);
		}
		
		for ( ; ; ) {
			_delay_us(1);
			
			if (fifo_push(fifo, val)) {
				alarm_unset();
				return true;
			}
			
			if (timeout_ms != 0 && alarm_check()) {
				return false;
			}
		}
	}
}

/* atomic: forcefully push a value onto a fifo; eats oldest values if full */
void fifo_push_force(volatile struct fifo *fifo, uint8_t val) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo_free(fifo) == 0) {
			/* throw away oldest value */
			(void)fifo_internal_pop((struct fifo *)fifo);
		}
		
		fifo_internal_push((struct fifo *)fifo, val);
	}
}


/* atomic: try to pop a value off of a fifo; returns false if empty */
bool fifo_pop(volatile struct fifo *fifo, uint8_t *out) {
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo_count(fifo) != 0) {
			*out = fifo_internal_pop((struct fifo *)fifo);
			result = true;
		}
	}
	
	return result;
}

/* atomic: pop a value off of a fifo and wait until successful; returns false on
 * timeout; a timeout of zero will wait forever */
bool fifo_pop_wait(volatile struct fifo *fifo, uint8_t *out,
	uint16_t timeout_ms) {
	if (fifo_pop(fifo, out)) {
		return true;
	} else {
		if (timeout_ms != 0) {
			alarm_set(timeout_ms);
		}
		
		for ( ; ; ) {
			_delay_us(1);
			
			if (fifo_pop(fifo, out)) {
				alarm_unset();
				return true;
			}
			
			if (timeout_ms != 0 && alarm_check()) {
				return false;
			}
		}
	}
}
