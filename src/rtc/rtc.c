/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "rtc/rtc.h"
#include "rtc/ds1302.h"


// put this somewhere else!
static uint8_t bcd_decode(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0f);
}


void rtc_init(void) {
	ds1302_init();
	
	
	ds1302_ck_write(DS1302_CKR_SEC, 0x00);
	
	
	for (uint8_t addr = 0x00; addr < 0x20; ++addr) {
		uint8_t reg = ds1302_ck_read(addr);
		printf_P(PSTR("reg 0x%02hx: %02hx\n"), addr, reg);
	}
	
	
	uint8_t b_year  = ds1302_ck_read(DS1302_CKR_YEAR);
	uint8_t b_month = ds1302_ck_read(DS1302_CKR_MONTH);
	uint8_t b_date  = ds1302_ck_read(DS1302_CKR_DATE);
	uint8_t b_day   = ds1302_ck_read(DS1302_CKR_DAY);
	
	uint8_t b_hour   = ds1302_ck_read(DS1302_CKR_HOUR);
	uint8_t b_minute = ds1302_ck_read(DS1302_CKR_MIN);
	uint8_t b_second = ds1302_ck_read(DS1302_CKR_SEC);
	
	
	printf_P(PSTR("%u %u %u %u %u %u %u\n"),
		b_year, b_month, b_date, b_day, b_hour, b_minute, b_second);
	
	
	uint16_t i_year  = bcd_decode(b_year) + 2000;
	uint8_t  i_month = bcd_decode(b_month);
	uint8_t  i_date  = bcd_decode(b_date);
	uint8_t  i_day   = bcd_decode(b_day);
	
	//uint8_t i_hour
	
	
	// need s_... values for days of week and months maybe
	
	
	// pretty-print on lcd
	//fprintf_P(lcd, PSTR(""), )
}
