/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "ui/time.h"
#include "button/button.h"
#include "lcd/lcd.h"
#include "time/clock.h"
#include "ui/ui.h"


enum time_field {
	TIME_FIELD_NONE = 0,
	
	TIME_FIELD_YEAR  = 1,
	TIME_FIELD_MONTH = 2,
	TIME_FIELD_DAY   = 3,
	
	TIME_FIELD_HOUR = 4,
	TIME_FIELD_MIN  = 5,
	TIME_FIELD_SEC  = 6,
	
	TIME_FIELD_LAST = TIME_FIELD_SEC,
};


static struct {
	time_t time;
	struct tm tm;
	
	uint8_t field;
} state = {
	.time  = 0,
	
	.field = TIME_FIELD_NONE,
};


static const uint8_t field_pos[] PROGMEM = {
	/* row | col */
	(1 << 4) | (6),
	(1 << 4) | (9),
	(1 << 4) | (12),
	(3 << 4) | (6),
	(3 << 4) | (9),
	(3 << 4) | (12),
};


static void _ui_page_time_draw_time(void) {
	lcd_goto(0, 3);
	fprintf_P(lcd, PSTR("%u/%02u/%02u"),
		state.tm.tm_year + 1900, state.tm.tm_mon + 1, state.tm.tm_mday);
	
	lcd_goto(2, 5);
	fprintf_P(lcd, PSTR("%02u:%02u:%02u"),
		state.tm.tm_hour, state.tm.tm_min, state.tm.tm_sec);
}

static void _ui_page_time_change_field(uint8_t new_field) {
	if (state.field != TIME_FIELD_NONE) {
		uint8_t pos = pgm_read_byte(field_pos + (state.field - 1));
		lcd_goto((pos >> 4), (pos & 0x0f));
		fputc(' ', lcd);
	}
	
	if (new_field != TIME_FIELD_NONE) {
		uint8_t pos = pgm_read_byte(field_pos + (new_field - 1));
		lcd_goto((pos >> 4), (pos & 0x0f));
		fputc('^', lcd);
	}
	
	state.field = new_field;
}


static void _ui_page_time_check_time(void) {
	time_t now = time(NULL);
	if (state.time != now) {
		state.time = now;
		gmtime_r(&state.time, &state.tm);
		
		_ui_page_time_draw_time();
	}
}

static void _ui_page_time_set_time(void) {
	state.time = mktime(&state.tm);
	set_system_time(state.time);
	clock_store();
}


static void _ui_page_time_adjust_up(void) {
	if (state.field == TIME_FIELD_YEAR) {
		if (++state.tm.tm_year > 199) {
			state.tm.tm_year = 100;
		}
	} else if (state.field == TIME_FIELD_MONTH) {
		if (++state.tm.tm_mon > 11) {
			state.tm.tm_mon = 0;
		}
	} else if (state.field == TIME_FIELD_DAY) {
		if (++state.tm.tm_mday > 31) {
			state.tm.tm_mday = 0;
		}
	} else if (state.field == TIME_FIELD_HOUR) {
		if (++state.tm.tm_hour > 23) {
			state.tm.tm_hour = 0;
		}
	} else if (state.field == TIME_FIELD_MIN) {
		if (++state.tm.tm_min > 59) {
			state.tm.tm_min = 0;
		}
	} else if (state.field == TIME_FIELD_SEC) {
		state.tm.tm_sec = 0;
	}
	
	_ui_page_time_draw_time();
}

static void _ui_page_time_adjust_down(void) {
	if (state.field == TIME_FIELD_YEAR) {
		if (--state.tm.tm_year < 100) {
			state.tm.tm_year = 199;
		}
	} else if (state.field == TIME_FIELD_MONTH) {
		if (--state.tm.tm_mon < 0) {
			state.tm.tm_mon = 11;
		}
	} else if (state.field == TIME_FIELD_DAY) {
		if (--state.tm.tm_mday < 0) {
			state.tm.tm_mday = 31;
		}
	} else if (state.field == TIME_FIELD_HOUR) {
		if (--state.tm.tm_hour < 0) {
			state.tm.tm_hour = 23;
		}
	} else if (state.field == TIME_FIELD_MIN) {
		if (--state.tm.tm_min < 0) {
			state.tm.tm_min = 59;
		}
	} else if (state.field == TIME_FIELD_SEC) {
		state.tm.tm_sec = 0;
	}
	
	_ui_page_time_draw_time();
}


void ui_page_time_init(void) {
	ui_header(PSTR("TIME"));
	
	_ui_page_time_check_time();
	_ui_page_time_change_field(TIME_FIELD_NONE);
}

bool ui_page_time_update(void) {
	_ui_page_time_check_time();
	
	struct button_event event;
	while (button_pump(&event)) {
		if (event.num == 3 && !event.down) {
			return false;
		} else if (event.num == 2 && !event.down) {
			if (state.field != TIME_FIELD_LAST) {
				_ui_page_time_change_field(state.field + 1);
			} else {
				_ui_page_time_change_field(0);
			}
		}
		
		if (state.field != TIME_FIELD_NONE) {
			if (event.num == 1 && event.down) {
				_ui_page_time_adjust_down();
			} else if (event.num == 0 && event.down) {
				_ui_page_time_adjust_up();
			} else if ((event.num == 1 || event.num == 0) && !event.down) {
				_ui_page_time_set_time();
			}
		}
	}
	
	return true;
}
