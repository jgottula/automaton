/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_CAN_CAN_H
#define AUTOMATON_DEV_CAN_CAN_H


#include "std.h"


struct can_msg {
	uint16_t id;
	
	uint8_t dlc;
	uint8_t data[8];
};

struct can_msg_eid {
	uint16_t sid;
	uint32_t eid;
	
	uint8_t dlc;
	uint8_t data[8];
};


void can_init(uint16_t kbps);

void can_bitrate(uint16_t kbps);

void can_tx(const struct can_msg *msg);


#endif
