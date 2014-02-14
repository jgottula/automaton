/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_LCD_LCD_H
#define AUTOMATON_DEV_LCD_LCD_H


#include "std.h"


enum {
	LCD_PAGES = 8,
	LCD_ROWS  = 64,
	LCD_COLS  = 128,
};


void lcd_init(void);

void lcd_onoff(bool on);
void lcd_contrast(uint8_t value);

void lcd_clear(void);

void lcd_update(bool force);

void lcd_draw_sprite(uint8_t row, uint8_t col, uint8_t width,
	const uint8_t sprite[static width], const uint8_t mask[static width]);


#endif
