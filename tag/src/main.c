#include "config.h"
#include "adc.h"
#include "uart.h"
#include "timer.h"
#include "proto.h"
#include "crc32.h"

volatile ProtoEnvelope g_env ALIGN4;
volatile uint8_t g_enc[sizeof(g_env)] ALIGN4;

int main(void) {

	timer_init();
	uart_init();
	adc_init();

	// Reset envelope
	memset(&g_env, 0, sizeof(g_env));

	adc_start();

	g_env.oid = crc32(&NRF_FICR->DEVICEID, sizeof(NRF_FICR->DEVICEID));
	g_env.batt = adc_bat(true);

	printf("Starting Device ID[%d]\n", g_env.oid);

	while (true) {

		g_env.batt = adc_bat(false);

		//nrf_gpio_pin_clear(LED_RGB_RED);
		//nrf_gpio_pin_set(LED_RGB_BLUE);
		//nrf_delay_us(1000000);
		//nrf_gpio_pin_clear(LED_RGB_BLUE);
		//nrf_gpio_pin_set(LED_RGB_RED);

	}

}
