/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/lcd.h"
#include "dev/lcd/st7565.h"


uint8_t lcd_buf[(LCD_ROWS * LCD_COLS) / 8];
static struct {
	uint8_t min;
	uint8_t max;
} lcd_mod[LCD_PAGES];


static void _lcd_clear_ddram(void) {
	for (uint8_t i = 0; i < ST7565_PAGES; ++i) {
		st7565_cmd(ST_CMD_PAGE_ADDR | i);
		st7565_cmd(ST_CMD_COL_ADDR_HI | 0b0000);
		st7565_cmd(ST_CMD_COL_ADDR_LO | 0b0000);
		
		for (uint8_t j = 0; j < ST7565_COLS; ++j) {
			st7565_data(0b00000000);
		}
	}
}

static void _lcd_mod_reset(void) {
	for (uint8_t i = 0; i < LCD_PAGES; ++i) {
		lcd_mod[i].min = 255;
		lcd_mod[i].max = 0;
	}
}

void _lcd_mod_set(uint8_t page, uint8_t col, uint8_t len) {
	uint8_t from = col;
	uint8_t to = (col + len) - 1;
	
	uint8_t *min = &lcd_mod[page].min;
	uint8_t *max = &lcd_mod[page].max;
	
	if (from < *min) {
		*min = from;
	}
	if (to > *max) {
		*max = to;
	}
}

void _lcd_mod_set_all(void) {
	for (uint8_t i = 0; i < LCD_PAGES; ++i) {
		lcd_mod[i].min = 0;
		lcd_mod[i].max = LCD_COLS - 1;
	}
}


void lcd_init(void) {
	st7565_init();
	
	_lcd_clear_ddram();
	_lcd_mod_reset();
}


void lcd_onoff(bool on) {
	st7565_cmd(ST_CMD_ON_OFF | (on ? 0b1 : 0b0));
}

void lcd_contrast(uint8_t value) {
	st7565_cmd(ST_CMD_VOLUME);
	st7565_cmd(value & 0x3f);
}


void lcd_clear(void) {
	memset(lcd_buf, 0, sizeof(lcd_buf));
	_lcd_mod_set_all();
}


void lcd_update(bool force) {
	for (uint8_t i = 0; i < LCD_PAGES; ++i) {
		uint8_t col = 0;
		uint8_t len = LCD_COLS;
		
		if (!force) {
			uint8_t min = lcd_mod[i].min;
			uint8_t max = lcd_mod[i].max;
			
			if (min > max) {
				continue;
			}
			
			col = min;
			len = (max - min) + 1;
		}
		
		st7565_cmd(ST_CMD_PAGE_ADDR | i);
		st7565_cmd(ST_CMD_COL_ADDR_HI | (((col + 1) >> 4) & 0x0f));
		st7565_cmd(ST_CMD_COL_ADDR_LO | ((col + 1) & 0x0f));
		
		for (uint8_t j = 0; j < len; ++j) {
			st7565_data(lcd_buf[(i * LCD_COLS) + col + j]);
		}
	}
	
	_lcd_mod_reset();
}
