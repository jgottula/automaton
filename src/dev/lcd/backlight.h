/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_LCD_BACKLIGHT_H
#define AUTOMATON_DEV_LCD_BACKLIGHT_H


#include "std.h"


void lcd_bl_init(void);

void lcd_bl_rgb(uint8_t red, uint8_t green, uint8_t blue);


#endif
