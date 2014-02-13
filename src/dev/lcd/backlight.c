/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/backlight.h"


void lcd_bl_init(void) {
	/* turn off backlight for now */
	PORTE.OUTSET = 0b00000111;
	PORTE.DIRSET = 0b00000111;
}

/* TODO: PWM!! and have function(s) to set RGB color */
