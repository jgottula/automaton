/* automaton
 * (c) 2013 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "obd/stn1110.h"
#include "uart/uart.h"


// TODO: make a state struct
static bool stn_init = false;
static bool stn_echo = true;


static void stn1110_eat_prompt(uint16_t timeout) {
	// TODO: timeout
	
	while (!uart_avail(UART_STN1110) || fgetc(stn1110) != '>') {
		_delay_us(1);
	}
}

static void stn1110_eat_line(uint16_t timeout) {
	// TODO: timeout
	
	while (!uart_avail(UART_STN1110) || fgetc(stn1110) != '\n') {
		_delay_us(1);
	}
}


static bool stn1110_cmd(const char *cmd) {
	// TODO: log all commands sent and responses received to the PC uart
	// TODO: non-PSTR version
	// TODO: timeout
	// TODO: handle commands that return string other than 'OK' or '?'
	//       (supply buffer and length; for NULL buffer, don't copy)
	
	fputs_P(cmd, stn1110);
	fputc('\n', stn1110);
	
	char buf[64];
	uint8_t len = 0;
	
	bool echo    = !stn_echo;
	bool newline = false;
	bool prompt  = false;
	do {
		int c;
		if (uart_avail(UART_STN1110) && (c = fgetc(stn1110)) != EOF) {
			// REMOVE ME
			fprintf_P(stdout, PSTR("stn rx 0x%02" PRIx8 " '%c'\n"),
				(uint8_t)c, isgraph((char)c) ? (char)c : ' ');
			
			if (!echo) {
				if ((char)c == '\n') {
					echo = true;
				}
			} else {
				if (!newline) {
					if ((char)c != '\n') {
						/* avoid overflows (including terminating NULL) */
						assert(len < sizeof(buf) - 1);
						buf[len++] = (char)c;
					} else {
						newline = true;
					}
				} else {
					if ((char)c == '>') {
						buf[len] = '\0';
						prompt = true;
					}
				}
			}
		}
	} while (!prompt);
	
	// REMOVE ME
	fprintf_P(stdout, PSTR("buf: |%s|\n"), buf);
	
	if (strcmp_P(buf, PSTR("OK")) == 0) {
		fputs_P(PSTR("stn1110_cmd: TRUE\n"), stdout);
		return true;
	} else {
		fputs_P(PSTR("stn1110_cmd: FALSE\n"), stdout);
		return false;
	}
	
	// should never be reached?!
		fputs_P(PSTR("stn1110_cmd: FALSE (!!)\n"), stdout);
	return false;
	
	// TODO: use global 'stn1110_error' errno-type variable?
	// return failure if we time out
	// return different kind of failure if reply buffer is too small
}


static void stn1110_echo_off(void) {
	
}

static void stn1110_baud_switch(void) {
	// see page 60 of the ELM327 manual for the procedure
}


void stn1110_init(void) {
	uart_init(UART_STN1110, UART_DIV_9600, 0, 0);
	
	stn_echo = true;
	
	/* reboot the stn1110 if it's already up */
	fputs_P(PSTR("atz"), stn1110);
	
	/* wait for the prompt */
	stn1110_eat_prompt(0);
	//_delay_ms(1000);
	
	/* disable echo */
	stn1110_cmd(PSTR("at e 0")); // TODO: use pstr
	stn_echo = false;
	
	/* increase baud rate */
	/*if (stn1110_cmd(PSTR("st sbr 115200"))) { // TODO: use pstr
		uart_flush(UART_STN1110, 0);
		uart_init(UART_STN1110, UART_DIV_115200, 0, 0);
	}*/
	
	stn_init = true;
}


bool stn1110_at(const char *cmd, char *reply, size_t reply_max,
	uint16_t timeout) {
	// send 'at' first, then call stn1110_cmd
}
