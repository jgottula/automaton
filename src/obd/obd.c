/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "obd/obd.h"
#include "dev/can/can.h"


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
}


void obd_test(void) {
	printf_P(PSTR("obd_test()\n"));
	uart_flush();
	
	_delay_ms(100); // TODO: remove me!
	can_init(500);
	_delay_ms(100); // TODO: remove me!
	
	_obd_get_pid_bitmask();
	
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
	
	
	_delay_ms(500);
	
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
	_delay_ms(50); // TODO: get rid of this delay!
	
	if (can_rx(&msg)) {
		uint8_t len = msg.data[0];
		ASSERT(len > 2);
		
		ASSERT(msg.data[1] == 0x41);
		ASSERT(msg.data[2] == pid);
		
		memcpy(data, msg.data + 3, len - 2);
		return len - 2;
	} else {
		fputs_P(PSTR("obd_query: can_rx fail\n"), stdout);
		return 0;
	}
}
