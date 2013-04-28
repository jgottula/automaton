/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_CONV_BCD_H
#define AUTOMATON_CONV_BCD_H


#include "std.h"


uint8_t bcd_decode_packed(uint8_t bcd_p);

uint8_t bcd_encode_packed(uint8_t bin);


#endif
