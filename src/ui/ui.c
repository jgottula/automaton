/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "ui/ui.h"
#include "lcd/lcd.h"
#include "ui/sleep.h"
#include "ui/time.h"


struct ui_state ui = {
	.page = UI_PAGE_DEFAULT,
};


static void _ui_init(void) {
	
}


void ui_loop(void) {
	_ui_init();
	
	for ( ; ; ) {
		switch (ui.page) {
		case UI_PAGE_SLEEP:
			ui_page_sleep();
			break;
		case UI_PAGE_TIME:
			ui_page_time();
			break;
		default:
			ASSERT(0);
		}
	}
}


void ui_header(const char *pstr) {
	lcd_clear();
	
	char c;
	while ((c = pgm_read_byte(pstr++)) != '\0') {
		fprintf_P(lcd, PSTR("%c|\r\n"), c);
	}
}




#if 0
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
	
	/*alarm_set(1000);*/
	
	char obd_buf[64];
	uint8_t obd_idx = 0;
	
	bool do_cmd = true;
	for ( ; ; ) {
		int c;
		
		/*bool do_cmd = false;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if (timer0_count >= 1000) {
				do_cmd = true;
				timer0_count -= 1000;
			}
		}*/
		
		if (do_cmd) {
			// get throttle position
			fputs_P(PSTR("0111\n"), stn1110);
			obd_idx = 0;
			do_cmd = false;
		}
		
		/* repeatedly receive and transmit data until neither are available */
		bool got_data;
		do {
			got_data = false;
			
			/* tx */
			/*if (uart_avail(UART_PC) && (c = fgetc(stdin)) != EOF) {
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
			}*/
			
			/* rx */
			if (uart_avail(UART_STN1110) && (c = fgetc(stn1110)) != EOF) {
				fputc(c, stdout);
				
				if (c != '\n') {
					obd_buf[obd_idx++] = c;
				} else {
					obd_buf[obd_idx] = '\0';
					
					/*// REMOVE ME
					strcpy_P(obd_buf, PSTR("10 41 e8"));*/
					
					lcd_clear();
					// TODO: just overwrite
					
					/*static uint8_t counter = 0;*/
					
					uint8_t discard, accel;
					if (sscanf_P(obd_buf, PSTR("%hhx %hhx %hhx"),
						&discard, &discard, &accel)== 3) {
						/*accel = counter++;*/
						
						int16_t accel_i = (int16_t)accel - 40;
						accel_i *= 100;
						accel_i /= (205 - 40);
						
						int16_t accel_b = (accel_i * 4) / 5;
						if (accel_b < 0) accel_b = 0;
						if (accel_b > 100) accel_b = 100;
						
						fprintf_P(lcd, PSTR("%3d"), accel_i);
						fputc(' ', lcd);
						
						while (accel_b >= 5) {
							lcd_write(0xff);
							accel_b -= 5;
						}
						if (accel_b > 0) {
							lcd_write(accel_b - 1);
						}
					} else {
						fprintf_P(lcd, PSTR("error:\r\n%s"), obd_buf);
					}
					
					do_cmd = true;
					
					/*_delay_ms(100);*/
				}
				
				got_data = true;
			}
		} while (got_data);
		
		// TODO: go to sleep instead
		_delay_us(10);
	}
}
#endif
