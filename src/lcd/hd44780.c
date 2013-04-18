/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "lcd/hd44780.h"
#include "time/delay.h"


/* private: allow the address counter time to increment (beyond busy flag) */
static void hd44780_delay_for_addr_incr(void) {
	_delay_us(4);
}


/* private: set the lcd bus to input mode with pull-ups */
static void hd44780_bus_mode_input(void) {
	io_write(DDR(IO_LCD_BUS), IO_LCD_BUS_ALL, 0);
	io_write(PORT(IO_LCD_BUS), IO_LCD_BUS_ALL, IO_LCD_BUS_ALL);
}

/* private: set the lcd bus to output mode */
static void hd44780_bus_mode_output(void) {
	io_write(DDR(IO_LCD_BUS), IO_LCD_BUS_ALL, IO_LCD_BUS_ALL);
}


/* private: put a byte/nibble on the bus and cycle E */
static void hd44780_raw_write_subcycle(uint8_t bus) {
	io_write(PORT(IO_LCD_BUS), IO_LCD_BUS_ALL, bus);
	
	PORT(IO_LCD_CTRL) |= IO_LCD_CTRL_E;
	delay_150ns();
	
	PORT(IO_LCD_CTRL) &= ~IO_LCD_CTRL_E;
	delay_1200ns();
}

/* private: cycle E and get a byte from the bus */
static uint8_t hd44780_raw_read_subcycle(void) {
	PORT(IO_LCD_CTRL) |= IO_LCD_CTRL_E;
	delay_150ns();
	
	uint8_t bus = io_read(PIN(IO_LCD_BUS), IO_LCD_BUS_ALL);
	
	PORT(IO_LCD_CTRL) &= ~IO_LCD_CTRL_E;
	delay_1200ns();
	
	return bus;
}


/* private: set/clear RS, clear RW, and perform a read cycle */
static void hd44780_raw_write_cycle(uint8_t rs, uint8_t bus) {
	io_write(PORT(IO_LCD_CTRL), IO_LCD_CTRL_RS | IO_LCD_CTRL_RW, rs);
	
#if HD44780_BUS_WIDTH == 8
	hd44780_raw_write_subcycle(bus);
#elif HD44780_BUS_WIDTH == 4
	hd44780_raw_write_subcycle(bus >> 4);
	hd44780_raw_write_subcycle(bus);
#endif
}

/* private: set/clear RS, set RW, and perform a write cycle */
static uint8_t hd44780_raw_read_cycle(uint8_t rs) {
	hd44780_bus_mode_input();
	io_write(PORT(IO_LCD_CTRL), IO_LCD_CTRL_RS | IO_LCD_CTRL_RW,
		rs | IO_LCD_CTRL_RW);
	
#if HD44780_BUS_WIDTH == 8
	uint8_t bus = hd44780_raw_read_subcycle();
#elif HD44780_BUS_WIDTH == 4
	uint8_t bus = hd44780_raw_read_subcycle() << 4;
	bus |= hd44780_raw_read_subcycle();
#endif
	
	hd44780_bus_mode_output();
	return bus;
}


/* private: perform a write cycle with RS unset (instruction) */
static void hd44780_raw_write_instr(uint8_t instr) {
	hd44780_raw_write_cycle(0, instr);
}

/* private: perform a write cycle with RS set (data) */
static void hd44780_raw_write_data(uint8_t data) {
	hd44780_raw_write_cycle(IO_LCD_CTRL_RS, data);
}

/* private: perform a read cycle with RS unset (address) */
static uint8_t hd44780_raw_read_addr(void) {
	return hd44780_raw_read_cycle(0);
}

/* private: perform a read cycle with RS set (data) */
static uint8_t hd44780_raw_read_data(void) {
	return hd44780_raw_read_cycle(IO_LCD_CTRL_RS);
}


/* private: continuously poll the busy flag until it clears */
static void hd44780_busy_wait(void) {
	while ((hd44780_raw_read_addr() & HD44780_IBIT_BF) != 0);
}


/* private: write multiple data */
static void hd44780_write_multi(uint8_t len, const uint8_t data[static len]) {
	/* we assume that the address counter is set to increment */
	for (uint8_t i = 0; i < len; ++i) {
		hd44780_busy_wait();
		hd44780_delay_for_addr_incr();
		
		hd44780_raw_write_data(data[i]);
	}
}

/* private: read multiple data (explicitly set addr immediately before this!) */
static void hd44780_read_multi(uint8_t len, uint8_t data[static len]) {
	/* we assume that the address counter is set to increment */
	for (uint8_t i = 0; i < len; ++i) {
		hd44780_busy_wait();
		hd44780_delay_for_addr_incr();
		
		data[i] = hd44780_raw_read_data();
	}
}


/* private: wait, then execute the clear instruction */
static void hd44780_instr_clear(void) {
	hd44780_busy_wait();
	hd44780_raw_write_instr(_BV(HD44780_INSTR_CLEAR));
}

