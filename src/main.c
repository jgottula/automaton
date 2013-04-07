/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
#include "debug/reset.h"
#include "io/uart.h"
#include "lcd/hd44780.h"


noreturn void main(void) {
	reset_defuse();
	sei();
	
	uart_init(UART_DEBUG, UART_DIV_115200, 100, 100);
	uart_write_pstr(UART_DEBUG, "automaton: debug uart\n");
	
	uart_write_pstr(UART_DEBUG, "init hd44780\n");
	lcd_init();
	lcd_write_pstr("automaton\r\n");
	
	/*uart_write_pstr(UART_DEBUG, "init iso9141-2\n");
	iso_init();*/
	
	/*uart_write_pstr("input loop\n");
	for ( ; ; ) {
		char r;
		if (uart_read_chr(&r)) {
			switch (r) {
			case '\x03':
				reset();
			}
		}
		
		uint8_t iso_r;
		if (iso_read(&iso_r)) {
			uart_write_hex8(iso_r);
			lcd_write_hex8(iso_r);
		}
	}*/
	
	die();
}
