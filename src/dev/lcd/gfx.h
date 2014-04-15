/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_LCD_GFX_H
#define AUTOMATON_DEV_LCD_GFX_H


#include "std.h"


enum {
	GFX_MODE_ON     = 0b00,
	GFX_MODE_OFF    = 0b01,
	GFX_MODE_INVERT = 0b10,
};


/* TODO: change to xy; ALSO: fix font functions likewise */

void lcd_draw_pixel(uint8_t row, uint8_t col, uint8_t mode);

void lcd_draw_hline(uint8_t row, uint8_t col, uint8_t length, uint8_t mode);
void lcd_draw_vline(uint8_t row, uint8_t col, uint8_t length, uint8_t mode);
void lcd_draw_dline(uint8_t row, uint8_t col, uint8_t width, uint8_t height,
	uint8_t mode);

void lcd_draw_rect_outline(uint8_t row, uint8_t col, uint8_t width,
	uint8_t height,uint8_t mode);
void lcd_draw_rect_filled(uint8_t row, uint8_t col, uint8_t width,
	uint8_t height,uint8_t mode);

void lcd_draw_sprite(uint8_t row, uint8_t col, uint8_t width,
	const uint8_t sprite[static width], const uint8_t mask[static width]);


#endif