/* private: wait, then execute the cursor home instruction */
static void hd44780_instr_home(void) {
	hd44780_busy_wait();
	hd44780_raw_write_instr(_BV(HD44780_INSTR_HOME));
}

/* private: wait, then execute the entry mode instruction */
static void hd44780_instr_ent_mode(uint8_t param) {
	hd44780_busy_wait();
	hd44780_raw_write_instr(_BV(HD44780_INSTR_ENT_MODE) | (param & 0x03));
}

/* private: wait, then execute the on/off instruction */
static void hd44780_instr_onoff(uint8_t param) {
	hd44780_busy_wait();
	hd44780_raw_write_instr(_BV(HD44780_INSTR_ONOFF) | (param & 0x07));
}

/* private: wait, then execute the shift instruction */
static void hd44780_instr_shift(uint8_t param) {
	hd44780_busy_wait();
	hd44780_raw_write_instr(_BV(HD44780_INSTR_SHIFT) | (param & 0x0a));
}

/* private: wait, then execute the function set instruction */
static void hd44780_instr_func_set(uint8_t param) {
	hd44780_busy_wait();
	hd44780_raw_write_instr(_BV(HD44780_INSTR_FUNC_SET) | (param & 0x1a));
}

/* private: wait, then execute the set cgram address instruction */
static void hd44780_instr_cgram_addr(uint8_t addr) {
	hd44780_busy_wait();
	hd44780_raw_write_instr(_BV(HD44780_INSTR_CGRAM_ADDR) | (addr & 0x3f));
}

/* private: wait, then execute the set ddram address instruction */
static void hd44780_instr_ddram_addr(uint8_t addr) {
	hd44780_busy_wait();
	hd44780_raw_write_instr(_BV(HD44780_INSTR_DDRAM_ADDR) | (addr & 0x7f));
}


/* public: perform the initialization sequence specified in the datasheet */
void hd44780_init(uint8_t func_set, uint8_t onoff, uint8_t ent_mode) {
	io_write(DDR(IO_LCD_CTRL), IO_LCD_CTRL_ALL, IO_LCD_CTRL_ALL);
	io_write(DDR(IO_LCD_BUS), IO_LCD_BUS_ALL, IO_LCD_BUS_ALL);
	
	_delay_ms(40);
	hd44780_raw_write_instr(_BV(HD44780_INSTR_FUNC_SET) | func_set);
	
	_delay_ms(5);
	hd44780_raw_write_instr(_BV(HD44780_INSTR_FUNC_SET) | func_set);
	
	_delay_us(100);
	hd44780_raw_write_instr(_BV(HD44780_INSTR_FUNC_SET) | func_set);
	
	/* these instructions, unlike those above, will wait on the busy flag */
	hd44780_instr_func_set(func_set);
	hd44780_instr_clear();
	hd44780_instr_onoff(onoff);
	hd44780_instr_ent_mode(ent_mode);
}


/* public: read data from cgram */
void hd44780_read_cgram(uint8_t addr, uint8_t len, uint8_t data[static len]) {
	/* datasheet: must explicitly set address before data reads */
	hd44780_instr_cgram_addr(addr);
	
	hd44780_read_multi(len, data);
}

/* public: read data from ddram */
void hd44780_read_ddram(uint8_t addr, uint8_t len, uint8_t data[static len]) {
	/* datasheet: must explicitly set address before data reads */
	hd44780_instr_ddram_addr(addr);
	
	hd44780_read_multi(len, data);
}

/* public: write data to cgram or ddram */
void hd44780_write(uint8_t len, const uint8_t data[static len]) {
	hd44780_write_multi(len, data);
}


/* public: get the current address */
uint8_t hd44780_get_addr(void) {
	return (hd44780_raw_read_addr() & ~HD44780_IBIT_BF);
}

/* public: set the address (within cgram) */
void hd44780_set_cgaddr(uint8_t addr) {
	hd44780_instr_cgram_addr(addr);
}

/* public: set the address (within ddram) */
void hd44780_set_ddaddr(uint8_t addr) {
	hd44780_instr_ddram_addr(addr);
}


/* public: clear the entire display and set the address to ddram:0 */
void hd44780_clear(void) {
	hd44780_instr_clear();
}

/* public: set the address to ddram:0 and reset display shift */
void hd44780_home(void) {
	hd44780_instr_home();
}

/* public: set cursor move direction and display shift */
void hd44780_ent_mode(uint8_t param) {
	hd44780_instr_ent_mode(param);
}

/* public: control display power status, cursor visibility, and cursor blink */
void hd44780_onoff(uint8_t param) {
	hd44780_instr_onoff(param);
}

/* public: move cursor or shift display */
void hd44780_shift(uint8_t param) {
	hd44780_instr_shift(param);
}

/* public: set bus width, number of display lines, and font */
void hd44780_func_set(uint8_t param) {
	hd44780_instr_func_set(param);
}
