#include "config.h"
#include "adc.h"
#include "uart.h"
#include "timer.h"
#include "proto.h"
#include "crc32.h"
#include "radio.h"
#include "aes.h"
#include "rng.h"
#include "temp.h"

void blink(uint8_t pin, uint8_t times) {
	while(times--) {
		nrf_gpio_pin_clear(pin);
		nrf_delay_ms(5);
		nrf_gpio_pin_set(pin);
		nrf_delay_ms(50);
	}
}

int main(void) {

	timer_init();
	uart_init();
	adc_init();
	radio_init();
	rng_init();
	temp_init();

	nrf_gpio_cfg_output(LED_RGB_RED);
	nrf_gpio_cfg_output(LED_RGB_GREEN);
	nrf_gpio_cfg_output(LED_RGB_BLUE);

	nrf_gpio_pin_set(LED_RGB_RED);
	nrf_gpio_pin_set(LED_RGB_GREEN);
	nrf_gpio_pin_set(LED_RGB_BLUE);

	// Start environment measurements
	adc_start();
	temp_start();

	// Reset envelope
	memset((uint8_t*) &g_env, 0, sizeof(g_env));

	// Initialize values
	g_env.oid = crc32(&NRF_FICR->DEVICEID, sizeof(NRF_FICR->DEVICEID));
	g_env.batt = adc_bat(true);
	g_env.temp = temp_get(true);
	g_env.proto = RFPROTO_TRACK;
	g_env.data.tracker.seq = 0;

	// Init AES service
	aes_init(g_env.oid);

	// Print something
	printf("Starting Device ID[%u]\r\n", (uint32_t) g_env.oid);

	blink(LED_RGB_BLUE, 5);

	// Start timer
	NRF_RTC0->CC[0] = MILLISECONDS(100UL);
	NRF_RTC0->CC[1] = SECONDS(1UL);

	NRF_RTC0->TASKS_START = 1;

	while (true) {
		nrf_gpio_pin_clear(LED_RGB_GREEN);
		nrf_delay_ms(5);
		nrf_gpio_pin_set(LED_RGB_GREEN);
		nrf_delay_ms(2000);
	}

}
