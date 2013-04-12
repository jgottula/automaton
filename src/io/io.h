/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_IO_IO_H
#define AUTOMATON_IO_IO_H


#include "std.h"


#define io_read(_reg, _mask) \
	(_reg & _mask)
#define io_write(_reg, _mask, _val) \
	(_reg) = (((_reg) & ~(_mask)) | ((_val) & (_mask)))


#define DDR(_x) \
	_CONCAT(DDR, _x)
#define PORT(_x) \
	_CONCAT(PORT, _x)
#define PIN(_x) \
	_CONCAT(PIN, _x)


#define IO_BTN      A
#define IO_SPI      B
#define IO_LCD_BUS  C
#define IO_LCD_CTRL D
#define IO_LED      D


/*enum io_port_a {
	
};*/

enum io_port_b {
	/* warning: don't set PB4 as input, or spi master mode may be disabled */
	IO_SPI_SS_SDCARD = _BV(PB3),
	IO_SPI_SS_DS1302 = _BV(PB4),
	IO_SPI_SS_ALL    = IO_SPI_SS_SDCARD | IO_SPI_SS_DS1302,
	
	IO_SPI_MOSI = _BV(PB5),
	IO_SPI_MISO = _BV(PB6),
	IO_SPI_SCK  = _BV(PB7),
	IO_SPI_ALL  = IO_SPI_MOSI | IO_SPI_MISO | IO_SPI_SCK,
};

enum io_port_c {
	IO_LCD_BUS_ALL = 0xff,
};

enum io_port_d {
	IO_LED_0   = _BV(PD4),
	IO_LED_ALL = IO_LED_0,
	
	IO_LCD_CTRL_E   = _BV(PD5),
	IO_LCD_CTRL_RW  = _BV(PD6),
	IO_LCD_CTRL_RS  = _BV(PD7),
	IO_LCD_CTRL_ALL = IO_LCD_CTRL_E | IO_LCD_CTRL_RW | IO_LCD_CTRL_RS,
};


#endif
