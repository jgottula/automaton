/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_IO_SPI_H
#define AUTOMATON_IO_SPI_H


#include "std.h"


enum spi_mode {
	SPI_MODE_0 = 0,
	SPI_MODE_1 = _BV(CPHA),
	SPI_MODE_2 = _BV(CPOL),
	SPI_MODE_3 = _BV(CPOL) | _BV(CPHA),
};

enum spi_endian {
	SPI_MSB_FIRST = 0,
	SPI_LSB_FIRST = _BV(DORD),
};

/* bit 7 is the double-speed bit */
enum spi_divisor {
#if F_CPU == 20000000UL
	SPI_DIV_156K  = _BV(SPR1) | _BV(SPR0),
	SPI_DIV_312K  = _BV(SPR1),
	SPI_DIV_625K  = _BV(SPR1) | _BV(7),
	SPI_DIV_1_25M = _BV(SPR0),
	SPI_DIV_2_5M  = _BV(SPR0) | _BV(7),
	SPI_DIV_5M    = 0,
	SPI_DIV_10M   = 0 | _BV(7),
#elif F_CPU == 10000000UL
	SPI_DIV_78K   = _BV(SPR1) | _BV(SPR0),
	SPI_DIV_156K  = _BV(SPR1),
	SPI_DIV_312K  = _BV(SPR1) | _BV(7),
	SPI_DIV_625M  = _BV(SPR0),
	SPI_DIV_1_25M = _BV(SPR0) | _BV(7),
	SPI_DIV_2_5M  = 0,
	SPI_DIV_5M    = 0 | _BV(7),
#else
#error SPI clock dividers are wrong
#endif
};

enum spi_devices {
	SPI_DS1302_MODE   = SPI_MODE_2,
	SPI_DS1302_ENDIAN = SPI_LSB_FIRST,
	SPI_DS1302_DIV    = SPI_DIV_1_25M,
};


void spi_init(void);

void spi_begin(uint8_t slave, uint8_t settings);
void spi_end(void);

uint8_t spi_xfer(uint8_t byte);


#endif
