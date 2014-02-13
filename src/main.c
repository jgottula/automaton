/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
#include "dev/lcd/backlight.h"
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


#include "dev/lcd/st7565.h"
int main(void) {
	mcu_setup_xtal();
	mcu_setup_pll();
	mcu_use_pll();
	
	lcd_bl_init();
	//button_init();
	
	PORTE.OUT = 0b00000000;
	lcd_init();
	lcd_onoff(true);
	
	//st7565_cmd(ST7565_DISP_ALL_PTS | 0b1);
	
	for (uint16_t i = 0; i < 8; ++i) {
		st7565_cmd(ST7565_PAGE_ADDR_SET | i);
		st7565_cmd(ST7565_COL_ADDR_SET_HI | 0b0000);
		st7565_cmd(ST7565_COL_ADDR_SET_LO | 0b0000);
		
		for (uint16_t j = 0; j < 128; ++j) {
			if (j % 2 == 0) {
				st7565_data(0b10101010);
			} else {
				st7565_data(0b01010101);
			}
		}
		
		st7565_data(0b00000000);
		st7565_data(0b00000000);
		st7565_data(0b00000000);
		st7565_data(0b00000000);
		st7565_data(0b00000000);
	}
	
	
	uint8_t colors[] = {
		0b000, // white
		0b011, // red
		0b001, // yellow
		0b101, // green
		0b100, // cyan
		0b110, // blue
		0b010, // purple
	};
	for (;;) {
		for (uint8_t i = 0; i < sizeof(colors); ++i) {
			_delay_ms(500);
			PORTE.OUT = colors[i];
		}
	}
	
	
	
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
}
