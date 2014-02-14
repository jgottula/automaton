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


void lcd_draw_chr(char c) {
	/* only handle the printable ASCII set */
	if (c < ' ' || c > '~') {
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
	
	uint8_t space = 0b00000000;
	lcd_draw_sprite(cur.row, cur.col, 1, &space, mask);
	lcd_draw_sprite(cur.row, cur.col + 1, width, sprite, mask);
	
	cur.col += (width + 1);
}

void lcd_draw_str(const char *s) {
	while (*s != '\0') {
		lcd_draw_chr(*(s++));
	}
}
