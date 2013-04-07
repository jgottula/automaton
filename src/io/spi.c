/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "io/spi.h"


static void spi_select(uint8_t slave) {
	io_write(PORT(IO_SPI), IO_SPI_SS_ALL, IO_SPI_SS_ALL ^ slave);
}

static void spi_deselect(void) {
	io_write(PORT(IO_SPI), IO_SPI_SS_ALL, IO_SPI_SS_ALL);
}


void spi_init(void) {
	io_write(DDR(IO_SPI), IO_SPI_ALL | IO_SPI_SS_ALL,
		(IO_SPI_ALL ^ IO_SPI_MISO) | IO_SPI_SS_ALL);
	io_write(PORT(IO_SPI), IO_SPI_ALL | IO_SPI_SS_ALL, IO_SPI_SS_ALL);
}


void spi_begin(uint8_t slave, uint8_t settings) {
	SPCR = _BV(SPIE) | _BV(MSTR) | (settings & ~_BV(7));
	SPSR = ((settings & _BV(7)) ? _BV(SPI2X) : 0);
	
	/* clear out any existing state */
	(void)SPSR;
	(void)SPDR;
	
	spi_select(slave);
}

void spi_end(void) {
	spi_deselect();
}


uint8_t spi_xfer(uint8_t byte) {
	SPDR = byte;
	
	while (!(SPSR & _BV(SPIF)));
	return SPDR;
}
