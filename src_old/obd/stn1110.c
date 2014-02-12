/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "obd/stn1110.h"
#include "time/alarm.h"
#include "uart/uart.h"


static struct {
	struct {
		bool init : 1;
		bool echo : 1;
	};
	
	struct alarm alarm;
} stn_state = {
	.init = false,
	.echo = true,
};


/* send incoming characters to the bit bucket until the prompt character;
 * timeout of zero will wait forever */
static void _stn1110_eat_prompt(uint16_t timeout) {
	if (timeout != 0) {
		alarm_start(&stn_state.alarm, timeout);
	}
	
	while (!uart_avail(UART_STN1110) || fgetc(stn1110) != '>') {
		_delay_us(1);
		
		if (timeout != 0 && alarm_expired(&stn_state.alarm)) {
			break;
		}
	}
	
	alarm_stop(&stn_state.alarm);
}

/* send incoming characters to the bit bucket until the newline character;
 * timeout of zero will wait forever */
static void _stn1110_eat_line(uint16_t timeout) {
	if (timeout != 0) {
		alarm_start(&stn_state.alarm, timeout);
	}
	
	while (!uart_avail(UART_STN1110) || fgetc(stn1110) != '\n') {
		_delay_us(1);
		
		if (timeout != 0 && alarm_expired(&stn_state.alarm)) {
			break;
		}
	}
	
	alarm_stop(&stn_state.alarm);
}


static void _stn1110_echo_off(void) {
	
}

static void _stn1110_baud_switch(void) {
	// see page 60 of the ELM327 manual for the procedure
}


void stn1110_tx(const char *prefix, const char *buf) {
	printf_P(PSTR("tx|%S%s|\n"), prefix, buf);
	fprintf_P(stn1110, PSTR("%S%s\n"), prefix, buf);
}

bool stn1110_rx(char *buf, size_t buf_max) {
	static bool in_progress = false;
	static struct {
		uint8_t buf_idx;
		
		bool post_echo    : 1;
		bool post_newline : 1;
		bool post_prompt  : 1;
	} pos;
	
	if (!in_progress) {
		pos.buf_idx = 0;
		
		pos.post_echo    = !stn_state.echo;
		pos.post_newline = false;
		pos.post_prompt  = false;
	}
	
	int c;
	while (uart_avail(UART_STN1110) && (c = fgetc(stn1110)) != EOF) {
		if (!pos.post_echo) {
			if ((char)c == '\n') {
				pos.post_echo = true;
			}
		} else {
			if (!pos.post_newline) {
				if ((char)c != '\n') {
					ASSERT(pos.buf_idx < buf_max - 1);
					buf[pos.buf_idx++] = (char)c;
				} else {
					pos.post_newline = true;
				}
			} else {
				if ((char)c == '>') {
					buf[pos.buf_idx++] = '\0';
					pos.post_prompt = true;
					break;
				}
			}
		}
	}
	
	bool done = pos.post_prompt;
	
	in_progress = !done;
	if (done) {
		printf_P(PSTR("rx|%s|\n"), buf);
	}
	
	return done;
}


/* TODO: timeout */
bool stn1110_at(const char *cmd) {
	char rsp[64];
	
	stn1110_tx(PSTR("at"), cmd);
	while (!stn1110_rx(rsp, sizeof(rsp)));
	
	// TODO: interpret response
	return true;
}


void stn1110_init(void) {
	_delay_ms(2000);
	
	uart_init(UART_STN1110, UART_DIV_STN1110, 0, 0);
	
	alarm_register(&stn_state.alarm);
	
	stn_state.echo = true;
	
	/* reboot the stn1110 if it's already up */
	//fputs_P(PSTR("atz"), stn1110);
	
	/* wait for the prompt */
	//_stn1110_eat_prompt(1000);
	
	/* disable echo */
#warning put this line back!
	//stn1110_at("e 0"); // TODO: use pstr
	stn_state.echo = false;
	// TODO: call _stn1110_echo_off
	
	/* increase baud rate */
	/*if (_stn1110_cmd(PSTR("st sbr 115200"))) { // TODO: use pstr
		uart_flush(UART_STN1110, 0);
		uart_init(UART_STN1110, UART_DIV_115200, 0, 0);
	}*/
	// TODO: call _stn1110_baud_switch
	
	// TODO: if the thing isn't responding, return an error status
	
	stn_state.init = true;
}

void stn1110_deinit(void) {
	stn_state.init = false;
	
	// TODO: flush?
	uart_stop(UART_STN1110);
	
	alarm_unregister(&stn_state.alarm);
}
