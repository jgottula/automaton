/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEBUG_BREAK_H
#define AUTOMATON_DEBUG_BREAK_H


#include "std.h"


#define _BREAK() \
	__asm__ __volatile__( \
		"\nbreak" \
		: \
		: \
		: "memory" \
	)


#endif
