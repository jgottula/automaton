/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/gfx.h"
#include "dev/lcd/lcd.h"


extern uint8_t lcd_buf[(LCD_ROWS * LCD_COLS) / 8];

void _lcd_mod_set(uint8_t page, uint8_t col, uint8_t len);
void _lcd_mod_set_all(void);


void lcd_draw_pixel(uint8_t row, uint8_t col, uint8_t mode) {
	uint8_t page  = row / 8;
	uint8_t shift = row % 8;
	
	uint8_t *tgt = lcd_buf + ((page * LCD_COLS) + col);
	
	/* bounds checks */
	if (row >= LCD_ROWS || col >= LCD_COLS) {
		return;
	}
	
	_lcd_mod_set(page, col, 1);
	
	uint8_t mask = (1 << shift);
	
	if (mode == GFX_MODE_ON) {
		*tgt |= mask;
	} else if (mode == GFX_MODE_OFF) {
		*tgt &= ~mask;
	} else if (mode == GFX_MODE_INVERT) {
		*tgt ^= mask;
	}
}


void lcd_draw_hline(uint8_t row, uint8_t col, uint8_t length, uint8_t mode) {
	uint8_t page  = row / 8;
	uint8_t shift = row % 8;
	
	uint8_t *tgt = lcd_buf + ((page * LCD_COLS) + col);
	
	/* bounds checks */
	if (row >= LCD_ROWS || col >= LCD_COLS) {
		return;
	}
	if ((uint16_t)col + (uint16_t)length > LCD_COLS) {
		length = LCD_COLS - col;
	}
	
	_lcd_mod_set(page, col, length);
	
	uint8_t mask = (1 << shift);
	
	for (uint8_t i = 0; i < length; ++i) {
		if (mode == GFX_MODE_ON) {
			*(tgt++) |= mask;
		} else if (mode == GFX_MODE_OFF) {
			*(tgt++) &= ~mask;
		} else if (mode == GFX_MODE_INVERT) {
			*(tgt++) ^= mask;
		}
	}
}

void lcd_draw_vline(uint8_t row, uint8_t col, uint8_t length, uint8_t mode) {
	/* bounds checks */
	if (row >= LCD_ROWS || col >= LCD_COLS) {
		return;
	}
	if ((uint16_t)row + (uint16_t)length > LCD_ROWS) {
		length = LCD_ROWS - row;
	}
	
	while (length != 0) {
		uint8_t page  = row / 8;
		uint8_t shift = row % 8;
		
		uint8_t *tgt = lcd_buf + ((page * LCD_COLS) + col);
		
		_lcd_mod_set(page, col, 1);
		
		uint8_t mask = 0b11111111;
		uint8_t seg_len = 8;
		
		if (length < 8) {
			mask >>= (8 - length);
		}
		if (shift != 0) {
			mask <<= shift;
			seg_len -= shift;
		}
		
		if (seg_len > length) {
			seg_len = length;
		}
		
		if (mode == GFX_MODE_ON) {
			*tgt |= mask;
		} else if (mode == GFX_MODE_OFF) {
			*tgt &= ~mask;
		} else if (mode == GFX_MODE_INVERT) {
			*tgt ^= mask;
		}
		
		length -= seg_len;
		row    += seg_len;
	}
}

void lcd_draw_dline(uint8_t row, uint8_t col, uint8_t width, uint8_t height,
	uint8_t mode) {
	// TODO
}


void lcd_draw_rect_outline(uint8_t row, uint8_t col, uint8_t width,
	uint8_t height, uint8_t mode) {
	/* bounds checks */
	if (width < 2 || height < 2) {
		return;
	}
	
	lcd_draw_hline(row, col, width, mode);
	lcd_draw_hline(row + (height - 1), col, width, mode);
	
	if (height > 2) {
		lcd_draw_vline(row + 1, col, height - 2, mode);
		lcd_draw_vline(row + 1, col + (width - 1), height - 2, mode);
	}
}

void lcd_draw_rect_filled(uint8_t row, uint8_t col, uint8_t width,
	uint8_t height, uint8_t mode) {
	/* bounds checks */
	if (row >= LCD_ROWS || col >= LCD_COLS) {
		return;
	}
	if ((uint16_t)row + (uint16_t)height > LCD_ROWS) {
		height = LCD_ROWS - row;
	}
	if ((uint16_t)col + (uint16_t)width > LCD_COLS) {
		width = LCD_COLS - col;
	}
	
	while (height != 0) {
		uint8_t page  = row / 8;
		uint8_t shift = row % 8;
		
		uint8_t *tgt = lcd_buf + ((page * LCD_COLS) + col);
		
		_lcd_mod_set(page, col, width);
		
		uint8_t mask = 0b11111111;
		uint8_t seg_len = 8;
		
		if (height < 8) {
			mask >>= (8 - height);
		}
		if (shift != 0) {
			mask <<= shift;
			seg_len -= shift;
		}
		
		if (seg_len > height) {
			seg_len = height;
		}
		
		for (uint8_t i = 0; i < width; ++i) {
			if (mode == GFX_MODE_ON) {
				*(tgt++) |= mask;
			} else if (mode == GFX_MODE_OFF) {
				*(tgt++) &= ~mask;
			} else if (mode == GFX_MODE_INVERT) {
				*(tgt++) ^= mask;
			}
		}
		
		height -= seg_len;
		row    += seg_len;
	}
}


void lcd_draw_sprite(uint8_t row, uint8_t col, uint8_t width,
	const uint8_t sprite[static width], const uint8_t mask[static width]) {
	uint8_t page  = row / 8;
	uint8_t shift = row % 8;
	
	uint8_t *tgt = lcd_buf + ((page * LCD_COLS) + col);
	
	/* bounds checks */
	if (row >= LCD_ROWS || col >= LCD_COLS) {
		return;
	}
	if ((uint16_t)col + (uint16_t)width > LCD_COLS) {
		width = LCD_COLS - col;
	}
	
	_lcd_mod_set(page, col, width);
	
	if (shift == 0) { /* no page crossings: easy */
		for (uint8_t i = 0; i < width; ++i) {
			*tgt = (*tgt & ~mask[i]) | sprite[i];
			
			++tgt;
		}
	} else if (page >= 7) { /* screen-bottom overrun; clip it */
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
		
		_lcd_mod_set(page + 1, col, width);
	}
}
