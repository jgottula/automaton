/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "rtc/ds1302.h"
#include "time/delay.h"


enum ds1302_cmd_bit {
	/* read/write */
	DS1302_CBIT_RD = _BV(0),
	DS1302_CBIT_WR = 0,
	
	/* ram or clock/calendar */
	DS1302_CBIT_RAM = _BV(6),
	DS1302_CBIT_CK  = 0,
	
	/* always one */
	DS1302_CBIT_ONE = _BV(7),
};

enum ds1302_addr_bit {
	/* ram or clock/calendar */
	DS1302_ABIT_RAM = _BV(5),
	DS1302_ABIT_CK  = 0,
};


static void ds1302_bitbang_init(void) {
	/* set CE and SCLK as outputs; set IO as input with no pull-up */
	IO_WRITE(PORT(IO_RTC), IO_RTC_ALL, 0);
	IO_WRITE(DDR(IO_RTC), IO_RTC_ALL, IO_RTC_CE | IO_RTC_SCLK);
}


static void ds1302_bitbang_io_input(void) {
	PORT(IO_RTC) |= IO_RTC_IO;
	DDR(IO_RTC) &= ~IO_RTC_IO;
}

static void ds1302_bitbang_io_output(void) {
	DDR(IO_RTC) |= IO_RTC_IO;
}


static void ds1302_bitbang_write_cycle(uint8_t bus) {
	ds1302_bitbang_io_output();
	
	/* write data in lsb-first order */
	for (uint8_t i = 8; i != 0; --i) {
		if (bus & _BV(0)) {
			PORT(IO_RTC) |= IO_RTC_IO;
		} else {
			PORT(IO_RTC) &= ~IO_RTC_IO;
		}
		DELAY_NSEC(200); // t_DC
		
		PORT(IO_RTC) |= IO_RTC_SCLK;
		DELAY_NSEC(1000); // t_CH t_CDH
		
		PORT(IO_RTC) &= ~IO_RTC_SCLK;
		DELAY_NSEC(1000); // t_CL
		
		bus >>= 1;
	}
	
	ds1302_bitbang_io_input();
}

static uint8_t ds1302_bitbang_read_cycle(void) {
	uint8_t bus = 0;
	
	/* read data in lsb-first order */
	for (uint8_t i = 0; i < 8; ++i) {
		bus >>= 1;
		
		DELAY_NSEC(800); // t_CDD
		if (PIN(IO_RTC) & IO_RTC_IO) {
			bus |= _BV(7);
		}
		
		/* 7 clock cycles for 8 bits */
		if (i != 7) {
			PORT(IO_RTC) |= IO_RTC_SCLK;
			DELAY_NSEC(1000); // t_CH
			
			PORT(IO_RTC) &= ~IO_RTC_SCLK;
			DELAY_NSEC(200); // t_CL (minus t_CDD)
		}
	}
	
	return bus;
}


static void ds1302_bitbang_write(uint8_t addr, uint8_t data) {
	PORT(IO_RTC) |= IO_RTC_CE;
	_delay_us(4); // t_CC
	
	uint8_t cmd = ((addr & 0x2f) << 1) | DS1302_CBIT_ONE | DS1302_CBIT_WR;
	ds1302_bitbang_write_cycle(cmd);
	ds1302_bitbang_write_cycle(data);
	
	PORT(IO_RTC) &= ~IO_RTC_CE;
	_delay_us(4); // t_CWH
}

static uint8_t ds1302_bitbang_read(uint8_t addr) {
	PORT(IO_RTC) |= IO_RTC_CE;
	_delay_us(4); // t_CC
	
	uint8_t cmd = ((addr & 0x2f) << 1) | DS1302_CBIT_ONE | DS1302_CBIT_RD;
	printf_P(PSTR("ds1302_bitbang_read: cmd 0x%02x\n"), cmd);
	ds1302_bitbang_write_cycle(cmd);
	
	return ds1302_bitbang_read_cycle();
}


void ds1302_init(void) {
	ds1302_bitbang_init();
}


void ds1302_ram_write(uint8_t addr, uint8_t data) {
	ds1302_bitbang_write(addr | DS1302_ABIT_RAM, data);
}

void ds1302_ck_write(uint8_t addr, uint8_t data) {
	ds1302_bitbang_write(addr | DS1302_ABIT_CK, data);
}

uint8_t ds1302_ram_read(uint8_t addr) {
	return ds1302_bitbang_read(addr | DS1302_ABIT_RAM);
}

uint8_t ds1302_ck_read(uint8_t addr) {
	return ds1302_bitbang_read(addr | DS1302_ABIT_CK);
}


#warning TODO: burst mode
