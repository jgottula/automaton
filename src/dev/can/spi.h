/* automaton
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef AUTOMATON_DEV_CAN_SPI_H
#define AUTOMATON_DEV_CAN_SPI_H


#include "std.h"


void can_spi_init(void);

void can_spi_select(void);
void can_spi_deselect(void);

uint8_t can_spi_xfer(uint8_t byte);


#endif
