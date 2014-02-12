/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/st7565.h"
#include "dev/lcd/spi.h"


void st7565_init(void) {
	// cycle LCD_RESET low for 1ms, then set it high
	
	lcd_spi_init();
	
	// see page 42 of ST7565.pdf for initialization routine
}
