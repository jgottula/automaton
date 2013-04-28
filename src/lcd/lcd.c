/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "lcd/lcd.h"
#include "lcd/hd44780.h"


enum lcd_init_param {
	LCD_INIT_FUNC_SET =
		HD44780_BUS_4BIT | HD44780_LINES_TWO | HD44780_FONT_5X8,
	LCD_INIT_ONOFF =
		HD44780_DISPLAY_ON | HD44780_CUR_HIDE | HD44780_BLINK_OFF,
	LCD_INIT_ENT_MODE =
		HD44780_ADDR_INCR | HD44780_SHIFT_DISABLE,
};


struct lcd_state {
	bool init;
	
	uint8_t ent_mode;
	uint8_t onoff;
	uint8_t func_set;
	
	uint8_t addr;
	
	struct {
		uint8_t r;
		uint8_t c;
	} cur;
};


struct lcd_state lcd_state;
const struct lcd_state lcd_state_init PROGMEM = {
	.init = false,
	
	.ent_mode = LCD_INIT_ENT_MODE,
	.onoff    = LCD_INIT_ONOFF,
	.func_set = LCD_INIT_FUNC_SET,
	
	.addr = 0x00,
	
	.cur = {
		.r = 0,
		.c = 0,
	},
};


/* set the hd44780 addr to what we have locally */
static void _lcd_send_ddaddr(void) {
	if (!lcd_state.init) {
		return;
	}
	
	hd44780_set_ddaddr(lcd_state.addr);
}


/* calculate the ddram addr based on changed lcd_state.cur.{x,y} */
static void _lcd_cur_set(bool send) {
	_Static_assert(LCD_ROWS == 4 && LCD_COLS == 20,
		"lcd cursor assumptions wrong");
	
	uint8_t new_addr = lcd_state.cur.c;
	if ((lcd_state.cur.r % 2) == 0) {
		new_addr += (lcd_state.cur.r * 10);
	} else {
		new_addr += 64 + ((lcd_state.cur.r - 1) * 10);
	}
	
	lcd_state.addr = new_addr;
	if (send) {
		_lcd_send_ddaddr();
	}
}

/* update local cursor values to match the hd44780's auto-increment activity */
static void _lcd_cur_adv(void) {
	/* we assume that the address counter is set to increment */
	if (++lcd_state.cur.c == LCD_COLS) {
		lcd_state.cur.c = 0;
		if (++lcd_state.cur.r == LCD_ROWS) {
			lcd_state.cur.r = 0;
		}
		
		_lcd_cur_set(true);
	} else {
		_lcd_cur_set(false);
	}
}


/* initialize the lcd using the initial parameters */
void lcd_init(void) {
	hd44780_init(LCD_INIT_FUNC_SET, LCD_INIT_ONOFF, LCD_INIT_ENT_MODE);
	
	memcpy_P(&lcd_state, &lcd_state_init, sizeof(lcd_state));
	
	lcd_state.init = true;
}


/* store a 5x8 character in cgram */
void lcd_custom_store(uint8_t code, const uint8_t data[static 8]) {
	if (!lcd_state.init) {
		return;
	}
	
	code &= 0x07;
	
	hd44780_set_cgaddr(code * 8);
	hd44780_write(8, data);
	
	/* restore the previous ddram address */
	_lcd_send_ddaddr();
}

/* load a 5x8 character from cgram */
void lcd_custom_load(uint8_t code, uint8_t data[static 8]) {
	if (!lcd_state.init) {
		return;
	}
	
	code &= 0x07;
	
	hd44780_read_cgram(code * 8, 8, data);
	
	/* restore the previous ddram address */
	_lcd_send_ddaddr();
}


/* clear the screen and return the cursor to home */
void lcd_clear(void) {
	if (!lcd_state.init) {
		return;
	}
	
	lcd_goto(0, 0);
	hd44780_clear();
}


/* set the cursor position */
void lcd_goto(uint8_t r, uint8_t c) {
	lcd_state.cur.r = (r % LCD_ROWS);
	lcd_state.cur.c = (c % LCD_COLS);
	_lcd_cur_set(true);
}

/* set the cursor position (row only) */
void lcd_goto_r(uint8_t r) {
	lcd_state.cur.r = (r % LCD_ROWS);
	_lcd_cur_set(true);
}

/* set the cursor position (col only) */
void lcd_goto_c(uint8_t c) {
	lcd_state.cur.c = (c % LCD_COLS);
	_lcd_cur_set(true);
}


/* adjust the cursor position */
void lcd_rel(int8_t delta_r, int8_t delta_c) {
	lcd_goto(lcd_state.cur.r + delta_r, lcd_state.cur.c + delta_c);
}

/* adjust the cursor position (row only) */
void lcd_rel_r(int8_t delta_r) {
	lcd_goto_r(lcd_state.cur.r + delta_r);
}

/* adjust the cursor position (col only) */
void lcd_rel_c(int8_t delta_c) {
	lcd_goto_c(lcd_state.cur.c + delta_c);
}


/* write a character to ddram; update cursor based on \r and \n */
void lcd_write(char chr) {
	if (!lcd_state.init) {
		return;
	}
	
	if (chr == '\r') {
		lcd_goto_c(0);
		return;
	} else if (chr == '\n') {
		lcd_goto_r(lcd_state.cur.r + 1);
		return;
	}
	
	hd44780_write(1, (const uint8_t *)&chr);
	
	_lcd_cur_adv();
}

/* read a character from ddram */
char lcd_read(void) {
	if (!lcd_state.init) {
		return '\0';
	}
	
	char chr;
	hd44780_read_ddram(lcd_state.addr, 1, (uint8_t *)&chr);
	
	_lcd_cur_adv();
	return chr;
}
