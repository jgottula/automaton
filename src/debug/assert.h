/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEBUG_ASSERT_H
#define AUTOMATON_DEBUG_ASSERT_H


#include "std.h"


/* on assertion failure: reset immediately */
#define ASSERT_FAIL_RESET 0
/* on assertion failure: break for debugging */
#define ASSERT_FAIL_BREAK 1


#define ASSERT(_expr) \
	if (!(_expr)) { \
		assert_fail(PSTR(__FILE__), __LINE__); \
	}


noreturn void assert_fail(const __flash char *file, uint16_t line);


#endif
