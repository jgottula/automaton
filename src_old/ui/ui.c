/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "ui/ui.h"
#include "lcd/lcd.h"
#include "ui/raw.h"
#include "ui/sleep.h"
#include "ui/time.h"


struct ui_state ui = {
	.page = UI_PAGE_DEFAULT,
};


static const ui_page_init_fn_t ui_func_init[] PROGMEM = {
	ui_page_sleep_init,
	ui_page_time_init,
	ui_page_raw_init,
};
static const ui_page_cleanup_fn_t ui_func_cleanup[] PROGMEM = {
	ui_page_sleep_cleanup,
	ui_page_time_cleanup,
	ui_page_raw_cleanup,
};
static const ui_page_update_fn_t ui_func_update[] PROGMEM = {
	ui_page_sleep_update,
	ui_page_time_update,
	ui_page_raw_update,
};


static void _ui_call_init(void) {
	ui_page_init_fn_t init_fn = pgm_read_word(ui_func_init + ui.page);
	init_fn();
}

static void _ui_call_cleanup(void) {
	ui_page_cleanup_fn_t cleanup_fn = pgm_read_word(ui_func_cleanup + ui.page);
	cleanup_fn();
}

static bool _ui_call_update(void) {
	ui_page_update_fn_t update_fn = pgm_read_word(ui_func_update + ui.page);
	return update_fn();
}


static void _ui_change_page(uint8_t new_page, bool first_time) {
	if (!first_time) {
		_ui_call_cleanup();
	}
	
	ui.page = new_page;
	
	_ui_call_init();
}


static void _ui_init(void) {
	_ui_change_page(UI_PAGE_DEFAULT, true);
}


void ui_loop(void) {
	_ui_init();
	
	for ( ; ; ) {
		if (!_ui_call_update()) {
			if (ui.page != (UI_PAGE_END - 1)) {
				_ui_change_page(ui.page + 1, false);
			} else {
				_ui_change_page(0, false);
			}
		}
		
		_delay_us(100);
		
		// TODO: go to sleep briefly for power savings
		// (use ui.alarm to wake us up at a well-defined interval)
	}
}


void ui_header(const char *pstr) {
	lcd_clear();
	
	fputs_P(pstr, lcd);
}
