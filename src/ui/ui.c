/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "ui/ui.h"
#include "lcd/lcd.h"
#include "obd/stn1110.h"
#include "time/alarm.h"
#include "uart/uart.h"


// TODO: log all stn1110 TX and RX comm lines to the PC uart

extern volatile uint16_t timer0_count;


static const uint8_t custom_chars[][8] PROGMEM = {
	{
		0b00010000,
		0b00010000,
		0b00010000,
		0b00010000,
		0b00010000,
		0b00010000,
		0b00010000,
		0b00010000,
	}, {
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
	}, {
		0b00011100,
		0b00011100,
		0b00011100,
		0b00011100,
		0b00011100,
		0b00011100,
		0b00011100,
		0b00011100,
	}, {
		0b00011110,
		0b00011110,
		0b00011110,
		0b00011110,
		0b00011110,
		0b00011110,
		0b00011110,
		0b00011110,
	}, {
		/* unused */
	}, {
		/* unused */
	}, {
		/* unused */
	}, {
		/* unused */
	},
};


static void _ui_init_lcd(void) {
	uint8_t buf[8];
	
	for (uint8_t i = 0; i < 4; ++i) {
		memcpy_P(buf, custom_chars[i], 8);
		lcd_custom_store(i, buf);
	}
}


void ui_loop(void) {
	_ui_init_lcd();
	
	alarm_set(1000);
	
	bool tx = false, rx = false;
	for ( ; ; ) {
		int c;
		
		bool do_cmd = false;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if (timer0_count >= 1000) {
				do_cmd = true;
				timer0_count -= 1000;
			}
		}
		
		if (do_cmd) {
			// vehicle speed
			fputs_P(PSTR("010d\n"), stn1110);
			
			/* copypasta from tx block below */
			if (!tx) {
				tx = true;
				rx = false;
				
				lcd_clear();
				lcd_goto_xy(0, 0);
				fputs_P(PSTR(">>\r\n>>\r\n<<\r\n<<"), lcd);
				lcd_goto_xy(3, 0);
			}
			fputs_P(PSTR("010d"), lcd);
		}
		
		/* repeatedly receive and transmit data until neither are available */
		bool got_data;
		do {
			got_data = false;
			
			/* tx */
			if (uart_avail(UART_PC) && (c = fgetc(stdin)) != EOF) {
				fputc(c, stn1110);
				
				if (!tx) {
					tx = true;
					rx = false;
					
					lcd_clear();
					lcd_goto_xy(0, 0);
					fputs_P(PSTR(">>\r\n>>\r\n<<\r\n<<"), lcd);
					lcd_goto_xy(3, 0);
				}
				
				if (c != '\n') {
					fputc(c, lcd);
				}
				
				got_data = true;
			}
			
			/* rx */
			if (uart_avail(UART_STN1110) && (c = fgetc(stn1110)) != EOF) {
				fputc(c, stdout);
				
				if (!rx) {
					tx = false;
					rx = true;
					
					lcd_goto_xy(3, 2);
				}
				
				if (c != '\n') {
					fputc(c, lcd);
				}
				
				got_data = true;
			}
		} while (got_data);
		
		// TODO: go to sleep instead
		_delay_us(10);
	}
}
