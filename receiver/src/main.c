#include "config.h"
#include "adc.h"
#include "uart.h"
#include "timer.h"
#include "proto.h"
#include "crc32.h"
#include "radio.h"
#include "aes.h"
#include "rng.h"

int main(void) {

	timer_init();
	uart_init();
	adc_init();
	radio_init();
	rng_init();

	// Start ADC
	adc_start();

	// Reset envelope
	memset((uint8_t*) &g_env, 0, sizeof(g_env));

	// Initialize values
	g_env.oid = crc32(&NRF_FICR->DEVICEID, sizeof(NRF_FICR->DEVICEID));
	g_env.batt = adc_bat(true);
	g_env.proto = RFPROTO_TRACK;
	g_env.data.tracker.seq = 0;

	// Init AES service
	aes_init(g_env.oid);

	// Print something
	printf("Starting Device ID[%d]\r\n", (uint32_t) g_env.oid);

	// Start timer
	NRF_RTC0->CC[0] = MILLISECONDS(100UL);
	NRF_RTC0->CC[1] = SECONDS(1UL);

	NRF_RTC0->TASKS_START = 1;

	while (true);

}
