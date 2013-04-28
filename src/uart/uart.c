/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "uart/uart.h"
#include "algo/fifo.h"
#include "lcd/lcd.h"
#include "time/alarm.h"


#define UART_DEBUG_FIFO      0 // lcd: show fifo contents
#define UART_DEBUG_INT_FLAG  0 // lcd: show interrupt flag changes
#define UART_DEBUG_INT_COUNT 0 // count interrupt occurrences


enum uart_state {
	UART_ST_INIT      = _BV(0), // uart has been initialized
	UART_ST_TX_ACTIVE = _BV(1), // interrupt-driven tx is ongoing
};


struct uart {
	uint8_t state;
	
	uint16_t timeout_tx;
	uint16_t timeout_rx;
	
	struct fifo fifo_tx;
	struct fifo fifo_rx;
	
	volatile uint8_t  *const ucsr_a;
	volatile uint8_t  *const ucsr_b;
	volatile uint8_t  *const ucsr_c;
	volatile uint16_t *const ubrr;
	volatile uint8_t  *const udr;
};


static struct uart uarts[2] = {
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

#if UART_DEBUG_INT_COUNT
volatile uint16_t uart_int_count[2][2] = {
	{ 0, 0, },
	{ 0, 0, },
};
#endif


#if UART_DEBUG_FIFO
/* nonatomic: print fifo information to the lcd */
static void _uart_debug_fifo_common(const struct fifo *fifo) {
	/* length and indexes */
	fprintf_P(lcd, PSTR("len%02u push%02u pop%02u\r\n"),
		fifo->len, fifo->i_push, fifo->i_pop);
	
	/* data contents */
	uint8_t i = FIFO_SIZE;
	const uint8_t *d = fifo->data;
	while (i-- != 0) {
		char c = *(d++);
		if (!isprint(c) || c == '\r' || c == '\n') {
			c = '\xff';
		}
		
		fputc(c, lcd);
	}
	fputc('\n', lcd);
	
	/* index pointers */
	lcd_goto_c(fifo->i_push);
	fputc('^', lcd);
	lcd_goto_c(fifo->i_pop);
	fputc('^', lcd);
}

/* nonatomic: print tx fifo information to the lcd */
static void _uart_debug_fifo_tx(const struct uart *uart) {
	lcd_clear();
	fprintf_P(lcd, PSTR("uarts[%" PRIu8 "]->fifo_tx:\r\n"), uart - uarts);
	_uart_debug_fifo_common(&uart->fifo_tx);
}

/* nonatomic: print rx fifo information to the lcd */
static void _uart_debug_fifo_rx(const struct uart *uart) {
	lcd_clear();
	fprintf_P(lcd, PSTR("uarts[%" PRIu8 "]->fifo_rx:\r\n"), uart - uarts);
	_uart_debug_fifo_common(&uart->fifo_rx);
}
#endif


/* nonatomic: pop a byte from the tx fifo and transmit it; does no checking */
static void _uart_tx_from_fifo(struct uart *uart) {
	uint8_t byte;
	if (fifo_pop(&uart->fifo_tx, &byte)) {
		while (!(*uart->ucsr_a & _BV(UDRE0)));
		*uart->udr = byte;
	}
	
#if UART_DEBUG_FIFO
	_uart_debug_fifo_tx(uart);
#endif
}

/* nonatomic: receive a byte and push it to the rx fifo; does no checking */
static void _uart_rx_to_fifo(struct uart *uart) {
	uint8_t byte = *uart->udr;
	fifo_push_force(&uart->fifo_rx, byte);
	
#if UART_DEBUG_FIFO
	_uart_debug_fifo_rx(uart);
#endif
}


/* atomic: common rx interrupt handler */
static void _uart_int_rx(struct uart *uart) {
#if UART_DEBUG_INT_FLAG
	lcd_write('<');
#endif
	
	// TODO: check for UCSRnB->DORn (data overrun) before reading UDRn
	
	_uart_rx_to_fifo(uart);
	
#if UART_DEBUG_INT_FLAG
	lcd_write('>');
#endif
}

/* atomic: common udre interrupt handler */
static void _uart_int_udre(struct uart *uart) {
#if UART_DEBUG_INT_FLAG
	lcd_write('[');
#endif
	
	if (fifo_count(&uart->fifo_tx) == 1) {
		/* no longer in interrupt-driven mode */
		uart->state &= ~UART_ST_TX_ACTIVE;
		
		/* disable this interrupt */
		IO_WRITE(*uart->ucsr_b, _BV(UDRIE0), 0);
		
#if UART_DEBUG_INT_FLAG
		lcd_write('u');
#endif
	}
	
	_uart_tx_from_fifo(uart);
	
#if UART_DEBUG_INT_FLAG
	lcd_write(']');
#endif
}


ISR(USART0_RX_vect) {
	_uart_int_rx((struct uart *)uarts + 0);
	
#if UART_DEBUG_INT_COUNT
	++uart_int_count[0][0];
#endif
}

ISR(USART0_UDRE_vect) {
	_uart_int_udre((struct uart *)uarts + 0);
	
#if UART_DEBUG_INT_COUNT
	++uart_int_count[0][1];
#endif
}


ISR(USART1_RX_vect) {
	_uart_int_rx((struct uart *)uarts + 1);
	
#if UART_DEBUG_INT_COUNT
	++uart_int_count[1][0];
#endif
}

ISR(USART1_UDRE_vect) {
	_uart_int_udre((struct uart *)uarts + 1);
	
#if UART_DEBUG_INT_COUNT
	++uart_int_count[1][1];
#endif
}


/* atomic: write a byte to the tx fifo and prime the pump if necessary */
static bool _uart_write(uint8_t dev, uint8_t byte) {
	struct uart *uart = uarts + dev;
	
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (uart->state & UART_ST_INIT) {
			result = fifo_push_wait(&uart->fifo_tx, byte, uart->timeout_tx);
			
			if (result && !(uart->state & UART_ST_TX_ACTIVE)) {
#if UART_DEBUG_INT_FLAG
				lcd_write('(');
#endif
				
				/* now in interrupt-driven mode */
				uart->state |= UART_ST_TX_ACTIVE;
				
				/* enable the udre interrupt */
				IO_WRITE(*uart->ucsr_b, _BV(UDRIE0), _BV(UDRIE0));
				
#if UART_DEBUG_INT_FLAG
				lcd_write('U');
				lcd_write(')');
#endif
			}
		}
		
#if UART_DEBUG_FIFO
		if (result) {
			_uart_debug_fifo_tx(uart);
		}
#endif
	}
	
