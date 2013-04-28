/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "conv/bcd.h"


uint8_t bcd_decode_packed(uint8_t bcd_p) {
	uint8_t tens = (bcd_p >> 4);
	uint8_t ones = (bcd_p & 0x0f);
	
	return (tens * 10) + ones;
}


uint8_t bcd_encode_packed(uint8_t bin) {
	uint8_t tens = (bin / 10);
	uint8_t ones = (bin % 10);
	
	return (tens << 4) | ones;
}
