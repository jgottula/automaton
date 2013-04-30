/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_ALGO_CRC7_H
#define AUTOMATON_ALGO_CRC7_H


#include "std.h"


uint8_t crc7_byte(uint8_t crc, uint8_t datum);
uint8_t crc7_arr(uint8_t crc, uint8_t len, const uint8_t data[static len]);


#endif
