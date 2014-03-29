/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEBUG_FATAL_H
#define AUTOMATON_DEBUG_FATAL_H


#include "std.h"


/* on fatal error: reset immediately */
#define FATAL_RESET 0
/* on fatal error: break for debugging */
#define FATAL_BREAK 1


noreturn void fatal(const char *pstr);
noreturn void fatal_S(const char *str);


#endif
