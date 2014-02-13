/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/backlight.h"


void lcd_bl_init(void) {
	/* invert pins and set as output */
	PORTE.PIN0CTRL |= PORT_INVEN_bm;
	PORTE.PIN1CTRL |= PORT_INVEN_bm;
	PORTE.PIN2CTRL |= PORT_INVEN_bm;
	PORTE.OUTCLR = 0b00000111;
	PORTE.DIRSET = 0b00000111;
	
	/* clear the counter */
	TCE0.CNT = 0;
	/* set the period to 256 */
	TCE0.PER = 0xff;
	
	/* initial duty cycles: all 0% */
	TCE0.CCA = 0;
	TCE0.CCB = 0;
	TCE0.CCC = 0;
	
	/* tick at 500kHz */
	TCE0.CTRLA = TC_CLKSEL_DIV64_gc;
	/* clear the compare output values */
	TCE0.CTRLC = 0;
	/* no event-related stuff */
	TCE0.CTRLD = 0;
	/* use normal counter byte mode */
	TCE0.CTRLE = TC_BYTEM_NORMAL_gc;
	
	/* no interrupts */
	TCE0.INTCTRLA = TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_OFF_gc;
	/* no interrupts */
	TCE0.INTCTRLB = TC_CCDINTLVL_OFF_gc | TC_CCCINTLVL_OFF_gc |
		TC_CCBINTLVL_OFF_gc | TC_CCAINTLVL_OFF_gc;
	
	/* override PE0, PE1, PE2 with single-slope PWM */
	TCE0.CTRLB = TC0_CCCEN_bm | TC0_CCBEN_bm | TC0_CCAEN_bm |
		TC_WGMODE_SINGLESLOPE_gc;
}


void lcd_bl_rgb(uint8_t red, uint8_t green, uint8_t blue) {
	TCE0.CCA = blue;
	TCE0.CCB = green;
	TCE0.CCC = red;
}
