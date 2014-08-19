/*
 * uart.c
 *
 *  Created on: Aug 19, 2014
 *      Author: valdo
 */

#include <stdio.h>
#include "boards.h"
#include "simple_uart.h"
#include "uart.h"

void uart_init(void) {
	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
}

void dump_uint16(uint16_t* value) {
	char c32[32];
	sprintf(c32, "%u\n\r", *value);
	simple_uart_putstring((const uint8_t *) c32);
}
