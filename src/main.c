/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
#include "dev/lcd/lcd.h"
#include "mcu/clock.h"


#if 0
static void _main_init(const char *pstr) {
	printf_P(PSTR("main: init %S\n"), pstr);
}


static void _DEBUG_passthru_uart(void) {
	for ( ; ; ) {
		/*char c;
		
		while ((c = fgetc(stdin)) != EOF) {
			fputc(c, stn1110);
		}
		
		while ((c = fgetc(stn1110)) != EOF) {
			fputc(c, stdout);
		}*/
		
		while (uart_avail(UART_PC) != 0) {
			char c;
			
			if (uart_read(UART_PC, &c)) {
				uart_write(UART_STN1110, c);
			}
		}
		
		while (uart_avail(UART_STN1110) != 0) {
			char c;
			
			if (uart_read(UART_STN1110, &c)) {
				uart_write(UART_PC, c);
			}
		}
	}
}


#warning remove me
extern uint8_t mcusr;
#endif


int main(void) {
	// test LCD backlight stuff
	PORTE.DIRSET = 0b00000111;
	PORTE.OUT = 0b00000000;
	for (uint8_t i = 0; i < 8; ++i) {
		PORTE.OUT = (PORTE.OUT + 1) & 0b111;
		_delay_ms(25);
	}
	
	mcu_use_xtal();
	
	// test LCD backlight stuff
	PORTE.DIRSET = 0b00000111;
	PORTE.OUT = 0b00000000;
	for (uint8_t i = 0; i < 8; ++i) {
		PORTE.OUT = (PORTE.OUT + 1) & 0b111;
		_delay_ms(200);
	}
	
	mcu_use_pll();
	
	// test LCD backlight stuff
	PORTE.DIRSET = 0b00000111;
	PORTE.OUT = 0b00000000;
	for (uint8_t i = 0; i < 8; ++i) {
		PORTE.OUT = (PORTE.OUT + 1) & 0b111;
		_delay_ms(400);
	}
	
	for (;;);
	
	
/*	reset_defuse();
	
	
	uart_direct_write_pstr(UART_PC, UART_DIV_PC,
		PSTR("\n[[automaton][pc uart]]\n"));
	
	
	timer1_start();
	
	uart_init(UART_PC, UART_DIV_PC, 0, 0);
	stdfile_open();
	
	sei();
	fputs_P(PSTR("main: uart ok\n"), stdout);
	
	
	_main_init(PSTR("lcd"));
	lcd_init();
	
	fputs_P(PSTR("init..."), lcd);
	
	_main_init(PSTR("clock"));
	clock_init();
	
	_main_init(PSTR("button"));
	button_init();
	
	_main_init(PSTR("spi"));
	spi_init();
	
	_main_init(PSTR("sd"));
	sd_init();
	
	fputs_P(PSTR("\rinit stn1110..."), lcd);
	
	_main_init(PSTR("stn1110"));
	stn1110_init();
	
	lcd_clear();
	fputs_P(PSTR("main: init ok\n"), stdout);
	
	
	_DEBUG_passthru_uart();
	
	
	ui_loop();
	
	
	die();*/
}
