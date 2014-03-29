/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/font.h"
#include "dev/lcd/font_04b_03.h"
#include "dev/lcd/lcd.h"


static struct {
	uint8_t row;
	uint8_t col;
} cur;


void _lcd_newline(void) {
	cur.row += 6;
	// TODO: return to top if we would now clip thru the bottom
}


void lcd_set_cur(uint8_t row, uint8_t col) {
	cur.row = row;
	cur.col = col;
}


void lcd_draw_chr(char c) {
	/* only draw the printable ASCII set, and handle CR & NL specially */
	if (c < ' ' || c > '~') {
		if (c == '\r') {
			cur.col = 0;
		} else if (c == '\n') {
			_lcd_newline();
		}
		
		return;
	}
	
	uint8_t sprite[5];
	uint8_t mask[5];
	uint8_t width;
	
	for (uint8_t i = 0; i < 5; ++i) {
		sprite[i] = pgm_read_byte(font_04b_03_glyphs + (((c - ' ') * 5) + i));
		mask[i]   = pgm_read_byte(font_04b_03_mask + i);
	}
	width = pgm_read_byte(font_04b_03_widths + (c - ' '));
	
	/* automatically go to the next line if we overflow */
	if (cur.col + width >= LCD_COLS) {
		_lcd_newline();
		cur.col = 0;
	}
	
	uint8_t space = 0b00000000;
	lcd_draw_sprite(cur.row, cur.col, width, sprite, mask);
	lcd_draw_sprite(cur.row, cur.col + width, 1, &space, mask);
	
	cur.col += (width + 1);
}

void lcd_draw_str(const char *s) {
	while (*s != '\0') {
		lcd_draw_chr(*(s++));
	}
}
