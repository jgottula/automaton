/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "io/uart.h"
#include "algo/fifo.h"
#include "time/alarm.h"


enum uart_state {
	UART_ST_INIT      = _BV(0), // uart has been initialized
	UART_ST_TX_ACTIVE = _BV(1), // interrupt-driven tx is ongoing
	UART_ST_TX_WAIT   = _BV(2), // the last bit is still in the uart's tx buffer
};


struct uart {
	uint8_t state;
	
	uint16_t timeout_tx_ms;
	uint16_t timeout_rx_ms;
	
	struct fifo fifo_rx;
	struct fifo fifo_tx;
	
	volatile uint8_t  *const ucsr_a;
	volatile uint8_t  *const ucsr_b;
	volatile uint8_t  *const ucsr_c;
	volatile uint16_t *const ubrr;
	volatile uint8_t  *const udr;
};


static volatile struct uart uarts[2] = {
	{
		.state = 0,
		
		.ucsr_a = &UCSR0A,
		.ucsr_b = &UCSR0B,
		.ucsr_c = &UCSR0C,
		.ubrr   = &UBRR0,
		.udr    = &UDR0,
	},
	{
		.state = 0,
		
		.ucsr_a = &UCSR1A,
		.ucsr_b = &UCSR1B,
		.ucsr_c = &UCSR1C,
		.ubrr   = &UBRR1,
		.udr    = &UDR1,
	},
};


/* nonatomic: pop a byte from the tx fifo and transmit it; does no checking */
static void uart_tx_from_fifo(struct uart *uart) {
	uint8_t byte;
	if (fifo_pop(&uart->fifo_tx, &byte)) {
		*uart->udr = byte;
	}
}

/* nonatomic: receive a byte and push it to the rx fifo; does no checking */
static void uart_rx_to_fifo(struct uart *uart) {
	uint8_t byte = *uart->udr;
	fifo_push_force(&uart->fifo_rx, byte);
}


/* atomic: common rx interrupt handler */
static void uart_int_rx(struct uart *uart) {
	// TODO: check for UCSRnB->DORn (data overrun) before reading UDRn
	
	uart_rx_to_fifo(uart);
}

/* atomic: common udre interrupt handler */
static void uart_int_udre(struct uart *uart) {
	uart_tx_from_fifo(uart);
	
	if (fifo_count(&uart->fifo_tx) == 0) {
		uart->state &= ~UART_ST_TX_ACTIVE;
		
		io_write(*uart->ucsr_b, _BV(TXCIE0), _BV(TXCIE0));
		uart->state |= UART_ST_TX_WAIT;
	}
}

/* atomic: common tx interrupt handler */
static void uart_int_tx(struct uart *uart) {
	io_write(*uart->ucsr_b, _BV(TXCIE0), 0);
	uart->state &= ~UART_ST_TX_WAIT;
}


ISR(USART0_RX_vect) {
	uart_int_rx((struct uart *)uarts + 0);
}

ISR(USART0_UDRE_vect) {
	uart_int_udre((struct uart *)uarts + 0);
}

ISR(USART0_TX_vect) {
	uart_int_tx((struct uart *)uarts + 0);
}


ISR(USART1_RX_vect) {
	uart_int_rx((struct uart *)uarts + 1);
}

ISR(USART1_UDRE_vect) {
	uart_int_udre((struct uart *)uarts + 1);
}

ISR(USART1_TX_vect) {
	uart_int_tx((struct uart *)uarts + 1);
}


/* atomic: write a byte to the tx fifo and prime the pump if necessary */
static bool uart_write_raw(uint8_t dev, uint8_t byte) {
	/* non-volatile */
	struct uart *uart = (struct uart *)uarts + dev;
	
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (uart->state & UART_ST_INIT) {
			if (uart->timeout_tx_ms != 0) {
				result = fifo_push_wait(&uart->fifo_tx, byte,
					uart->timeout_tx_ms);
			} else {
				result = fifo_push(&uart->fifo_tx, byte);
			}
			
			/* prime the pump */
			if (result && !(uart->state & UART_ST_TX_ACTIVE)) {
				uart_tx_from_fifo(uart);
				uart->state |= UART_ST_TX_ACTIVE;
			}
		}
	}
	
	return result;
}

/* atomic: read a byte from the rx fifo if possible */
static bool uart_read_raw(uint8_t dev, uint8_t *byte) {
	/* non-volatile */
	struct uart *uart = (struct uart *)uarts + dev;
	
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (uart->state & UART_ST_INIT) {
			if (uart->timeout_rx_ms != 0) {
				result = fifo_pop_wait(&uart->fifo_rx, byte,
					uart->timeout_rx_ms);
			} else {
				result = fifo_pop(&uart->fifo_rx, byte);
			}
		}
	}
	
	return result;
}


/* atomic: initialize uart with baudrate divisor and optional tx/rx timeouts; a
 * timeout of zero will give up immediately */
void uart_init(uint8_t dev, uint16_t divisor, uint16_t timeout_tx_ms,
	uint16_t timeout_rx_ms) {
	/* non-volatile */
	struct uart *uart = (struct uart *)uarts + dev;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		uart->state = UART_ST_INIT;
		
		uart->timeout_tx_ms = timeout_tx_ms;
		uart->timeout_rx_ms = timeout_rx_ms;
		
		fifo_init(&uart->fifo_rx);
		fifo_init(&uart->fifo_tx);
		
		*uart->ucsr_a = ((divisor & _BV(12)) ? _BV(U2X0) : 0);
		*uart->ucsr_b = _BV(RXCIE0) | _BV(UDRIE0) | _BV(RXEN0) | _BV(TXEN0);
		*uart->ucsr_c = _BV(UCSZ01) | _BV(UCSZ00);
		
		*uart->ubrr = (divisor & (_BV(12) - 1));
	}
}

/* atomic: disable uart completely; stops interrupts and makes future reads and
 * writes fail */
void uart_stop(uint8_t dev) {
	/* non-volatile */
	struct uart *uart = (struct uart *)uarts + dev;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		uart->state = 0;
		
		*uart->ucsr_b = 0;
	}
}

/* atomic: flush the uart's tx buffer; a timeout of zero will wait forever;
 * returns false on timeout */
bool uart_flush(uint8_t dev, uint16_t timeout_ms) {
	volatile struct uart *uart = uarts + dev;
	
	if (timeout_ms != 0) {
		alarm_set(timeout_ms);
	}
	
	for ( ; ; ) {
		_delay_us(1);
		
		/* wait for tx to completely finish */
		if (!(uart->state & (UART_ST_TX_ACTIVE | UART_ST_TX_WAIT))) {
			alarm_unset();
			return true;
		}
		
		if (timeout_ms != 0 && alarm_check()) {
			return false;
		}
	}
}


/* nonatomic: write a character to the uart; optionally converts \n to \r\n;
 * returns false on failure */
bool uart_write(uint8_t dev, char chr) {
#if UART_LF_TO_CRLF
	if (chr == '\n') {
		if (!uart_write(dev, '\r')) {
			return false;
		}
	}
#endif
	
	return uart_write_raw(dev, chr);
}

/* nonatomic: read a character from the uart; returns false on failure */
bool uart_read(uint8_t dev, char *chr) {
	return uart_read_raw(dev, (uint8_t *)chr);
}
