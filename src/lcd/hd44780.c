/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "lcd/hd44780.h"
#include "time/delay.h"


/* private: allow the address counter time to increment (beyond busy flag) */
static void _hd44780_addr_incr_delay(void) {
	_delay_us(4); // t_ADD
}


/* private: set the lcd bus to input mode with pull-ups */
static void _hd44780_mode_input(void) {
	IO_WRITE(PORT(IO_LCD_BUS), IO_LCD_BUS_ALL, IO_LCD_BUS_ALL);
	IO_WRITE(DDR(IO_LCD_BUS), IO_LCD_BUS_ALL, 0);
}

/* private: set the lcd bus to output mode */
static void _hd44780_mode_output(void) {
	IO_WRITE(DDR(IO_LCD_BUS), IO_LCD_BUS_ALL, IO_LCD_BUS_ALL);
}


/* private: put a byte/nibble on the bus and cycle E */
static void _hd44780_write_subcycle(uint8_t bus) {
	IO_WRITE(PORT(IO_LCD_BUS), IO_LCD_BUS_ALL, bus);
	
	PORT(IO_LCD_CTRL) |= IO_LCD_CTRL_E;
	DELAY_NSEC(450); // PW_EH t_DSW
	
	PORT(IO_LCD_CTRL) &= ~IO_LCD_CTRL_E;
	DELAY_NSEC(550); // t_cycE
}

/* private: cycle E and get a byte from the bus */
static uint8_t _hd44780_read_subcycle(void) {
	PORT(IO_LCD_CTRL) |= IO_LCD_CTRL_E;
	DELAY_NSEC(450); // PW_EH t_DDR
	
	uint8_t bus = IO_READ(PIN(IO_LCD_BUS), IO_LCD_BUS_ALL);
	
	PORT(IO_LCD_CTRL) &= ~IO_LCD_CTRL_E;
	DELAY_NSEC(550); // t_cycE
	
	return bus;
}


/* private: set/clear RS, clear RW, and perform a read cycle */
static void _hd44780_write_cycle(uint8_t rs, uint8_t bus) {
	IO_WRITE(PORT(IO_LCD_CTRL), IO_LCD_CTRL_RS | IO_LCD_CTRL_RW, rs);
	DELAY_NSEC(60); // t_AS
	
#if HD44780_BUS_WIDTH == 8
	_hd44780_write_subcycle(bus);
#elif HD44780_BUS_WIDTH == 4
	_hd44780_write_subcycle(bus >> 4);
	_hd44780_write_subcycle(bus);
#endif
}

/* private: set/clear RS, set RW, and perform a write cycle */
static uint8_t _hd44780_read_cycle(uint8_t rs) {
	_hd44780_mode_input();
	IO_WRITE(PORT(IO_LCD_CTRL), IO_LCD_CTRL_RS | IO_LCD_CTRL_RW,
		rs | IO_LCD_CTRL_RW);
	DELAY_NSEC(60); // t_AS
	
#if HD44780_BUS_WIDTH == 8
	uint8_t bus = _hd44780_read_subcycle();
#elif HD44780_BUS_WIDTH == 4
	uint8_t bus = _hd44780_read_subcycle() << 4;
	bus |= _hd44780_read_subcycle();
#endif
	
	_hd44780_mode_output();
	return bus;
}


/* private: perform a write cycle with RS unset (instruction) */
static void _hd44780_write_instr(uint8_t instr) {
	_hd44780_write_cycle(0, instr);
}

/* private: perform a write cycle with RS set (data) */
static void _hd44780_write_data(uint8_t data) {
	_hd44780_write_cycle(IO_LCD_CTRL_RS, data);
}

/* private: perform a read cycle with RS unset (address) */
static uint8_t _hd44780_read_addr(void) {
	return _hd44780_read_cycle(0);
}

/* private: perform a read cycle with RS set (data) */
static uint8_t _hd44780_read_data(void) {
	return _hd44780_read_cycle(IO_LCD_CTRL_RS);
}


/* private: continuously poll the busy flag until it clears */
static void _hd44780_busy_wait(void) {
	while ((_hd44780_read_addr() & HD44780_IBIT_BF) != 0);
}


/* private: write multiple data */
static void _hd44780_write_multi(uint8_t len, const uint8_t data[static len]) {
	/* we assume that the address counter is set to increment */
	do {
		_hd44780_busy_wait();
		_hd44780_addr_incr_delay();
		
		_hd44780_write_data(*(data++));
	} while (--len != 0);
}

/* private: read multiple data (explicitly set addr immediately before this!) */
static void _hd44780_read_multi(uint8_t len, uint8_t data[static len]) {
	/* we assume that the address counter is set to increment */
	do {
		_hd44780_busy_wait();
		_hd44780_addr_incr_delay();
		
		*(data++) = _hd44780_read_data();
	} while (--len != 0);
}


/* private: wait, then execute the clear instruction */
static void _hd44780_instr_clear(void) {
	_hd44780_busy_wait();
	_hd44780_write_instr(_BV(HD44780_INSTR_CLEAR));
}

