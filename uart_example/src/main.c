#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "simple_uart.h"
#include "nrf_gpio.h"
#include "boards.h"

int main(void) {

	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);

	nrf_gpio_cfg_output(LED_RGB_RED);
	nrf_gpio_cfg_output(LED_RGB_BLUE);

	while (true) {

		nrf_gpio_pin_clear(LED_RGB_BLUE);
		nrf_gpio_pin_set(LED_RGB_RED);
		nrf_delay_us(1000000);

		simple_uart_putstring((const uint8_t *)" \n\rType something (q - exit): ");
		uint8_t cr = simple_uart_get();

		simple_uart_putstring((const uint8_t *)" \n\rReceived: ");
		simple_uart_put(cr);

		nrf_gpio_pin_clear(LED_RGB_RED);
		nrf_gpio_pin_set(LED_RGB_BLUE);
		nrf_delay_us(1000000);

		if (cr == 'q' || cr == 'Q') {
			simple_uart_putstring((const uint8_t *)" \n\rBye bye.");
			while (1);
		}

	}

}
