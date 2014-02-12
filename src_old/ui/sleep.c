/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "ui/sleep.h"
#include "button/button.h"
#include "lcd/lcd.h"
#include "ui/ui.h"


void ui_page_sleep_init(void) {
	lcd_clear();
}

void ui_page_sleep_cleanup(void) {
	
}

bool ui_page_sleep_update(void) {
	// TODO: go into low power mode
	// when a button is pressed, leave this mode
	
	struct button_event event;
	while (button_pump(&event)) {
		if (event.num == BTN_PAGE && !event.down) {
			return false;
		}
	}
	
	return true;
}
