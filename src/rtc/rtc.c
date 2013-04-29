/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "rtc/rtc.h"
#include "conv/bcd.h"
#include "rtc/ds1302.h"


#define RTC_DEBUG_GET 0 // print info in rtc_get_tm
#define RTC_DEBUG_SET 0 // print info in rtc_set_tm


void rtc_init(void) {
	ds1302_init();
}


void rtc_get_tm(struct tm *tm) {
	uint8_t ck[8];
	ds1302_ck_read_all(ck);
	
	/* no handling for 12-hour time; you get what you deserve if you use that */
	tm->tm_sec  = bcd_decode_packed(ck[DS1302_CK_SEC] & 0x7f);
	tm->tm_min  = bcd_decode_packed(ck[DS1302_CK_MIN]);
	tm->tm_hour = bcd_decode_packed(ck[DS1302_CK_HOUR]);
	tm->tm_mday = bcd_decode_packed(ck[DS1302_CK_DATE]);
	tm->tm_wday = bcd_decode_packed(ck[DS1302_CK_DAY]) - 1;
	tm->tm_mon  = bcd_decode_packed(ck[DS1302_CK_MONTH]) - 1;
	tm->tm_year = bcd_decode_packed(ck[DS1302_CK_YEAR]) + 100;
	
	/* we don't set tm->tm_yday */
	
	tm->tm_isdst = -1;
	
#if RTC_DEBUG_GET
	fputs_P(PSTR("rtc_get_tm:\n====================\n"), stdout);
	
	printf_P(PSTR("ck[SEC]     = 0x%02x\n"), ck[DS1302_CK_SEC]);
	printf_P(PSTR("ck[MIN]     = 0x%02x\n"), ck[DS1302_CK_MIN]);
	printf_P(PSTR("ck[HOUR]    = 0x%02x\n"), ck[DS1302_CK_HOUR]);
	printf_P(PSTR("ck[DATE]    = 0x%02x\n"), ck[DS1302_CK_DATE]);
	printf_P(PSTR("ck[MONTH]   = 0x%02x\n"), ck[DS1302_CK_MONTH]);
	printf_P(PSTR("ck[DAY]     = 0x%02x\n"), ck[DS1302_CK_DAY]);
	printf_P(PSTR("ck[YEAR]    = 0x%02x\n"), ck[DS1302_CK_YEAR]);
	printf_P(PSTR("ck[WR_PROT] = 0x%02x\n"), ck[DS1302_CK_WR_PROT]);
	
	printf_P(PSTR("tm_sec   = %d\n"), tm->tm_sec);
	printf_P(PSTR("tm_min   = %d\n"), tm->tm_min);
	printf_P(PSTR("tm_hour  = %d\n"), tm->tm_hour);
	printf_P(PSTR("tm_mday  = %d\n"), tm->tm_mday);
	printf_P(PSTR("tm_mon   = %d\n"), tm->tm_mon);
	printf_P(PSTR("tm_wday  = %d\n"), tm->tm_wday);
	printf_P(PSTR("tm_year  = %d\n"), tm->tm_year);
	printf_P(PSTR("tm_yday  = %d\n"), tm->tm_yday);
	printf_P(PSTR("tm_isdst = %d\n"), tm->tm_isdst);
#endif
}

void rtc_set_tm(const struct tm *tm) {
	uint8_t ck[8];
	
	ck[DS1302_CK_SEC]   = bcd_encode_packed(tm->tm_sec);
	ck[DS1302_CK_MIN]   = bcd_encode_packed(tm->tm_min);
	ck[DS1302_CK_HOUR]  = bcd_encode_packed(tm->tm_hour);
	ck[DS1302_CK_DATE]  = bcd_encode_packed(tm->tm_mday);
	ck[DS1302_CK_MONTH] = bcd_encode_packed(tm->tm_mon + 1);
	ck[DS1302_CK_DAY]   = bcd_encode_packed(tm->tm_wday + 1);
	ck[DS1302_CK_YEAR]  = bcd_encode_packed(tm->tm_year - 100);
	
	ck[DS1302_CK_WR_PROT] = 0x00;
	
	ds1302_ck_write_all(ck);
	
#if RTC_DEBUG_SET
	fputs_P(PSTR("rtc_set_tm:\n====================\n"), stdout);
	
	printf_P(PSTR("ck[SEC]     = 0x%02x\n"), ck[DS1302_CK_SEC]);
	printf_P(PSTR("ck[MIN]     = 0x%02x\n"), ck[DS1302_CK_MIN]);
	printf_P(PSTR("ck[HOUR]    = 0x%02x\n"), ck[DS1302_CK_HOUR]);
	printf_P(PSTR("ck[DATE]    = 0x%02x\n"), ck[DS1302_CK_DATE]);
	printf_P(PSTR("ck[MONTH]   = 0x%02x\n"), ck[DS1302_CK_MONTH]);
	printf_P(PSTR("ck[DAY]     = 0x%02x\n"), ck[DS1302_CK_DAY]);
	printf_P(PSTR("ck[YEAR]    = 0x%02x\n"), ck[DS1302_CK_YEAR]);
	printf_P(PSTR("ck[WR_PROT] = 0x%02x\n"), ck[DS1302_CK_WR_PROT]);
	
	printf_P(PSTR("tm_sec   = %d\n"), tm->tm_sec);
	printf_P(PSTR("tm_min   = %d\n"), tm->tm_min);
	printf_P(PSTR("tm_hour  = %d\n"), tm->tm_hour);
	printf_P(PSTR("tm_mday  = %d\n"), tm->tm_mday);
	printf_P(PSTR("tm_mon   = %d\n"), tm->tm_mon);
	printf_P(PSTR("tm_wday  = %d\n"), tm->tm_wday);
	printf_P(PSTR("tm_year  = %d\n"), tm->tm_year);
	printf_P(PSTR("tm_yday  = %d\n"), tm->tm_yday);
	printf_P(PSTR("tm_isdst = %d\n"), tm->tm_isdst);
#endif
}
