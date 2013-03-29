/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_LIB_DEBUG_H
#define AUTOMATON_LIB_DEBUG_H


#include "std.h"
#include "lib.h"


noreturn void die(void);
noreturn void reset(void);

void led_set(bool on);


#endif
