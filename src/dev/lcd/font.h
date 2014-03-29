/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_LCD_FONT_H
#define AUTOMATON_DEV_LCD_FONT_H


#include "std.h"


void lcd_set_cur(uint8_t row, uint8_t col);

void lcd_draw_chr(char c);
void lcd_draw_str(const char *s);


#endif
