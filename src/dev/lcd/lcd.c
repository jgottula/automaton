/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/lcd.h"
#include "dev/lcd/st7565.h"


void lcd_init(void) {
	st7565_init();
}


void lcd_onoff(bool on) {
	st7565_cmd(ST7565_DISP_ON_OFF | (on ? 0b1 : 0b0));
}

void lcd_contrast(uint8_t value) {
	st7565_cmd(ST7565_VOLUME_SET);
	st7565_cmd(value & 0x3f);
}
