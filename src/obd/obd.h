/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_OBD_OBD_H
#define AUTOMATON_OBD_OBD_H


#include "std.h"


void obd_test(void);

bool obd_check_pid_support(uint8_t pid);

uint8_t obd_query(uint8_t pid, uint8_t data[static 5]);


#endif
