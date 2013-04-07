/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_ALGO_FIFO_H
#define AUTOMATON_ALGO_FIFO_H


#include "std.h"


/* cannot exceed 256; power of two is preferred (AND instead of MOD) */
#define FIFO_SIZE 16


struct fifo {
	uint8_t data[FIFO_SIZE];
	
	uint8_t len;
	
	uint8_t i_push;
	uint8_t i_pop;
};


static inline uint8_t fifo_count(volatile struct fifo *fifo) {
	return fifo->len;
}

static inline uint8_t fifo_free(volatile struct fifo *fifo) {
	return (FIFO_SIZE - fifo->len);
}


void fifo_init(volatile struct fifo *fifo);

bool fifo_push(volatile struct fifo *fifo, uint8_t val);
bool fifo_push_wait(volatile struct fifo *fifo, uint8_t val,
	uint16_t timeout_ms);
void fifo_push_force(volatile struct fifo *fifo, uint8_t val);

bool fifo_pop(volatile struct fifo *fifo, uint8_t *out);
bool fifo_pop_wait(volatile struct fifo *fifo, uint8_t *out,
	uint16_t timeout_ms);


#endif
