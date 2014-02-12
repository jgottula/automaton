/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_IO_PORT_H
#define AUTOMATON_IO_PORT_H


#include "std.h"


//#define REG_MODIFY(_reg, _mask, _val) \
// 	(_reg) = (((_reg) & ~(_mask)) | ((_val) & (_mask)))


/* pinout:
PA0-7: BTN_0-7

PB0: SD_DAT1
PB1: SD_DAT2
PB2: CAN_INT
PB4-7: JTAG_TMS, _TDI, _TCK, _TDO

PC0-3: LCD_RESET, _SCK, _A0, _SID
PC4-7: RTC_SS, _MOSI, _MISO, _SCK
USARTC0 (SPI master mode): LCD
SPIC: RTC

PD0: CAN_STANDBY
PD1-3: GPS_TX, _RX, _1HZ
PD4-7: CAN_SS, _MOSI, _MISO, _SCK
USARTD0: GPS
SPID: CAN

PE0-2: LCD_BL_R-, _G-, _B-
PE3-7: SD_DETECT, _SS, _MOSI, _MISO, _SCK
SPIE: SD

PF1: RTC_INT
PF2-5: FTDI_TXD, _RXD, _RTS#, _CTS#
USARTF0: FTDI
*/


#if 0
#define DDR(_x) \
	CONCAT(DDR, _x)
#define PORT(_x) \
	CONCAT(PORT, _x)
#define PIN(_x) \
	CONCAT(PIN, _x)


#define IO_LCD_BUS  A
#define IO_BTN      A
#define IO_RTC      B
#define IO_SD       B
#define IO_SPI      B
#define IO_LED      D
#define IO_LCD_CTRL D


enum io_port_a {
	IO_LCD_BUS_ALL = _BV(PA0) | _BV(PA1) | _BV(PA2) | _BV(PA3),
	
	/* these must be contiguous */
	IO_BTN_A     = _BV(PA4),
	IO_BTN_B     = _BV(PA5),
	IO_BTN_C     = _BV(PA6),
	IO_BTN_D     = _BV(PA7),
	IO_BTN_ALL   = IO_BTN_A | IO_BTN_B | IO_BTN_C | IO_BTN_D,
	IO_BTN_FIRST = IO_BTN_A,
};

enum io_port_b {
	IO_RTC_IO   = _BV(PB0),
	IO_RTC_SCLK = _BV(PB1),
	IO_RTC_CE   = _BV(PB2),
	IO_RTC_ALL  = IO_RTC_IO | IO_RTC_SCLK | IO_RTC_CE,
	
	IO_SD_DETECT = _BV(PB3),
	
	/* warning: don't set PB4 as input, or spi master mode may be disabled */
	IO_SPI_SS_SD  = _BV(PB4),
	IO_SPI_SS_ALL = IO_SPI_SS_SD,
	
	IO_SPI_MOSI = _BV(PB5),
	IO_SPI_MISO = _BV(PB6),
	IO_SPI_SCK  = _BV(PB7),
	IO_SPI_ALL  = IO_SPI_MOSI | IO_SPI_MISO | IO_SPI_SCK,
};

/*enum io_port_c {*/
	/* PC2: JTAG */
	/* PC3: JTAG */
	/* PC4: JTAG */
	/* PC5: JTAG */
/*};*/

enum io_port_d {
	/* PD0: UART0 rx */
	/* PD1: UART0 tx */
	/* PD2: UART1 rx */
	/* PD3: UART1 tx */
	
	IO_LCD_CTRL_E   = _BV(PD4),
	IO_LCD_CTRL_RW  = _BV(PD5),
	IO_LCD_CTRL_RS  = _BV(PD6),
	IO_LCD_CTRL_ALL = IO_LCD_CTRL_E | IO_LCD_CTRL_RW | IO_LCD_CTRL_RS,
	
	IO_LED_0   = _BV(PD7),
	IO_LED_ALL = IO_LED_0,
};
#endif


#endif
