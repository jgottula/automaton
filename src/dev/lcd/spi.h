/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_LCD_SPI_H
#define AUTOMATON_DEV_LCD_SPI_H


#include "std.h"


void lcd_spi_init(void);

void lcd_spi_write(uint8_t byte);
void lcd_spi_write_many(uint16_t len, const uint8_t bytes[static len]);
void lcd_spi_write_many_P(uint16_t len, const uint8_t *bytes);


#endif
