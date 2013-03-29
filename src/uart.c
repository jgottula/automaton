/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "uart.h"


void uart_init(void) {
	/* 9600 baud */
	UBRR0 = 129;
	
	/* no double speed */
	io_write(UCSR0A, _BV(U2X0), 0);
	
	/* async 8N1 */
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	
	/* no interrupts */
	UCSR0B = 0;
}


void uart_write_chr(char chr) {
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = chr;
}

void uart_write_str(const char *str) {
	while (*str != '\0') {
		uart_write_chr(*(str++));
	}
}


static void uart_write_hex_nibble(uint8_t nibble) {
	if (nibble < 0xa) {
		uart_write_chr('0' + nibble);
	} else {
		uart_write_chr('a' + (nibble - 0xa));
	}
}

void uart_write_hex8(uint8_t hex) {
	uart_write_hex_nibble(hex >> 4);
	uart_write_hex_nibble(hex & 0x0f);
}

void uart_write_hex16(uint16_t hex) {
	uart_write_hex8(hex >> 8);
	uart_write_hex8(hex & 0xff);
}
