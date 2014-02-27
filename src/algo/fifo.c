/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "algo/fifo.h"


/* nonatomic: push without any checks whatsoever */
static void _fifo_push(struct fifo *fifo, uint8_t val) {
	fifo->data[fifo->i_push] = val;
	
	++fifo->i_push;
	if (++fifo->i_push >= fifo->size) {
		fifo->i_push = 0;
	}
	
	++fifo->count;
}

/* nonatomic: pop without any checks whatsoever */
static uint8_t _fifo_pop(struct fifo *fifo) {
	uint8_t val = fifo->data[fifo->i_pop];
	
	++fifo->i_pop;
	if (++fifo->i_pop >= fifo->size) {
		fifo->i_pop = 0;
	}
	
	--fifo->count;
	
	return val;
}


/* nonatomic: initialize fifo */
void fifo_init(struct fifo *fifo, uint8_t size) {
	fifo->i_push = 0;
	fifo->i_pop  = 0;
	
	fifo->size  = size;
	fifo->count = 0;
	
	fifo->data = malloc(size);
}

/* nonatomic: deinitialize fifo */
void fifo_deinit(struct fifo *fifo) {
	free(fifo->data);
}


/* atomic: try to push a value onto a fifo; returns false if full */
bool fifo_push(struct fifo *fifo, uint8_t val) {
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo->count < fifo->size) {
			_fifo_push(fifo, val);
			result = true;
		}
	}
	
	return result;
}

/* atomic: try to pop a value off of a fifo; returns false if empty */
bool fifo_pop(struct fifo *fifo, uint8_t *out) {
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo->count > 0) {
			*out = _fifo_pop(fifo);
			result = true;
		}
	}
	
	return result;
}


/* atomic: try to push two values onto a fifo; returns false if full */
bool fifo_push16(struct fifo *fifo, uint16_t val) {
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo->count + 1 < fifo->size) {
			_fifo_push(fifo, val);
			_fifo_push(fifo, val >> 8);
			result = true;
		}
	}
	
	return result;
}

/* atomic: try to pop two values off of a fifo; returns false if empty */
bool fifo_pop16(struct fifo *fifo, uint16_t *out) {
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (fifo->count > 1) {
			*out = (_fifo_pop(fifo) | ((uint16_t)_fifo_pop(fifo) << 8));
			result = true;
		}
	}
	
	return result;
}


/* atomic: push a value onto a fifo; removes the oldest value if full */
void fifo_push_force(struct fifo *fifo, uint8_t val) {
	if (fifo->count >= fifo->size) {
		/* pop into bit bucket */
		(void)_fifo_pop(fifo);
	}
	
	_fifo_push(fifo, val);
}

/* atomic: push two values onto a fifo; removes the oldest value pair if full */
void fifo_push16_force(struct fifo *fifo, uint16_t val) {
	if (fifo->count + 1 >= fifo->size) {
		/* pop into bit bucket */
		(void)_fifo_pop(fifo);
		(void)_fifo_pop(fifo);
	}
	
	_fifo_push(fifo, val);
	_fifo_push(fifo, val >> 8);
}
