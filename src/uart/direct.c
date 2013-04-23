/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "uart/direct.h"


static void uart_direct_init(uint8_t dev, uint16_t divisor) {
	uint8_t ucsr_a = ((divisor & _BV(12)) ? _BV(U2X0) : 0);
	uint8_t ucsr_b = _BV(TXEN0);
	uint8_t ucsr_c = _BV(UCSZ01) | _BV(UCSZ00);
	
	uint16_t ubrr = (divisor & (_BV(12) - 1));
	
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (dev == 0) {
			/* disable everything for a moment */
			UCSR0B = 0;
			
			UCSR0A = ucsr_a;
			UCSR0B = ucsr_b;
			UCSR0C = ucsr_c;
			
			UBRR0 = ubrr;
		} else {
			/* disable everything for a moment */
			UCSR1B = 0;
			
			UCSR1A = ucsr_a;
			UCSR1B = ucsr_b;
			UCSR1C = ucsr_c;
			
			UBRR1 = ubrr;
		}
	}
}

static void uart_direct_write_chr(uint8_t dev, char chr) {
	/* convert LF to CRLF */
	if (chr == '\n') {
		uart_direct_write_chr(UART_PC, '\r');
	}
	
	while (!((dev == 0 ? UCSR0A : UCSR1A) & _BV(UDRE0)));
	
	if (dev == 0) {
		UDR0 = (uint8_t)chr;
	} else {
		UDR1 = (uint8_t)chr;
	}
}


/* synchronously write to a uart; this disables uart interrupts and effectively
 * invalidates the normal (buffered) uart state */
void uart_direct_write(uint8_t dev, uint16_t divisor, const char *str) {
	uart_direct_init(dev, divisor);
	
	while (*str != '\0') {
		uart_direct_write_chr(dev, *(str++));
	}
}

/* synchronously write to a uart; this disables uart interrupts and effectively
 * invalidates the normal (buffered) uart state */
void uart_direct_write_P(uint8_t dev, uint16_t divisor, const char *pstr) {
	uart_direct_init(dev, divisor);
	
	char c;
	while ((c = pgm_read_byte(pstr++)) != '\0') {
		uart_direct_write_chr(dev, c);
	}
}
