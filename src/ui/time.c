/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "ui/time.h"
#include "ui/ui.h"


static void _ui_page_time_init(void) {
	
}


void ui_page_time(void) {
	_ui_page_time_init();
	
	fputs_P(PSTR("time page"), lcd);
	for ( ; ; );
}
