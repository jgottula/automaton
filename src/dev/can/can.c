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
	if (kbps == 500) {
		/* BRP = 0 */
		mcp2515_setup_bit_timings(2, 0, 8, 8, 8);
	} else if (kbps == 250) {
		/* BRP = 1 */
		mcp2515_setup_bit_timings(2, 1, 8, 8, 8);
	} else {
		ASSERT(0);
	}
}


bool can_tx(const struct can_msg *msg) {
	struct mcp_tx_buf tx_buf;
	
	tx_buf.sid_h8 = (msg->id >> 3);
	tx_buf.sid_l3 = msg->id;
	
	tx_buf.exide  = 0;
	tx_buf.eid_h2 = 0;
	tx_buf.eid_m8 = 0;
	tx_buf.eid_l8 = 0;
	
	tx_buf.rtr = 0;
	
	tx_buf.dlc = msg->dlc;
	memcpy(tx_buf.d, msg->data, msg->dlc);
	
	
	uint8_t tx_buf_idx = mcp2515_choose_tx_buf();
	
	/* load tx buffer all at once */
	mcp2515_cmd_load_tx_buf(tx_buf_idx << 1, &tx_buf);
	
	/* request transmission */
	mcp2515_cmd_rts(1 << tx_buf_idx);
	
	// TODO: wait for TXnIF (or an error int) and return success/failure
	return true;
}

bool can_rx(struct can_msg *msg) {
	struct mcp_rx_buf *rx_buf;
	if ((rx_buf = mcp2515_get_rx_buf()) == NULL) {
		return false;
	}
	
	msg->id = ((uint16_t)rx_buf->sid_h8 << 3) | rx_buf->sid_l3;
	
	msg->dlc = rx_buf->dlc;
	memcpy(msg->data, rx_buf->d, rx_buf->dlc);
	
	free(rx_buf);
	return true;
}
