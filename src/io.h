/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_IO_H
#define AUTOMATON_IO_H


#include "std.h"


typedef volatile uint8_t *reg8_t;
typedef volatile uint16_t *reg16_t;


static inline uint8_t _io_read(reg8_t reg, uint8_t mask) {
	return *reg & mask;
}

static inline uint16_t _io_read16(reg16_t reg, uint16_t mask) {
	return *reg & mask;
}

static inline void _io_write(reg8_t reg, uint8_t mask, uint8_t val) {
	*reg = (*reg & ~mask) | (val & mask);
}

static inline void _io_write16(reg16_t reg, uint16_t mask, uint16_t val) {
	*reg = (*reg & ~mask) | (val & mask);
}


#define io_read(_reg, _mask) \
	_io_read(&_reg, _mask)
#define io_read16(_reg, _mask) \
	_io_read16(&_reg, _mask)
#define io_write(_reg, _mask, _val) \
	_io_write(&_reg, _mask, _val)
#define io_write16(_reg, _mask, _val) \
	_io_write16(&_reg, _mask, _val)


#endif
