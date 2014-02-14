/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_LCD_ST7565_H
#define AUTOMATON_DEV_LCD_ST7565_H


#include "std.h"


enum {
	ST7565_PAGES = 9,
	
	ST7565_ROWS = 65,
	ST7565_COLS = 132,
	
	ST7565_START_LINE = 32,
	
	ST_CMD_ON_OFF       = 0b10101110,
	ST_CMD_START_LINE   = 0b01000000,
	ST_CMD_PAGE_ADDR    = 0b10110000,
	ST_CMD_COL_ADDR_HI  = 0b00010000,
	ST_CMD_COL_ADDR_LO  = 0b00000000,
	ST_CMD_ADC          = 0b10100000,
	ST_CMD_REVERSE      = 0b10100110,
	ST_CMD_ALL_PTS      = 0b10100100,
	ST_CMD_LCD_BIAS     = 0b10100010,
	ST_CMD_RMW_SET      = 0b11100000,
	ST_CMD_RMW_RESET    = 0b11101110,
	ST_CMD_RESET        = 0b11100010,
	ST_CMD_COM          = 0b11000000,
	ST_CMD_PWR_CTRL     = 0b00101000,
	ST_CMD_RESIST_RATIO = 0b00100000,
	ST_CMD_VOLUME       = 0b10000001,
	ST_CMD_STATIC       = 0b10101100,
};


void st7565_init(void);

void st7565_cmd(uint8_t cmd);
void st7565_data(uint8_t data);


#endif