	return result;
}

/* atomic: read a byte from the rx fifo if possible */
static bool _uart_read(uint8_t dev, uint8_t *byte) {
	struct uart *uart = uarts + dev;
	
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (uart->state & UART_ST_INIT) {
#if UART_DEBUG_INT_FLAG
			lcd_write('{');
#endif
			
			result = fifo_pop_wait(&uart->fifo_rx, byte, uart->timeout_rx);
			
#if UART_DEBUG_INT_FLAG
			lcd_write('}');
#endif
		}
		
#if UART_DEBUG_FIFO
		if (result) {
			_uart_debug_fifo_rx(uart);
		}
#endif
	}
	
	return result;
}


/* atomic: initialize uart with baudrate divisor and optional tx/rx timeouts;
 * any previous state will be cleared; a timeout of zero will try forever */
void uart_init(uint8_t dev, uint16_t divisor, uint16_t timeout_tx,
	uint16_t timeout_rx) {
	struct uart *uart = uarts + dev;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		uart->state = UART_ST_INIT;
		
		uart->timeout_tx = timeout_tx;
		uart->timeout_rx = timeout_rx;
		
		fifo_init(&uart->fifo_rx);
		fifo_init(&uart->fifo_tx);
		
		*uart->ucsr_a = ((divisor & _BV(12)) ? _BV(U2X0) : 0);
		*uart->ucsr_b = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
		*uart->ucsr_c = _BV(UCSZ01) | _BV(UCSZ00);
		
		*uart->ubrr = (divisor & (_BV(12) - 1));
	}
}

/* atomic: disable uart completely; stops interrupts and makes future reads and
 * writes fail */
void uart_stop(uint8_t dev) {
	struct uart *uart = uarts + dev;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		uart->state = 0;
		
		*uart->ucsr_b = 0;
	}
	
	fifo_deinit(&uart->fifo_rx);
	fifo_deinit(&uart->fifo_tx);
}


/* atomic: flush the uart's tx buffer; a timeout of zero will wait forever;
 * returns false on timeout */
bool uart_flush(uint8_t dev, uint16_t timeout) {
	struct uart *uart = uarts + dev;
	if (!(uart->state & UART_ST_INIT)) {
		return false;
	}
	
	if (timeout != 0) {
		struct alarm alarm;
		alarm_register(&alarm);
		alarm_start(&alarm, timeout);
		
		bool result;
		for ( ; ; ) {
			/* wait for tx to completely finish */
			if (!(uart->state & UART_ST_TX_ACTIVE)) {
				result = true;
				break;
			}
			
			if (timeout != 0 && alarm_expired(&alarm)) {
				result = false;
				break;
			}
			
			_delay_us(1);
		}
		
		alarm_unregister(&alarm);
		return result;
	} else {
		for ( ; ; ) {
			if (!(uart->state & UART_ST_TX_ACTIVE)) {
				return true;
			}
			
			_delay_us(1);
		}
	}
}


/* nonatomic: count the bytes waiting in the rx fifo */
uint8_t uart_avail(uint8_t dev) {
	struct uart *uart = uarts + dev;
	if (!(uart->state & UART_ST_INIT)) {
		return 0;
	}
	
	return uart->fifo_rx.len;
}


/* nonatomic: write a character to the uart; returns false on failure */
bool uart_write(uint8_t dev, char chr) {
	struct uart *uart = uarts + dev;
	if (!(uart->state & UART_ST_INIT)) {
		return false;
	}
	
	return _uart_write(dev, chr);
}

/* nonatomic: read a character from the uart; returns false on failure */
bool uart_read(uint8_t dev, char *chr) {
	struct uart *uart = uarts + dev;
	if (!(uart->state & UART_ST_INIT)) {
		return false;
	}
	
	return _uart_read(dev, (uint8_t *)chr);
}
