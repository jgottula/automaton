/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "iso9141-2.h"


static void iso_raw_write(uint8_t byte) {
	while (!(UCSR1A & _BV(UDRE1)));
	UDR1 = byte;
}


static void iso_raw_cmd(void) {
	
}


void iso_init(void) {
	/* set io directions; pull the K line high */
	io_write(DDR(IO_ISO9141), IO_ISO9141_ALL, IO_ISO9141_TX);
	io_write(PORT(IO_ISO9141), IO_ISO9141_ALL, IO_ISO9141_TX);
	_delay_ms(300);
	
	/* K low */
	io_write(PORT(IO_ISO9141), IO_ISO9141_TX, 0);
	_delay_ms(25);
	
	/* K high */
	io_write(PORT(IO_ISO9141), IO_ISO9141_TX, IO_ISO9141_TX);
	_delay_ms(25);
	
	/* 10400 baud */
	UBRR1 = 119;
	
	/* no double speed */
	io_write(UCSR1A, _BV(U2X1), 0);
	
	/* async 8N1 */
	UCSR1C = _BV(UCSZ10) | _BV(UCSZ11);
	
	/* enable tx and rx; no interrupts */
	UCSR1B = _BV(TXEN1) | _BV(RXEN1);
	
	// ...
}


bool iso_read(uint8_t *byte) {
	if (UCSR1A & _BV(RXC1)) {
		*byte = UDR1;
		return true;
	} else {
		return false;
	}
}
