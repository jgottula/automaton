/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEBUG_STDFILE_H
#define AUTOMATON_DEBUG_STDFILE_H


#include "std.h"


extern FILE *lcd;
extern FILE *stn1110;


void stdfile_open(void);
void stdfile_close(void);


#endif
