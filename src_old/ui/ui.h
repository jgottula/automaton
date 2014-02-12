/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_UI_UI_H
#define AUTOMATON_UI_UI_H


#include "std.h"
#include "time/alarm.h"


typedef void (*ui_page_init_fn_t)(void);
typedef void (*ui_page_cleanup_fn_t)(void);
typedef bool (*ui_page_update_fn_t)(void);


enum ui_page {
	UI_PAGE_SLEEP = 0,
	UI_PAGE_TIME  = 1,
	UI_PAGE_RAW   = 2,
	
	UI_PAGE_END,
	
	UI_PAGE_DEFAULT = UI_PAGE_TIME,
};


struct ui_state {
	uint8_t page;
	
	struct alarm alarm;
};


extern struct ui_state ui;


void ui_loop(void);

void ui_header(const char *pstr);


#endif
