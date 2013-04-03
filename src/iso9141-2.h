/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_ISO9141_2_H
#define AUTOMATON_ISO9141_2_H


#include "std.h"
#include "lib.h"


void iso_init(void);

void iso_write(uint8_t byte);

bool iso_read(uint8_t *byte);


#endif
