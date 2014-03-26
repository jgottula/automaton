/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/can/can.h"
#include "dev/can/mcp2515.h"


void can_init(void) {
	mcp2515_init();
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
