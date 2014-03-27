/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
#include "dev/can/can.h"
#include "dev/lcd/backlight.h"
#include "dev/lcd/font.h"
#include "dev/lcd/lcd.h"
#include "dev/uart/uart.h"
#include "mcu/clock.h"
#include "mcu/pmic.h"
#include "obd/obd.h"


int main(void) {
	mcu_setup_xtal();
	mcu_setup_pll();
	mcu_use_pll();
	
	pmic_init();
	
	uart_init();
	stdfile_open();
	
	
	lcd_bl_init();
	//button_init();
	
	lcd_init();
	lcd_onoff(true);
	lcd_bl_rgb(255, 0, 0);
	
	fputs_P(PSTR("\nhello from automaton!\n"), stdout);
	
	//can_init();
	obd_test();
	
	
	// TODO: test 16-bit fifo functions
	// TODO: test ASSERT()
	
	
	/*uart_direct_write_pstr(UART_PC, UART_DIV_PC,
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
	
	
	ui_loop();*/
	
	die();
}
