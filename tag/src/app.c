#include "config.h"
#include "proto.h"
#include "main.h"

void RTC0_IRQHandler(void) {

	// Each random ms
	if (NRF_RTC0->EVENTS_COMPARE[0]) {
		NRF_RTC0->EVENTS_COMPARE[0] = 0;

		g_env.batt = adc_bat(false);
		g_env.temp = temp_get(false);
		g_env.seq += 1;

#ifdef BOARD_PCA10000
		printf_env(&g_env);
#endif

		aes_encr(&g_env, &g_enc, sizeof(g_env), SIGNATURE_SIZE);
		radio_start((uint32_t) &g_enc);

	}

	// Sample voltage each 10 secs
	if (NRF_RTC0->EVENTS_COMPARE[1]) {
		NRF_RTC0->EVENTS_COMPARE[1] = 0;

		adc_start();
		temp_start();

#ifdef BOARD_PCA10000
		printf("Sample ADC/Temp\r\n");
#endif

		NRF_RTC0->CC[1] = NRF_RTC0->COUNTER + SECONDS(10UL);

	}

}

void RADIO_IRQHandler(void) {

	if (NRF_RADIO->EVENTS_END) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_END = 0;

		// Setup loop after
		NRF_RTC0->CC[0] = NRF_RTC0->COUNTER + MILLISECONDS(50 + (rng(16) % 42));

		blink(LED_RGB_RED, 5);

	}

	if (NRF_RADIO->EVENTS_READY) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_READY = 0;

	}

	if (NRF_RADIO->EVENTS_DISABLED) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_DISABLED = 0;

	}

}

