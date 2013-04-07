/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_LCD_HD44780_H
#define AUTOMATON_LCD_HD44780_H


#include "std.h"


enum hd44780_instr {
	HD44780_INSTR_CLEAR      = 0,
	HD44780_INSTR_HOME       = 1,
	HD44780_INSTR_ENT_MODE   = 2,
	HD44780_INSTR_ONOFF      = 3,
	HD44780_INSTR_SHIFT      = 4,
	HD44780_INSTR_FUNC_SET   = 5,
	HD44780_INSTR_CGRAM_ADDR = 6,
	HD44780_INSTR_DDRAM_ADDR = 7,
};

enum hd44780_instr_bit {
	/* entry mode */
	HD44780_IBIT_S  = _BV(0),
	HD44780_IBIT_ID = _BV(1),
	
	/* on/off */
	HD44780_IBIT_B = _BV(0),
	HD44780_IBIT_C = _BV(1),
	HD44780_IBIT_D = _BV(2),
	
	/* shift */
	HD44780_IBIT_RL = _BV(2),
	HD44780_IBIT_SC = _BV(3),
	
	/* function set */
	HD44780_IBIT_F  = _BV(2),
	HD44780_IBIT_N  = _BV(3),
	HD44780_IBIT_DL = _BV(4),
	
	/* read addr */
	HD44780_IBIT_BF = _BV(7),
};

enum hd44780_param {
	/* entry mode: address increment/decrement */
	HD44780_ADDR_DECR = 0,
	HD44780_ADDR_INCR = HD44780_IBIT_ID,
	
	/* entry mode: display shift */
	HD44780_SHIFT_DISABLE = 0,
	HD44780_SHIFT_ENABLE  = HD44780_IBIT_S,
	
	/* on/off: display power */
	HD44780_DISPLAY_OFF = 0,
	HD44780_DISPLAY_ON  = HD44780_IBIT_D,
	
	/* on/off: cursor visibility */
	HD44780_CUR_HIDE = 0,
	HD44780_CUR_SHOW = HD44780_IBIT_C,
	
	/* on/off: cursor blink */
	HD44780_BLINK_OFF = 0,
	HD44780_BLINK_ON  = HD44780_IBIT_B,
	
	/* shift: cursor or display move */
	HD44780_SHIFT_CUR  = 0,
	HD44780_SHIFT_DISP = HD44780_IBIT_SC,
	
	/* shift: direction */
	HD44780_SHIFT_LEFT  = 0,
	HD44780_SHIFT_RIGHT = HD44780_IBIT_RL,
	
	/* function set: bus width */
	HD44780_BUS_4BIT = 0,
	HD44780_BUS_8BIT = HD44780_IBIT_DL,
	
	/* function set: display lines */
	HD44780_LINES_ONE = 0,
	HD44780_LINES_TWO = HD44780_IBIT_N,
	
	/* function set: font size */
	HD44780_FONT_5X8  = 0,
	HD44780_FONT_5X10 = HD44780_IBIT_F,
};


void hd44780_init(uint8_t func_set, uint8_t onoff, uint8_t ent_mode);

void hd44780_read_cgram(uint8_t addr, uint8_t len, uint8_t data[static len]);
void hd44780_read_ddram(uint8_t addr, uint8_t len, uint8_t data[static len]);
void hd44780_write(uint8_t len, const uint8_t data[static len]);

uint8_t hd44780_get_addr(void);
void hd44780_set_cgaddr(uint8_t addr);
void hd44780_set_ddaddr(uint8_t addr);

void hd44780_clear(void);
void hd44780_home(void);
void hd44780_ent_mode(uint8_t param);
void hd44780_onoff(uint8_t param);
void hd44780_shift(uint8_t param);
void hd44780_func_set(uint8_t param);


#endif
