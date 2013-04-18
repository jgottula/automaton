/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
#include "debug/die.h"
#include "debug/reset.h"
#include "debug/stdfile.h"
#include "io/spi.h"
#include "io/uart.h"
#include "lcd/hd44780.h"


noreturn void main(void) {
	uint8_t mcusr = MCUSR;
	MCUSR = 0;
	
	wdt_disable();
	sei();
	
	uart_init(UART_DEBUG, UART_DIV_115200, 100, 100);
	stdfile_open();
	
	printf_P(PSTR("automaton: debug uart\n"));
	
	printf_P(PSTR("init hd44780\n"));
	lcd_init();
	fprintf_P(lcd, PSTR("AUTOmaton\r\n"));
	
	printf_P(PSTR("init spi\n"));
	spi_init();
	
	
	
	printf_P(PSTR("done\n"));
	
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
