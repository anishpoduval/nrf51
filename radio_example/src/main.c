#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "radio.h"

static uint8_t packet[4];

int main(void) {

    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

	radio_init();

    // Set payload pointer
    NRF_RADIO->PACKETPTR = (uint32_t)packet;

	while (true) {

		nrf_gpio_pin_clear(LED_RGB_RED);
		nrf_gpio_pin_set(LED_RGB_BLUE);
		nrf_delay_us(1000000);

		packet[0] = 128;
		NRF_RADIO->EVENTS_READY = 0U;
		NRF_RADIO->TASKS_TXEN = 1;
		while (NRF_RADIO->EVENTS_READY == 0U);

		NRF_RADIO->TASKS_START = 1U;
		NRF_RADIO->EVENTS_END = 0U;
		while (NRF_RADIO->EVENTS_END == 0U);

		NRF_RADIO->EVENTS_DISABLED = 0U;

		// Disable radio
		NRF_RADIO->TASKS_DISABLE = 1U;
		while (NRF_RADIO->EVENTS_DISABLED == 0U);

		nrf_gpio_pin_clear(LED_RGB_BLUE);
		nrf_gpio_pin_set(LED_RGB_RED);
		nrf_delay_us(1000000);
	}
}
