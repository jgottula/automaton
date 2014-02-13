/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "mcu/mem.h"


#undef malloc
void *safe_malloc(size_t size) {
	void *buf = malloc(size);
	ASSERT(buf);
	
	return buf;
}
