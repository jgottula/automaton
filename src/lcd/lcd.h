/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_LCD_LCD_H
#define AUTOMATON_LCD_LCD_H


#include "std.h"


#define LCD_ROWS 4
#define LCD_COLS 20


extern FILE *lcd;


void lcd_init(void);

void lcd_custom_store(uint8_t code, const uint8_t data[static 8]);
void lcd_custom_load(uint8_t code, uint8_t data[static 8]);

void lcd_goto_xy(uint8_t x, uint8_t y);
void lcd_goto_x(uint8_t x);
void lcd_goto_y(uint8_t y);

void lcd_write(char chr);


#endif
