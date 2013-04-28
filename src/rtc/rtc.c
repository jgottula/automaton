/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "rtc/rtc.h"
#include "conv/bcd.h"
#include "rtc/ds1302.h"


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
}
