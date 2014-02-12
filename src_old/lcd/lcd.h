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


void lcd_init(void);

void lcd_custom_store(uint8_t code, const uint8_t data[static 8]);
void lcd_custom_load(uint8_t code, uint8_t data[static 8]);

void lcd_clear(void);

void lcd_goto(uint8_t r, uint8_t c);
void lcd_goto_r(uint8_t r);
void lcd_goto_c(uint8_t c);

void lcd_rel(int8_t delta_r, int8_t delta_c);
void lcd_rel_r(int8_t delta_r);
void lcd_rel_c(int8_t delta_c);

void lcd_write(char chr);
char lcd_read(void);


#endif
