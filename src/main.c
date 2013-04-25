/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
#include "button/button.h"
#include "debug/die.h"
#include "debug/stdfile.h"
#include "io/spi.h"
#include "lcd/lcd.h"
#include "obd/stn1110.h"
#include "uart/direct.h"
#include "uart/uart.h"
#include "ui/ui.h"


static void main_init(const char *pstr) {
	printf_P(PSTR("main: init %S\n"), pstr);
}


noreturn void main(void) {
	uint8_t mcusr = MCUSR;
	MCUSR = 0;
	
	wdt_disable();
	
	uart_direct_write_P(UART_PC, UART_DIV_115200,
		PSTR("\n[[automaton][pc uart]]\n"));
	
	uart_init(UART_PC, UART_DIV_115200, 0, 0);
	stdfile_open();
	
	sei();
	fputs_P(PSTR("main: uart ok\n"), stdout);
	
	main_init(PSTR("lcd"));
	lcd_init();
	/*fputs_P(PSTR("AUTOmaton\r\n"), lcd);*/
	
	main_init(PSTR("button"));
	button_init();
	
	main_init(PSTR("spi"));
	spi_init();
	
	main_init(PSTR("stn1110"));
	stn1110_init();
	
	fputs_P(PSTR("main: init ok\n"), stdout);
	
	ui_loop();
	
	die();
}
