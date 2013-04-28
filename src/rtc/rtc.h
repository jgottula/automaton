/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_RTC_RTC_H
#define AUTOMATON_RTC_RTC_H


#include "std.h"


void rtc_init(void);

void rtc_get_tm(struct tm *tm);
void rtc_set_tm(const struct tm *tm);


#endif
