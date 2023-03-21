/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "mcu/defer.h"


uint8_t defer_mask = 0;


// TOOD: timer ISR goes here


void defer(uint8_t mask) {
	// setup a one-shot timer to basically implement low-priority DPCs
}


