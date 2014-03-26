/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_CAN_CAN_H
#define AUTOMATON_DEV_CAN_CAN_H


#include "std.h"


void can_init(void);

void can_tx(uint16_t id, uint8_t dlc, const uint8_t data[static dlc]);


#endif
