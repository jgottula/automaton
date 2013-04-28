/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "ui/time.h"
#include "lcd/lcd.h"
#include "ui/ui.h"


static void _ui_page_time_init(void) {
	ui_header(PSTR("TIME"));
}


void ui_page_time(void) {
	_ui_page_time_init();
	
	lcd_goto(0, 2);
	fputs_P(PSTR("time page"), lcd);
	
	for ( ; ; );
}
