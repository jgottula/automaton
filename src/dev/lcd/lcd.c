/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/lcd.h"
#include "dev/lcd/st7565.h"


uint8_t lcd_buf[(LCD_ROWS * LCD_COLS) / 8];


void _lcd_clear_ddram(void) {
	for (uint8_t i = 0; i < ST7565_PAGES; ++i) {
		st7565_cmd(ST_CMD_PAGE_ADDR | i);
		st7565_cmd(ST_CMD_COL_ADDR_HI | 0b0000);
		st7565_cmd(ST_CMD_COL_ADDR_LO | 0b0000);
		
		for (uint8_t j = 0; j < ST7565_COLS; ++j) {
			st7565_data(0b00000000);
		}
	}
}


void lcd_init(void) {
	st7565_init();
	_lcd_clear_ddram();
}


void lcd_onoff(bool on) {
	st7565_cmd(ST_CMD_ON_OFF | (on ? 0b1 : 0b0));
}

void lcd_contrast(uint8_t value) {
	st7565_cmd(ST_CMD_VOLUME);
	st7565_cmd(value & 0x3f);
}


void lcd_clear(void) {
	
}


void lcd_update(void) {
	// TODO: use a rectangle to confine updates to only changed pixels
	
	for (uint8_t i = 0; i < ST7565_PAGES; ++i) {
		st7565_cmd(ST_CMD_PAGE_ADDR | i);
		st7565_cmd(ST_CMD_COL_ADDR_HI | 0b0000);
		st7565_cmd(ST_CMD_COL_ADDR_LO | 0b0001);
		
		for (uint8_t j = 0; j < LCD_COLS; ++j) {
			st7565_data(lcd_buf[(i * LCD_COLS) + j]);
		}
	}
}


void lcd_draw_sprite(uint8_t r, uint8_t c, uint8_t width,
	const uint8_t sprite[static width], const uint8_t mask[static width]) {
	uint8_t *tgt = lcd_buf + (((r / 8) * LCD_COLS) + c);
	uint8_t shift = r % 8;
	
	/* prevent overruns */
	if (c + width > LCD_COLS) {
		width = LCD_COLS - c;
	}
	
	if (shift == 0) { /* no page crossings: easy */
		for (uint8_t i = 0; i < width; ++i) {
			*tgt = (*tgt & ~mask[i]) | sprite[i];
			
			++tgt;
		}
	} else if ((r / 8) >= 7) { /* screen-bottom overrun; clip it */
		for (uint8_t i = 0; i < width; ++i) {
			uint8_t mask_top   = mask[i] << shift;
			uint8_t sprite_top = sprite[i] << shift;
			
			*tgt = (*tgt & ~mask_top) | sprite_top;
			
			++tgt;
		}
	} else { /* normal case for cross-page sprites */
		uint8_t *tgt_below = tgt + LCD_COLS;
		
		for (uint8_t i = 0; i < width; ++i) {
			uint8_t mask_top      = mask[i] << shift;
			uint8_t sprite_top    = sprite[i] << shift;
			uint8_t mask_bottom   = mask[i] >> (8 - shift);
			uint8_t sprite_bottom = sprite[i] >> (8 - shift);
			
			*tgt       = (*tgt & ~mask_top) | sprite_top;
			*tgt_below = (*tgt_below & ~mask_bottom) | sprite_bottom;
			
			++tgt;
			++tgt_below;
		}
	}
}
