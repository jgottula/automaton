/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "sd/phy.h"
#include "algo/crc7.h"
#include "io/spi.h"


void sd_phy_init(void) {
	/* use pull-up on card detect pin */
	IO_WRITE(PORT(IO_SD), IO_SD_DETECT, IO_SD_DETECT);
	IO_WRITE(DDR(IO_SD), IO_SD_DETECT, 0);
}


bool sd_phy_card_present(void) {
	return (IO_READ(PIN(IO_SD), IO_SD_DETECT) == 0);
}

// TODO: generic command and app-command functions
// (different kind for each permutation of return type etc)
