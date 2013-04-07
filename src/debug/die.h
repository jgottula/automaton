/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEBUG_DIE_H
#define AUTOMATON_DEBUG_DIE_H


#include "std.h"


static inline noreturn void die(void) {
	cli();
	for ( ; ; );
}


#endif
