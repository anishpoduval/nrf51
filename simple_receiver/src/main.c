#include "config.h"
#include "proto.h"
#include "main.h"

void RADIO_IRQHandler(void) {

	if (NRF_RADIO->EVENTS_END) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_END = 0;

	}

	if (NRF_RADIO->EVENTS_READY) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_READY = 0;

	}

	if (NRF_RADIO->EVENTS_DISABLED) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_DISABLED = 0;

	}

	if (NRF_RADIO->EVENTS_ADDRESS) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_ADDRESS = 0;

	}

}

int main(void) {

    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

	// flash red LED and blue LED alternately
	nrf_gpio_cfg_output(LED_RGB_RED);
	nrf_gpio_cfg_output(LED_RGB_BLUE);
	nrf_gpio_pin_set(LED_RGB_RED);
	nrf_gpio_pin_set(LED_RGB_BLUE);

	radio_init();
	radio_init_rx();

	ProtoEnvelope g_env ALIGN4;
	uint8_t g_enc[sizeof(g_env)] ALIGN4;

	radio_start((uint32_t) g_enc);

	while (true) {

		nrf_gpio_pin_clear(LED_RGB_RED);
		nrf_gpio_pin_set(LED_RGB_BLUE);
		nrf_delay_us(1000000);
		nrf_gpio_pin_clear(LED_RGB_BLUE);
		nrf_gpio_pin_set(LED_RGB_RED);
		nrf_delay_us(1000000);
	}
}
