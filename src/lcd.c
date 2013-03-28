/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "lcd.h"


static void _lcd_bus_input(void) {
	io_write(DDR(IO_LCD_BUS), IO_LCD_BUS_ALL, 0);
	io_write(PORT(IO_LCD_BUS), IO_LCD_BUS_ALL, IO_LCD_BUS_ALL);
}

static void _lcd_bus_output(void) {
	io_write(DDR(IO_LCD_BUS), IO_LCD_BUS_ALL, IO_LCD_BUS_ALL);
}


static void _lcd_write_cycle(uint8_t rs, uint8_t bus) {
	io_write(PORT(IO_LCD_BUS), IO_LCD_BUS_ALL, bus);
	
	io_write(PORT(IO_LCD_CTRL), IO_LCD_CTRL_ALL, rs | IO_LCD_CTRL_E);
	delay_150ns();
	
	io_write(PORT(IO_LCD_CTRL), IO_LCD_CTRL_E, 0);
	delay_1200ns();
}

static uint8_t _lcd_read_cycle(uint8_t rs) {
	_lcd_bus_input();
	
	io_write(PORT(IO_LCD_CTRL), IO_LCD_CTRL_ALL,
		rs | IO_LCD_CTRL_RW | IO_LCD_CTRL_E);
	delay_150ns();
	
	uint8_t bus = io_read(PIN(IO_LCD_BUS), IO_LCD_BUS_ALL);
	
	io_write(PORT(IO_LCD_CTRL), IO_LCD_CTRL_E, 0);
	delay_1200ns();
	
	_lcd_bus_output();
	
	return bus;
}


static void _lcd_write_instr(uint8_t instr) {
	_lcd_write_cycle(0, instr);
}

static void _lcd_write_data(uint8_t data) {
	_lcd_write_cycle(IO_LCD_CTRL_RS, data);
}


static uint8_t lcd_read_addr(void) {
	uint8_t addr = _lcd_read_cycle(0);
	
	_delay_us(4);
	return addr;
}

static uint8_t lcd_read_data(void) {
	uint8_t data = _lcd_read_cycle(IO_LCD_CTRL_RS);
	
	_delay_us(4);
	return data;
}


static void lcd_busy_wait(void) {
	while ((lcd_read_addr() & LCD_BUS_BF) != 0);
}


static void lcd_instr_clear(void) {
	lcd_busy_wait();
	_lcd_write_instr(_BV(LCD_INSTR_CLEAR));
}

static void lcd_instr_cur_home(void) {
	lcd_busy_wait();
	_lcd_write_instr(_BV(LCD_INSTR_CUR_HOME));
}

static void lcd_instr_ent_mode(uint8_t param) {
	lcd_busy_wait();
	_lcd_write_instr(_BV(LCD_INSTR_ENT_MODE) | (param & 0x03));
}

static void lcd_instr_onoff(uint8_t param) {
	lcd_busy_wait();
	_lcd_write_instr(_BV(LCD_INSTR_ONOFF) | (param & 0x07));
}

static void lcd_instr_shift(uint8_t param) {
	lcd_busy_wait();
	_lcd_write_instr(_BV(LCD_INSTR_SHIFT) | (param & 0x0a));
}

static void lcd_instr_func_set(uint8_t param) {
	lcd_busy_wait();
	_lcd_write_instr(_BV(LCD_INSTR_FUNC_SET) | (param & 0x1a));
}

static void lcd_instr_cgram_addr(uint8_t addr) {
	lcd_busy_wait();
	_lcd_write_instr(_BV(LCD_INSTR_CGRAM_ADDR) | (addr & 0x3f));
}

static void lcd_instr_ddram_addr(uint8_t addr) {
	lcd_busy_wait();
	_lcd_write_instr(_BV(LCD_INSTR_DDRAM_ADDR) | (addr & 0x7f));
}

static void lcd_write_data(uint8_t data) {
	lcd_busy_wait();
	_lcd_write_data(data);
}


void lcd_init(void) {
	io_write(DDR(IO_LCD_CTRL), IO_LCD_CTRL_ALL, IO_LCD_CTRL_ALL);
	io_write(DDR(IO_LCD_BUS), IO_LCD_BUS_ALL, IO_LCD_BUS_ALL);
	
	_delay_ms(40);
	_lcd_write_instr(_BV(LCD_INSTR_FUNC_SET) | LCD_BUS_DL | LCD_BUS_N);
	
	_delay_ms(5);
	_lcd_write_instr(_BV(LCD_INSTR_FUNC_SET) | LCD_BUS_DL | LCD_BUS_N);
	
	_delay_us(100);
	_lcd_write_instr(_BV(LCD_INSTR_FUNC_SET) | LCD_BUS_DL | LCD_BUS_N);
	
	lcd_instr_func_set(LCD_BUS_DL | LCD_BUS_N);
	lcd_instr_clear();
	lcd_instr_onoff(LCD_BUS_D);
	lcd_instr_ent_mode(LCD_BUS_ID);
}


void lcd_put(char chr) {
	lcd_write_data(chr);
	
	// handle wrapping
}
