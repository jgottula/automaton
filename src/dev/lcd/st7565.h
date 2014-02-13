/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_LCD_ST7565_H
#define AUTOMATON_DEV_LCD_ST7565_H


#include "std.h"


enum {
	ST7565_DISP_ON_OFF      = 0b10101110,
	ST7565_START_LINE_SET   = 0b01000000,
	ST7565_PAGE_ADDR_SET    = 0b10110000,
	ST7565_COL_ADDR_SET_HI  = 0b00010000,
	ST7565_COL_ADDR_SET_LO  = 0b00000000,
	ST7565_ADC_SELECT       = 0b10100000,
	ST7565_DISP_REVERSE     = 0b10100110,
	ST7565_DISP_ALL_PTS     = 0b10100100,
	ST7565_LCD_BIAS_SET     = 0b10100010,
	ST7565_RMW_SET          = 0b11100000,
	ST7565_RMW_RESET        = 0b11101110,
	ST7565_RESET            = 0b11100010,
	ST7565_COM_MODE_SELECT  = 0b11000000,
	ST7565_PWR_CTRL_SET     = 0b00101000,
	ST7565_RESIST_RATIO_SET = 0b00100000,
	ST7565_VOLUME_SET       = 0b10000001,
	ST7565_STATIC_ON_OFF    = 0b10101100,
};


void st7565_init(void);

void st7565_cmd(uint8_t cmd);
void st7565_data(uint8_t data);


#endif
