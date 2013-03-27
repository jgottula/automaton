/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_LCD_H
#define AUTOMATON_LCD_H


#include "std.h"
#include "lib.h"


enum lcd_instr {
	LCD_INSTR_CLEAR      = 0,
	LCD_INSTR_CUR_HOME   = 1,
	LCD_INSTR_ENT_MODE   = 2,
	LCD_INSTR_ONOFF      = 3,
	LCD_INSTR_SHIFT      = 4,
	LCD_INSTR_FUNC_SET   = 5,
	LCD_INSTR_CGRAM_ADDR = 6,
	LCD_INSTR_DDRAM_ADDR = 7,
};

enum lcd_bus_bit {
	/* entry mode */
	LCD_BUS_S  = _BV(0),
	LCD_BUS_ID = _BV(1),
	
	/* on/off */
	LCD_BUS_B = _BV(0),
	LCD_BUS_C = _BV(1),
	LCD_BUS_D = _BV(2),
	
	/* shift */
	LCD_BUS_RL = _BV(2),
	LCD_BUS_SC = _BV(3),
	
	/* function set */
	LCD_BUS_F  = _BV(2),
	LCD_BUS_N  = _BV(3),
	LCD_BUS_DL = _BV(4),
	
	/* read addr */
	LCD_BUS_BF = _BV(7),
};


#endif
