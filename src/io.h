/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_IO_H
#define AUTOMATON_IO_H


#include "std.h"


#define io_read(_reg, _mask) \
	(_reg & _mask)
#define io_write(_reg, _mask, _val) \
	(_reg) = (((_reg) & ~(_mask)) | ((_val) & (_mask)))


#endif
