/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "lcd/lcd.h"
#include "lcd/hd44780.h"


enum lcd_init_param {
	LCD_INIT_FUNC_SET =
		HD44780_BUS_8BIT | HD44780_LINES_TWO | HD44780_FONT_5X8,
	LCD_INIT_ONOFF =
		HD44780_DISPLAY_ON | HD44780_CUR_HIDE | HD44780_BLINK_OFF,
	LCD_INIT_ENT_MODE =
		HD44780_ADDR_INCR | HD44780_SHIFT_DISABLE,
};


struct lcd_state {
	uint8_t ent_mode;
	uint8_t onoff;
	uint8_t func_set;
	
	uint8_t addr;
	
	struct {
		uint8_t x;
		uint8_t y;
	} cur;
};


struct lcd_state state;
const struct lcd_state state_init PROGMEM = {
	.ent_mode = LCD_INIT_ENT_MODE,
	.onoff    = LCD_INIT_ONOFF,
	.func_set = LCD_INIT_FUNC_SET,
	
	.addr = 0x00,
	
	.cur = {
		.x = 0,
		.y = 0,
	},
};

FILE *lcd = NULL;


static void lcd_set_cur(void) {
	/*uint8_t new_addr = lcd_cur.x;
	if ((lcd_cur.y % 2) == 0) {
		new_addr += (lcd_cur.y * 10);
	} else {
		new_addr += 64 + ((lcd_cur.y - 1) * 10);
	}
	
	hd44780_set_ddaddr(new_addr);*/
}


static int lcd_file_put(char c, FILE *f) {
	(void)f;
	
	lcd_write_chr(c);
	return 0;
}


void lcd_init(void) {
	hd44780_init(LCD_INIT_FUNC_SET, LCD_INIT_ONOFF, LCD_INIT_ENT_MODE);
	
	memcpy_P(&state, &state_init, sizeof(state));
	
	lcd = fdevopen(lcd_file_put, NULL);
}


void lcd_goto_xy(uint8_t x, uint8_t y) {
	/*lcd_cur.x = (x % 20);
	lcd_cur.y = (y % 4);
	lcd_set_cur();*/
}

void lcd_goto_x(uint8_t x) {
	/*lcd_cur.x = (x % 20);
	lcd_set_cur();*/
}

void lcd_goto_y(uint8_t y) {
	/*lcd_cur.y = (y % 4);
	lcd_set_cur();*/
}


void lcd_write_chr(char chr) {
#if 0
	if (chr == '\r') {
		lcd_goto_x(0);
		return;
	} else if (chr == '\n') {
		lcd_goto_y(lcd_cur.y + 1);
		return;
	}
	
	lcd_write_data(chr);
	
	/* handle cursor wrapping */
	if (++lcd_cur.x == 20) {
		lcd_cur.x = 0;
		if (++lcd_cur.y == 4) {
			lcd_cur.y = 0;
		}
		lcd_set_cur();
	}
#endif
}
