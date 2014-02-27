/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/spi.h"


void lcd_spi_init(void) {
	/* invert clock (default high) */
	PORTC.PIN1CTRL |= PORT_INVEN_bm;
	
	/* set TX high, XCK low, and both as output */
	PORTC.OUTSET = 0b00001000;
	PORTC.OUTCLR = 0b00000010;
	PORTC.DIRSET = 0b00001010;
	
	/* use max baud rate: 2 Mbps */
	uint16_t bsel   = 0;
	uint8_t  bscale = 0;
	USARTF0.BAUDCTRLA = bsel & 0xff;
	USARTF0.BAUDCTRLB = ((bsel >> 8) & 0x0f) | (bscale & 0x0f);
	
	/* master SPI mode, sample on trailing clock edge */
	USARTC0.CTRLC = USART_CMODE_MSPI_gc | USART_CHSIZE1_bm;
	/* enable transmission only */
	USARTC0.CTRLB = USART_TXEN_bm;
	/* no interrupts */
	USARTC0.CTRLA = USART_RXCINTLVL_OFF_gc | USART_TXCINTLVL_OFF_gc |
		USART_DREINTLVL_OFF_gc;
}


void lcd_spi_write(uint8_t byte) {
	while (!(USARTC0.STATUS & USART_DREIF_bm));
	
	USARTC0.DATA = byte;
}

void lcd_spi_write_many(uint16_t len, const uint8_t bytes[static len]) {
	const uint8_t *byte = bytes;
	while (len-- != 0) {
		lcd_spi_write(*(byte++));
	}
}

void lcd_spi_write_many_P(uint16_t len, const uint8_t *bytes) {
	while (len-- != 0) {
		lcd_spi_write(pgm_read_byte(bytes++));
	}
}
