/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "algo/fifo.h"


/* nonatomic: push without any checks whatsoever */
static void _fifo_push(struct fifo *fifo, uint8_t val) {
	fifo->data[fifo->i_push] = val;
	
	++fifo->i_push;
	fifo->i_push %= FIFO_SIZE;
	
	++fifo->len;
}

/* nonatomic: pop without any checks whatsoever */
static uint8_t _fifo_pop(struct fifo *fifo) {
	uint8_t val = fifo->data[fifo->i_pop];
	
	++fifo->i_pop;
	fifo->i_pop %= FIFO_SIZE;
	
	--fifo->len;
	
	return val;
}


/* nonatomic: initialize fifo */
void fifo_init(struct fifo *fifo) {
	fifo->len = 0;
	
	fifo->i_push = 0;
	fifo->i_pop = 0;
	
	alarm_register(&fifo->alarm);
}

/* nonatomic: deinitialize fifo */
void fifo_deinit(struct fifo *fifo) {
	alarm_unregister(&fifo->alarm);
}


/* atomic: try to push a value onto a fifo; returns false if full */
bool fifo_push(struct fifo *fifo, uint8_t val) {
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo_free(fifo) != 0) {
			_fifo_push((struct fifo *)fifo, val);
			result = true;
		}
	}
	
	return result;
}

/* atomic: push a value onto a fifo and wait until successful; returns false on
 * timeout; a timeout of zero will wait forever */
bool fifo_push_wait(struct fifo *fifo, uint8_t val, uint16_t timeout) {
	if (fifo_push(fifo, val)) {
		return true;
	} else {
		if (timeout != 0) {
			alarm_start(&fifo->alarm, timeout);
		}
		
		for ( ; ; ) {
			NONATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				_delay_us(1);
			}
			
			if (fifo_push(fifo, val)) {
				alarm_stop(&fifo->alarm);
				return true;
			}
			
			if (timeout != 0 && alarm_expired(&fifo->alarm)) {
				return false;
			}
		}
	}
}

/* atomic: forcefully push a value onto a fifo; eats oldest values if full */
void fifo_push_force(struct fifo *fifo, uint8_t val) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo_free(fifo) == 0) {
			/* throw away oldest value */
			(void)_fifo_pop((struct fifo *)fifo);
		}
		
		_fifo_push((struct fifo *)fifo, val);
	}
}


/* atomic: try to pop a value off of a fifo; returns false if empty */
bool fifo_pop(struct fifo *fifo, uint8_t *out) {
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo_count(fifo) != 0) {
			*out = _fifo_pop((struct fifo *)fifo);
			result = true;
		}
	}
	
	return result;
}

/* atomic: pop a value off of a fifo and wait until successful; returns false on
 * timeout; a timeout of zero will wait forever */
bool fifo_pop_wait(struct fifo *fifo, uint8_t *out, uint16_t timeout) {
	if (fifo_pop(fifo, out)) {
		return true;
	} else {
		if (timeout != 0) {
			alarm_start(&fifo->alarm, timeout);
		}
		
		for ( ; ; ) {
			NONATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				_delay_us(1);
			}
			
			if (fifo_pop(fifo, out)) {
				alarm_stop(&fifo->alarm);
				return true;
			}
			
			if (timeout != 0 && alarm_expired(&fifo->alarm)) {
				return false;
			}
		}
	}
}
