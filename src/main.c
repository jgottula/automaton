/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
#include "lib.h"
#include "lcd.h"
#include "uart.h"


noreturn void main(void) {
	io_write(MCUSR, _BV(WDRF), 0);
	wdt_disable();
	
	led_set(false);
	
	uart_init();
	uart_write_pstr("hello world\n");
	
	lcd_init();
	lcd_put('A');
	uart_write_pstr("done\n");
	
	for ( ; ; ) {
		char r;
		if (uart_read_chr(&r)) {
			switch (r) {
			case '\x03':
				reset();
			}
		}
		
	}
	
	die();
}
