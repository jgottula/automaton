/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/can/can.h"
#include "dev/can/mcp2515.h"


void can_init(uint16_t kbps) {
	mcp2515_init();
	
	can_bitrate(kbps);
}


void can_bitrate(uint16_t kbps) {
	/* enter configuration mode */
	mcp2515_mode(0b10000000);
	
	if (kbps == 500) {
		/* BRP = 0 */
		mcp2515_setup_bit_timings(2, 0, 8, 8, 8);
	} else if (kbps == 250) {
		/* BRP = 1 */
		mcp2515_setup_bit_timings(2, 1, 8, 8, 8);
	} else {
		ASSERT(0);
	}
	
	/* return to normal mode */
	mcp2515_mode(0b00000000);
}


void can_tx(uint16_t id, uint8_t dlc, const uint8_t data[static dlc]) {
	struct {
		uint8_t sidh;
		uint8_t sidl;
		
		uint8_t eid8;
		uint8_t eid0;
		
		uint8_t dlc;
		uint8_t data[8];
	} tx_buf;
	
	tx_buf.sidh = (id >> 3);
	tx_buf.sidl = (id << 5);
	
	tx_buf.eid8 = 0;
	tx_buf.eid0 = 0;
	
	tx_buf.dlc = dlc;
	memcpy(tx_buf.data, data, dlc);
	
	
	/* load tx buffer all at once */
	mcp2515_cmd_load_tx_buf(0b000, 5 + dlc, &tx_buf);
	
	/* request transmission */
	mcp2515_cmd_rts(0b00000001);
}
