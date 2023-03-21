/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/gps/gps.h"
#include "dev/gps/mt3339.h"


// how this all will work:
// receive characters into a buffer from the GPS UART as they arrive
// when a CRLF is received, parse the sentence received (do this in the RX int?)
// keep a bunch of globals in this file that will contain the most recent
//  GPGSA, GPGSV, etc (update these as we parse sentences)
// also keep a global for the most recent PMTK_ACK result
// update a counter or something to indicate when any of these have been changed
// accesses need to be int-safe if we are updating the globals in ISRs
// provide public functions that will interpret these globals for the main code

// more on how RX parsing will work:
// set up a DPC-like mechanism, so that RX int can be high priority, yet the
//  parsing code can be low priority
// if the RX int needs to do some processing, it should activate a one-shot
//  quick timer with low int priority
// this timer ISR will then poll for stuff to do, and do it
// this way, if any high priority interrupts are pending during the RX int, they
//  will preempt the lower-priority parsing work


void gps_init(void) {
	
}


// will we actually use this, or will we parse sentences in the uart RX ISR?
void gps_poll(void) {
	// TODO: call this occasionally from the main loop (every several msec)
	
	// while (GPS UART has available characters) {
	//  receive available char into buffer
	//  if we just got a CRLF, interpret the sentence
	// }
}


void gps_old_stuff(void) {
	// old old old
	
	
	// TODO: gps uart baud rate
	// apparently settable up to 921600
	// use $PMTK251,<baud>*<checksum><CR><LF>
	// (according to UC530 datasheet)
	
	
	//_delay_ms(1000);
	
	// set update rate
	// set NMEA baud rate
	// turn off DGPS (*)
	// turn off SBAS (*)
	// turn off AIC (*)
	// turn off EASY (*)
	// turn off QZSS (doesn't seem to work)
	
	// full cold restart: reset everything
	char cmd0[] = "$PMTK104*37\r\n";
	for (char *c = cmd0; *c != '\0'; ++c) {
		gps_uart_write(*c);
		uart_write(*c);
	}
	
	// DGPS: 
	/*char cmd1[] = "$PMTK301,1*2D\r\n";
	for (char *c = cmd1; *c != '\0'; ++c) {
		gps_uart_write(*c);
		uart_write(*c);
	}*/
	
	// SBAS: on
	char cmd2[] = "$PMTK313,1*2E\r\n";
	for (char *c = cmd2; *c != '\0'; ++c) {
		gps_uart_write(*c);
		uart_write(*c);
	}
	
	// query EPO
	/*char cmd3[] = "$PMTK607*33\r\n";
	for (char *c = cmd3; *c != '\0'; ++c) {
		gps_uart_write(*c);
		uart_write(*c);
	}*/
	
	// active interference cancellation: on
	char cmd4[] = "$PMTK286,1*23\r\n";
	for (char *c = cmd4; *c != '\0'; ++c) {
		gps_uart_write(*c);
		uart_write(*c);
	}
	
	// EASY function: on
	char cmd5[] = "$PMTK869,1,1*35\r\n";
	for (char *c = cmd5; *c != '\0'; ++c) {
		gps_uart_write(*c);
		uart_write(*c);
	}
	
	// QZSS: 
	/*char cmd6[] = "$PMTK352,0*2B\r\n";
	for (char *c = cmd6; *c != '\0'; ++c) {
		gps_uart_write(*c);
		uart_write(*c);
	}*/
	
	uint8_t bl_mode = 0;
	uint8_t bl_cnt  = 0;
	
	char buf[128];
	uint8_t len = 0;
	for(;;) {
		if (bl_mode == 0) {
			lcd_bl_rgb(255, bl_cnt, 0);
		} else if (bl_mode == 1) {
			lcd_bl_rgb(255 - bl_cnt, 255, 0);
		} else if (bl_mode == 2) {
			lcd_bl_rgb(0, 255, bl_cnt);
		} else if (bl_mode == 3) {
			lcd_bl_rgb(0, 255 - bl_cnt, 255);
		} else if (bl_mode == 4) {
			lcd_bl_rgb(bl_cnt, 0, 255);
		} else if (bl_mode == 5) {
			lcd_bl_rgb(255, 0, 255 - bl_cnt);
		}
		
		char c;
		while (gps_uart_read(&c)) {
			if (c == '\n') {
				buf[len] = '\0';
				if (strncmp_P(buf, PSTR("$GPGSV"), 6) == 0 ||
					strncmp_P(buf, PSTR("$GPGSA"), 6) == 0) {
					fprintf(lcd, "%s\n", buf);
				}
				printf("%s\n", buf);
				
				len = 0;
			} else if (c != '\r') {
				buf[len++] = c;
			}
		}
		
		if (bl_cnt++ == 255) {
			if (++bl_mode == 6) {
				bl_mode = 0;
			}
		}
		
		_delay_ms(2);
	}
}
