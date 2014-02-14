/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/st7565.h"
#include "dev/lcd/spi.h"


void st7565_init(void) {
	PORTC.DIRSET = 0b00001111;
	
	/* must do this before LCD reset for some reason */
	lcd_spi_init();
	
	/* cycle LCD_RESET low for 1us */
	PORTC.OUTCLR = 0b00000001;
	_delay_us(1);
	PORTC.OUTSET = 0b00000001;
	_delay_us(1);
	
	/* set LCD bias to 1/7 duty cycle */
	st7565_cmd(ST_CMD_LCD_BIAS | 0b1);
	/* set ADC direction to normal */
	st7565_cmd(ST_CMD_ADC | 0b0);
	/* set COM output direction to reversed */
	st7565_cmd(ST_CMD_COM | (0b1 << 3));
	
	/* turn on voltage converter and wait */
	st7565_cmd(ST_CMD_PWR_CTRL | 0b100);
	_delay_ms(1);
	/* turn on voltage regulator and wait */
	st7565_cmd(ST_CMD_PWR_CTRL | 0b110);
	_delay_ms(1);
	/* turn on voltage follower and wait */
	st7565_cmd(ST_CMD_PWR_CTRL | 0b111);
	_delay_ms(1);
	
	st7565_cmd(ST_CMD_START_LINE | ST7565_START_LINE);
	st7565_cmd(ST_CMD_PAGE_ADDR | 0b0000);
	st7565_cmd(ST_CMD_COL_ADDR_HI | 0b0000);
	st7565_cmd(ST_CMD_COL_ADDR_LO | 0b0000);
}


void st7565_cmd(uint8_t cmd) {
	/* set A0 low for commands */
	PORTC.OUTCLR = 0b00000100;
	
	lcd_spi_write(cmd);
	
	/* prevent back-to-back commands from failing */
	_NOP();
	_NOP();
	_NOP();
	_NOP();
}

void st7565_data(uint8_t data) {
	/* set A0 high for data */
	PORTC.OUTSET = 0b00000100;
	
	lcd_spi_write(data);
}
