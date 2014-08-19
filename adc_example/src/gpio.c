/*
 * gpio.c
 *
 *  Created on: Aug 19, 2014
 *      Author: valdo
 */

#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "gpio.h"
#include "boards.h"

void gpio_init(void) {

	nrf_gpio_cfg_output(LED_RGB_RED);
	nrf_gpio_pin_clear(LED_RGB_RED);

}

void blink_red(void) {

	nrf_gpio_pin_set(LED_RGB_RED);
	nrf_delay_us(10000);
	nrf_gpio_pin_clear(LED_RGB_RED);

}
