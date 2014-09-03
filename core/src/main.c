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

void printf_env(const ProtoEnvelope *e) {
	switch (e->proto) {

		case RFPROTO_TRACK:

			printf("RFPROTO_TRACK: oid=%u, batt=%u, temp=%d, seq=%u\r\n", e->oid, e->batt, e->temp, e->seq);
			break;

		case RFPROTO_PROXY:

			printf("RFPROTO_PROXY: oid=%u, batt=%u, temp=%d, seq=%u ", e->oid, e->batt, e->temp, e->seq);
			for (uint8_t i = 0; i < PROXY_TAGS_LENGTH; i++) {
				const ProtoProxyTag *t = &(e->data.proxy.tags[i]);
				printf("(oid=%u,batt=%u,temp=%d,seq=%u,rssi=%d)", t->oid, t->batt, t->temp, t->seq, t->rssi);
			}
			printf("\r\n");
			break;

		default:

			printf("RFPR_ENVELOPE: proto=%u, oid=%u, batt=%u, temp=%d, seq=%u\r\n", e->proto, e->oid, e->batt, e->temp, e->seq);
			break;

	}
}


uint32_t init(void) {

	uint32_t oid = crc32(&NRF_FICR->DEVICEID, sizeof(NRF_FICR->DEVICEID));

	//nrf_gpio_cfg_output(LED_RGB_RED);
	nrf_gpio_cfg_output(LED_RGB_GREEN);
	//nrf_gpio_cfg_output(LED_RGB_BLUE);

	//nrf_gpio_pin_set(LED_RGB_RED);
	nrf_gpio_pin_set(LED_RGB_GREEN);
	//nrf_gpio_pin_set(LED_RGB_BLUE);

	timer_init();
	uart_init();
	adc_init();
	radio_init();
	rng_init();
	temp_init();

	// Start environment measurements
	adc_start();
	temp_start();

	// Init AES service
	aes_init(oid);

	// Print something
	printf("Starting Device ID[%u]\r\n", oid);

	blink(LED_RGB_GREEN, 5);

	return oid;

}

