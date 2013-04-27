/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_OBD_STN1110_H
#define AUTOMATON_OBD_STN1110_H


#include "std.h"


void stn1110_init(void);

bool stn1110_at(const char *cmd, char *reply, size_t reply_max,
	uint16_t timeout);


#endif
