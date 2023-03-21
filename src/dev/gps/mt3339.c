/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/gps/mt3339.h"
#include "dev/gps/nmea0183.h"


// provide function for PMTK_CMD_HOT_START, for example
// this function creates a sentence and calls nmea0183_tx() or whatever
// then, it waits for the ISR thread to receive a PMTK_ACK and update the global
//  that represents the most recent ACK status
// the function then returns the ACK status


uint8_t mt3339_hot_start(void) {
	
}

uint8_t mt3339_warm_start(void) {
	
}

uint8_t mt3339_cold_start(void) {
	
}

uint8_t mt3339_full_cold_start(void) {
	
}


uint8_t mt3339_set_nmea_updaterate(/* ? */) {
	
}
