/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/lcd/spi.h"


void lcd_spi_init(void) {
	/* no interrupts */
	USARTC0.CTRLA = USART_RXCINTLVL_OFF_gc | USART_TXCINTLVL_OFF_gc |
		USART_DREINTLVL_OFF_gc;
	/* enable transmission only */
	USARTC0.CTRLB = USART_TXEN_bm;
	/* master SPI mode, sample on trailing clock edge */
	USARTC0.CTRLC = USART_CMODE_MSPI_gc | USART_CHSIZE1_bm;
	
	/* invert clock (default high) */
	PORTC.PIN1CTRL |= PORT_INVEN_bm;
	
	/* max baud rate (2 Mbps) */
	USARTC0.BAUDCTRLA = 0;
	USARTC0.BAUDCTRLB = 0;
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


// TODO: functions for **fast** DMA block writes, from RAM or flash
