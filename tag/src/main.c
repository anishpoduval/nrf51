#include "config.h"
#include "proto.h"
#include "main.h"

int main(void) {

	nrf_gpio_cfg_output(LED_RGB_RED);
	nrf_gpio_cfg_output(LED_RGB_GREEN);
	nrf_gpio_cfg_output(LED_RGB_BLUE);

	nrf_gpio_pin_set(LED_RGB_RED);
	nrf_gpio_pin_set(LED_RGB_GREEN);
	nrf_gpio_pin_set(LED_RGB_BLUE);

	blink_loop(LED_RGB_GREEN);

	init(RFPROTO_TRACK);

	// Start timer
	NRF_RTC0->CC[0] = MILLISECONDS(100UL);
	NRF_RTC0->CC[1] = SECONDS(1UL);

	NRF_RTC0->TASKS_START = 1;

	radio_init_tx();

	blink_loop(LED_RGB_RED);

}
