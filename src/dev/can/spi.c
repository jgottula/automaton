/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/can/spi.h"


void can_spi_init(void) {
	/* set SS high, and set SS, SCK, MOSI as output; MISO as input */
	PORTD.OUTSET = 0b00010000;
	PORTD.DIRSET = 0b10110000;
	PORTD.DIRCLR = 0b01000000;
	
	/* enable SPI with MSB first, master mode, mode 00, and 8 MHz */
	SPID.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_0_gc |
		SPI_PRESCALER_DIV4_gc;
}


void can_spi_select(void) {
	PORTD.OUTCLR = 0b00010000;
}

void can_spi_deselect(void) {
	PORTD.OUTSET = 0b00010000;
}


uint8_t can_spi_xfer(uint8_t byte) {
	SPID.DATA = byte;
	while (!(SPID.STATUS & SPI_IF_bm));
	
	return SPID.DATA;
}
