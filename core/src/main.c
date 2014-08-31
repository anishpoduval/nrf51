/*
 * main.c
 *
 *  Created on: Aug 29, 2014
 *      Author: valdo
 */

#include "main.h"

void blink(uint8_t pin, uint8_t times) {
	while(times--) {
		nrf_gpio_pin_clear(pin);
		nrf_delay_ms(5);
		nrf_gpio_pin_set(pin);
		nrf_delay_ms(50);
	}
}

void blink_loop(uint8_t pin) {
	while (true) {
		nrf_gpio_pin_clear(pin);
		nrf_delay_ms(5);
		nrf_gpio_pin_set(pin);
		nrf_delay_ms(2000);
	}
}

#ifdef BOARD_PCA10000
void printf_env(ProtoEnvelope *e) {
	printf("ProtoEnvelope: oid=%u, batt=%u, temp=%d, proto=%u, seq=%u\r\n",
		                 e->oid,  e->batt,  e->temp, e->proto, e->seq);
}
#endif


void init(uint8_t proto) {

	nrf_gpio_cfg_output(LED_RGB_RED);
	nrf_gpio_cfg_output(LED_RGB_GREEN);
	nrf_gpio_cfg_output(LED_RGB_BLUE);

	nrf_gpio_pin_set(LED_RGB_RED);
	nrf_gpio_pin_set(LED_RGB_GREEN);
	nrf_gpio_pin_set(LED_RGB_BLUE);

	blink(LED_RGB_GREEN, 20);

	timer_init();
#ifdef BOARD_PCA10000
	uart_init();
#endif
	adc_init();
	radio_init();
	rng_init();
	temp_init();

	// Start environment measurements
	adc_start();
	temp_start();

	// Reset envelope
	memset((uint8_t*) &g_env, 0, sizeof(g_env));

	// Initialize values
	g_env.oid = crc32(&NRF_FICR->DEVICEID, sizeof(NRF_FICR->DEVICEID));
	g_env.batt = adc_bat(true);
	g_env.temp = temp_get(true);
	g_env.proto = proto;
	g_env.seq = 0;

	// Init AES service
	aes_init(g_env.oid);

#ifdef BOARD_PCA10000
	// Print something
	printf("Starting Device ID[%u]\r\n", (uint32_t) g_env.oid);
#endif

	blink(LED_RGB_BLUE, 5);

}

