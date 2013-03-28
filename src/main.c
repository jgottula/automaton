/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
#include "lib.h"
#include "lcd.h"


noreturn void main(void) {
	led_set(false);
	
	_delay_ms(10000);
	
	lcd_init();
	for ( ; ; ) {
		lcd_put('A');
	}
	
	die();
}
