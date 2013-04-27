/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_RTC_DS1302_H
#define AUTOMATON_RTC_DS1302_H


#include "std.h"


enum ds1302_ck_reg {
	DS1302_CKR_SEC     = 0x00,
	DS1302_CKR_MIN     = 0x01,
	DS1302_CKR_HOUR    = 0x02,
	DS1302_CKR_DATE    = 0x03,
	DS1302_CKR_MONTH   = 0x04,
	DS1302_CKR_DAY     = 0x05,
	DS1302_CKR_YEAR    = 0x06,
	DS1302_CKR_WR_PROT = 0x07,
	DS1302_CKR_TC_SEL  = 0x08,
};


void ds1302_init(void);

void ds1302_ram_write(uint8_t addr, uint8_t val);
void ds1302_ck_write(uint8_t addr, uint8_t val);
uint8_t ds1302_ram_read(uint8_t addr);
uint8_t ds1302_ck_read(uint8_t addr);

void ds1302_ck_write_all(const uint8_t data[static 8]);
void ds1302_ck_read_all(uint8_t data[static 8]);


#endif
