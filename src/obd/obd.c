/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "obd/obd.h"
#include "dev/can/can.h"
#include "dev/lcd/font.h"
#include "dev/lcd/gfx.h"
#include "dev/lcd/lcd.h"


static uint8_t pid_bitmask[32];


static void _obd_get_pid_bitmask(void) {
	uint8_t data[5];
	uint8_t len;
	
	for (uint8_t i = 0; i < 8; ++i) {
		uint8_t pid = i * 0x20;
		if (!obd_check_pid_support(pid)) {
			break;
		}
		
		ASSERT((len = obd_query(pid, data)) == 4);
		memcpy(pid_bitmask + (i * 4), data, 4);
	}
	
	// TODO: rewrite this so we don't assert on length mismatch
	// and, rely on obd_query's future support for pid support bitmask check
}


static void _obd_draw_axes(void) {
	lcd_clear();
	
	/* x-axis */
	lcd_draw_hline(57, 12, 116, GFX_MODE_ON);
	lcd_draw_vline(58, 13, 2, GFX_MODE_ON);
	lcd_draw_pixel(58, 23, GFX_MODE_ON);
	lcd_draw_vline(58, 33, 2, GFX_MODE_ON);
	lcd_draw_pixel(58, 43, GFX_MODE_ON);
	lcd_draw_vline(58, 53, 2, GFX_MODE_ON);
	lcd_draw_pixel(58, 63, GFX_MODE_ON);
	lcd_draw_vline(58, 73, 2, GFX_MODE_ON);
	lcd_draw_pixel(58, 83, GFX_MODE_ON);
	lcd_draw_vline(58, 93, 2, GFX_MODE_ON);
	lcd_draw_pixel(58, 103, GFX_MODE_ON);
	lcd_draw_vline(58, 113, 2, GFX_MODE_ON);
	lcd_draw_pixel(58, 123, GFX_MODE_ON);
	lcd_set_cur(59, 19); lcd_draw_str("1");
	lcd_set_cur(59, 39); lcd_draw_str("3");
	lcd_set_cur(59, 59); lcd_draw_str("5");
	lcd_set_cur(59, 79); lcd_draw_str("7");
	lcd_set_cur(59, 99); lcd_draw_str("9");
	lcd_set_cur(59, 119); lcd_draw_str("11");
	
	/* y-axis */
	lcd_draw_vline(0, 11, 58, GFX_MODE_ON);
	lcd_draw_hline(56, 9, 2, GFX_MODE_ON);
	lcd_draw_pixel(51, 10, GFX_MODE_ON);
	lcd_draw_hline(46, 9, 2, GFX_MODE_ON);
	lcd_draw_pixel(41, 10, GFX_MODE_ON);
	lcd_draw_hline(36, 9, 2, GFX_MODE_ON);
	lcd_draw_pixel(31, 10, GFX_MODE_ON);
	lcd_draw_hline(26, 9, 2, GFX_MODE_ON);
	lcd_draw_pixel(21, 10, GFX_MODE_ON);
	lcd_draw_hline(16, 9, 2, GFX_MODE_ON);
	lcd_draw_pixel(11, 10, GFX_MODE_ON);
	lcd_draw_hline(6, 9, 2, GFX_MODE_ON);
	lcd_draw_pixel(1, 10, GFX_MODE_ON);
	lcd_set_cur(54, 0); lcd_draw_str("0");
	lcd_set_cur(44, 0); lcd_draw_str("10");
	lcd_set_cur(34, 0); lcd_draw_str("20");
	lcd_set_cur(24, 0); lcd_draw_str("30");
	lcd_set_cur(14, 0); lcd_draw_str("40");
	lcd_set_cur(4, 0); lcd_draw_str("50");
	
	lcd_update(false);
}

static void _obd_draw_plot(uint8_t len, const uint8_t samples[static len],
	uint8_t head) {
	/* partial clear */
	lcd_draw_rect_filled(0, 13, 115, 56, GFX_MODE_OFF);
	
	uint8_t n = 0;
	uint8_t i = head;
	do {
		uint8_t kph = samples[i];
		uint8_t mph = CEIL((uint16_t)kph * 5, 8);
		
		/* saturate */
		if (mph > 56) {
			mph = 56;
		}
		
		if (mph > 0) {
			uint8_t col        = 127 - n;
			
			uint8_t row_bottom = 55;
			uint8_t row_top    = 56 - mph;
			
			uint8_t bar_height = (row_bottom - (row_top - 1));
			
			lcd_draw_vline(row_top, col, bar_height, GFX_MODE_ON);
		}
		
		++n;
		if (++i >= len) {
			i = 0;
		}
	} while (i != head);
	
	lcd_update(false);
}


