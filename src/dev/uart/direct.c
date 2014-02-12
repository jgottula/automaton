/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/uart/direct.h"


static void _uart_direct_init(void) {
	// initialize USART: manual, 21.5
	// first, set port direction
	// make sure to set RTS and CTS such that flow will never be impeded
	// we only need TX to be enabled (not RX)
	
	//REG_MODIFY(DIRF, 0x)
	
	//USARTF0.
}



#if 0
static void _uart_direct_init(uint8_t dev, uint16_t divisor) {
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


static void _uart_direct_write(uint8_t dev, char chr) {
	/* convert LF to CRLF */
	if (chr == '\n') {
		_uart_direct_write(UART_PC, '\r');
	}
	
	while (!((dev == 0 ? UCSR0A : UCSR1A) & _BV(UDRE0)));
	
	if (dev == 0) {
		UDR0 = (uint8_t)chr;
	} else {
		UDR1 = (uint8_t)chr;
	}
}

static void _uart_direct_flush(uint8_t dev) {
	while (!((dev == 0 ? UCSR0A : UCSR1A) & _BV(TXC0)));
}


/* synchronously write to a uart; this disables uart interrupts and effectively
 * invalidates the normal (buffered) uart state */
void uart_direct_write_chr(uint8_t dev, uint16_t divisor, char chr) {
	_uart_direct_init(dev, divisor);
	
	_uart_direct_write(dev, chr);
	
	_uart_direct_flush(dev);
}

/* synchronously write to a uart; this disables uart interrupts and effectively
 * invalidates the normal (buffered) uart state */
void uart_direct_write_str(uint8_t dev, uint16_t divisor, const char *str) {
	_uart_direct_init(dev, divisor);
	
	while (*str != '\0') {
		_uart_direct_write(dev, *(str++));
	}
	
	_uart_direct_flush(dev);
}

/* synchronously write to a uart; this disables uart interrupts and effectively
 * invalidates the normal (buffered) uart state */
void uart_direct_write_pstr(uint8_t dev, uint16_t divisor, const char *pstr) {
	_uart_direct_init(dev, divisor);
	
	char c;
	while ((c = pgm_read_byte(pstr++)) != '\0') {
		_uart_direct_write(dev, c);
	}
	
	_uart_direct_flush(dev);
}
#endif
