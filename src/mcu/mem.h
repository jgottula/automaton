/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_MCU_MEM_H
#define AUTOMATON_MCU_MEM_H


#include "std.h"


void *safe_malloc(size_t size);
#define malloc safe_malloc


#endif