/* private: wait, then execute the cursor home instruction */
static void _hd44780_instr_home(void) {
	_hd44780_busy_wait();
	_hd44780_write_instr(_BV(HD44780_INSTR_HOME));
}

/* private: wait, then execute the entry mode instruction */
static void _hd44780_instr_ent_mode(uint8_t param) {
	_hd44780_busy_wait();
	_hd44780_write_instr(_BV(HD44780_INSTR_ENT_MODE) | (param & 0x03));
}

/* private: wait, then execute the on/off instruction */
static void _hd44780_instr_onoff(uint8_t param) {
	_hd44780_busy_wait();
	_hd44780_write_instr(_BV(HD44780_INSTR_ONOFF) | (param & 0x07));
}

/* private: wait, then execute the shift instruction */
static void _hd44780_instr_shift(uint8_t param) {
	_hd44780_busy_wait();
	_hd44780_write_instr(_BV(HD44780_INSTR_SHIFT) | (param & 0x0a));
}

/* private: wait, then execute the function set instruction */
static void _hd44780_instr_func_set(uint8_t param) {
	_hd44780_busy_wait();
	_hd44780_write_instr(_BV(HD44780_INSTR_FUNC_SET) | (param & 0x1a));
}

/* private: wait, then execute the set cgram address instruction */
static void _hd44780_instr_cgram_addr(uint8_t addr) {
	_hd44780_busy_wait();
	_hd44780_write_instr(_BV(HD44780_INSTR_CGRAM_ADDR) | (addr & 0x3f));
}

/* private: wait, then execute the set ddram address instruction */
static void _hd44780_instr_ddram_addr(uint8_t addr) {
	_hd44780_busy_wait();
	_hd44780_write_instr(_BV(HD44780_INSTR_DDRAM_ADDR) | (addr & 0x7f));
}


/* public: perform the initialization sequence specified in the datasheet */
void hd44780_init(uint8_t func_set, uint8_t onoff, uint8_t ent_mode) {
	IO_WRITE(DDR(IO_LCD_CTRL), IO_LCD_CTRL_ALL, IO_LCD_CTRL_ALL);
	IO_WRITE(DDR(IO_LCD_BUS), IO_LCD_BUS_ALL, IO_LCD_BUS_ALL);
	
	_delay_ms(40);
	_hd44780_write_instr(_BV(HD44780_INSTR_FUNC_SET) | func_set);
	
	_delay_ms(5);
	_hd44780_write_instr(_BV(HD44780_INSTR_FUNC_SET) | func_set);
	
	_delay_us(100);
	_hd44780_write_instr(_BV(HD44780_INSTR_FUNC_SET) | func_set);
	
	/* these instructions, unlike those above, will wait on the busy flag */
	_hd44780_instr_func_set(func_set);
	_hd44780_instr_clear();
	_hd44780_instr_onoff(onoff);
	_hd44780_instr_ent_mode(ent_mode);
}


/* public: read data from cgram */
void hd44780_read_cgram(uint8_t addr, uint8_t len, uint8_t data[static len]) {
	/* datasheet: must explicitly set address before data reads */
	_hd44780_instr_cgram_addr(addr);
	
	_hd44780_read_multi(len, data);
}

/* public: read data from ddram */
void hd44780_read_ddram(uint8_t addr, uint8_t len, uint8_t data[static len]) {
	/* datasheet: must explicitly set address before data reads */
	_hd44780_instr_ddram_addr(addr);
	
	_hd44780_read_multi(len, data);
}

/* public: write data to cgram or ddram */
void hd44780_write(uint8_t len, const uint8_t data[static len]) {
	_hd44780_write_multi(len, data);
}


/* public: get the current address */
uint8_t hd44780_get_addr(void) {
	return (_hd44780_read_addr() & ~HD44780_IBIT_BF);
}

/* public: set the address (within cgram) */
void hd44780_set_cgaddr(uint8_t addr) {
	_hd44780_instr_cgram_addr(addr);
}

/* public: set the address (within ddram) */
void hd44780_set_ddaddr(uint8_t addr) {
	_hd44780_instr_ddram_addr(addr);
}


/* public: clear the entire display and set the address to ddram:0 */
void hd44780_clear(void) {
	_hd44780_instr_clear();
}

/* public: set the address to ddram:0 and reset display shift */
void hd44780_home(void) {
	_hd44780_instr_home();
}

/* public: set cursor move direction and display shift */
void hd44780_ent_mode(uint8_t param) {
	_hd44780_instr_ent_mode(param);
}

/* public: control display power status, cursor visibility, and cursor blink */
void hd44780_onoff(uint8_t param) {
	_hd44780_instr_onoff(param);
}

/* public: move cursor or shift display */
void hd44780_shift(uint8_t param) {
	_hd44780_instr_shift(param);
}

/* public: set bus width, number of display lines, and font */
void hd44780_func_set(uint8_t param) {
	_hd44780_instr_func_set(param);
}
