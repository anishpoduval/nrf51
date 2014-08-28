#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "radio.h"
#include "uart.h"

static uint8_t packet[4];

int main(void) {

    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

    nrf_gpio_cfg_output(LED_RGB_RED);
    nrf_gpio_cfg_output(LED_RGB_BLUE);

    uart_init(TX_PIN_NUMBER, RX_PIN_NUMBER);
	radio_init();

    // Set payload pointer
    NRF_RADIO->PACKETPTR = (uint32_t) packet;

	while (true) {

		//nrf_gpio_pin_set(LED_RGB_BLUE);

		//radio_send();

		//uart_putstring((const uint8_t *) "Sent ");
		//uart_put(packet[0]);
		//uart_putstring("\n\r");

		//nrf_gpio_pin_clear(LED_RGB_BLUE);
		//nrf_delay_us(1000000);

		//nrf_gpio_pin_set(LED_RGB_RED);

		if (radio_receive()) {
			printf("Received: no = %d, rssi = %d\n\r", packet[0], NRF_RADIO->RSSISAMPLE);
		} else {
			printf("Bad CRC");
		}

		//nrf_gpio_pin_clear(LED_RGB_RED);
		//nrf_delay_us(1000000);

	}
}
