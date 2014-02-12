/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_CONV_HEX_H
#define AUTOMATON_CONV_HEX_H


#include "std.h"


void conv_hex_encode(uint8_t *bytes, int8_t count);
int8_t conv_hex_decode(const char *ascii);


#endif
