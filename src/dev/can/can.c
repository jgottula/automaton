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
	uint8_t txb0sidh = (id >> 3);
	mcp2515_cmd_write(MCP_REG_TXB0SIDH, txb0sidh);
	uint8_t txb0sidl = (id << 5);
	mcp2515_cmd_write(MCP_REG_TXB0SIDL, txb0sidl);
	
	uint8_t txb0dlc = (dlc & 0b1111);
	mcp2515_cmd_write(MCP_REG_TXB0DLC, txb0dlc);
	
	for (uint8_t i = 0; i < dlc; ++i) {
		mcp2515_cmd_write(MCP_REG_TXB0D0 + i, data[i]);
	}
	
	/* request transmission */
	mcp2515_cmd_rts(0b00000001);
}