void obd_test(void) {
	printf_P(PSTR("obd_test()\n"));
	uart_flush();
	
	_delay_ms(100); // TODO: remove me!
	can_init(500);
	_delay_ms(100); // TODO: remove me!
	
#warning TODO: re-enable PID bitmask retrieval!!
	//_obd_get_pid_bitmask();
	
	fputs_P(PSTR("\nOBD-II PID support:"), stdout);
	uint16_t total = 0;
	for (uint16_t i = 0; i < 256; ++i) {
		if (obd_check_pid_support(i)) {
			if (i % 0x20 == 0) {
				fputc('\n', stdout);
			} else {
				fputc(' ', stdout);
			}
			printf_P(PSTR("%02X"), i);
			
			++total;
		}
	}
	printf_P(PSTR("\n(total: %u)\n"), total);
	
	
	_obd_draw_axes();
	
	uint8_t samples[115];
	uint8_t head = 0;
	
	memset(samples, 0, sizeof(samples));
	
	for ( ; ; ) {
		uint8_t data[5];
		uint8_t len;
		
		uint8_t sample = 0;
		
		if ((len = obd_query(0x0d, data)) != 1) {
			/* error */
		} else {
			sample = data[0];
		}
		
		
		samples[head] = sample;
		if (++head >= sizeof(samples)) {
			head = 0;
		}
		
		_obd_draw_plot(sizeof(samples), samples, head);
		
		_delay_ms(90); // total of ~100ms delay
	}
	
#if 0
	for ( ; ; ) {
		lcd_set_cur(0, 0);
		
		uint8_t data[5];
		uint8_t len;
		
		if ((len = obd_query(0x0d, data)) != 1) {
			fputs_P(PSTR("error\n"), lcd);
		} else {
			uint16_t kmh = data[0];
			uint16_t mph = (kmh * 5) / 8;
			
			fprintf_P(lcd, PSTR("mph: %u            \n"), mph);
		}
		
		if ((len = obd_query(0x0c, data)) != 2) {
			fputs_P(PSTR("error\n"), lcd);
		} else {
			uint16_t rpm = (((uint16_t)data[0] * 256) + data[1]) / 4;
			
			fprintf_P(lcd, PSTR("rpm: %u            \n"), rpm);
		}
		
		_delay_ms(100);
	}
#endif
}


bool obd_check_pid_support(uint8_t pid) {
	/* PID 00 is always supported */
	if (pid == 0x00) {
		return true;
	}
	--pid;
	
	uint8_t byte = pid / 8;
	uint8_t bit  = pid % 8;
	
	uint8_t val  = pid_bitmask[byte];
	uint8_t mask = (1 << (7 - bit));
	
	return ((val & mask) != 0);
}


uint8_t obd_query(uint8_t pid, uint8_t data[static 5]) {
	struct can_msg msg = {
		.id = 0x7df,
		
		.dlc = 8,
		.data = {
			2,   // len
			1,   // mode
			pid, // pid
		},
	};
	
	// TODO: check pid against pid support bitmask
	
#if 0
	can_tx(&msg);
	
	while (!can_rx(&msg));
	
	uint8_t len = msg.data[0];
	ASSERT(len > 2);
	
	ASSERT(msg.data[1] == 0x41);
	ASSERT(msg.data[2] == pid);
	
	memcpy(data, msg.data + 3, len - 2);
	return len - 2;
#endif
	
	can_tx(&msg);
	_delay_ms(10); // TODO: get rid of this delay!
	
	if (can_rx(&msg)) {
		uint8_t len = msg.data[0];
		ASSERT(len > 2);
		
		ASSERT(msg.data[1] == 0x41);
		ASSERT(msg.data[2] == pid);
		
		memcpy(data, msg.data + 3, len - 2);
		return len - 2;
	} else {
		static uint16_t err_cnt = 0;
		printf_P(PSTR("obd_query: can_rx fail: %u\n"), err_cnt++);
		return 0;
	}
}
