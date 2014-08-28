#include "config.h"
#include "timer.h"
#include "adc.h"
#include "proto.h"
#include "radio.h"
#include "aes.h"
#include "rng.h"

void RTC0_IRQHandler(void) {

	// Radio send events each random ms
	if (NRF_RTC0->EVENTS_COMPARE[0]) {
		NRF_RTC0->EVENTS_COMPARE[0] = 0;

		g_env.batt = adc_bat(false);
		g_env.data.tracker.seq += 1;

		aes_encr(&g_env, &g_enc, sizeof(g_env), SIGNATURE_SIZE);
		PRINT_ENVELOPE(g_env);

		/* set first packet pointer */
		NRF_RADIO->PACKETPTR = (uint32_t)&g_enc;

		/* send */
		NRF_RADIO->TASKS_START = 1U;

	}

	// Sample voltage each 10 secs
	if (NRF_RTC0->EVENTS_COMPARE[1]) {
		NRF_RTC0->EVENTS_COMPARE[1] = 0;

		adc_start();
		printf("Sample ADC\r\n");

		NRF_RTC0->CC[1] = NRF_RTC0->COUNTER + SECONDS(10UL);

	}

}

void RADIO_IRQHandler(void) {
	if (NRF_RADIO->EVENTS_END) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_END = 0;

		// Setup loop after
		NRF_RTC0->CC[0] = NRF_RTC0->COUNTER + MILLISECONDS(50 + (rng(16) % 42));

	}
}

void radio_init(void) {

	/* setup default radio settings for track mode */
	NRF_RADIO->MODE = RADIO_MODE_MODE_Nrf_2Mbit << RADIO_MODE_MODE_Pos;
	NRF_RADIO->FREQUENCY = TRACK_CHANNEL;
	NRF_RADIO->TXPOWER = RADIO_TXPOWER;
	NRF_RADIO->TXADDRESS = RADIO_TRACK_TXADDRESS;
	NRF_RADIO->PCNF1 = RADIO_TRACK_PCNF1;

	/* generic radio setup */
	NRF_RADIO->PREFIX0 = 0x46D7UL;
	NRF_RADIO->BASE0 = 0xEA8AF0B1UL;
	NRF_RADIO->BASE1 = 0xCC864569UL;
	NRF_RADIO->RXADDRESSES = 1;
	NRF_RADIO->PCNF0 = 0x0;
	NRF_RADIO->CRCCNF = (RADIO_CRCCNF_LEN_One << RADIO_CRCCNF_LEN_Pos);
	NRF_RADIO->CRCINIT = 0xFFUL;
	NRF_RADIO->CRCPOLY = 0x107UL;

	NRF_RADIO->INTENSET = (RADIO_INTENSET_END_Enabled << RADIO_INTENSET_END_Pos);
	NVIC_SetPriority(RADIO_IRQn, IRQ_PRIORITY_RADIO);
	NVIC_ClearPendingIRQ(RADIO_IRQn);
	NVIC_EnableIRQ(RADIO_IRQn);

	NRF_RADIO->EVENTS_READY = 0U;
	NRF_RADIO->TASKS_TXEN = 1;
	while (NRF_RADIO->EVENTS_READY == 0U);

}
