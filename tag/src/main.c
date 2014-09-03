#include "config.h"
#include "proto.h"
#include "main.h"

ProtoEnvelope g_env ALIGN4;
uint8_t g_enc[sizeof(g_env)] ALIGN4;

/**
 * Timer method
 */
void RTC0_IRQHandler(void) {
	if (NRF_RTC0->EVENTS_COMPARE[1]) {
		NRF_RTC0->EVENTS_COMPARE[1] = 0;

		// Sample data
		adc_start();
		temp_start();

		printf("Sample ADC/Temp\r\n");

		// Schedule next sample
		NRF_RTC0->CC[1] = NRF_RTC0->COUNTER + SECONDS(10UL);

	}
}

/**
 * Main method
 */
int main(void) {

	// Main init
    uint32_t oid = init();

	// Initialize values
    g_env.proto = RFPROTO_TRACK;
    g_env.oid   = oid;
	g_env.batt  = adc_bat(true);
	g_env.temp  = temp_get(true);
	g_env.seq   = 0;

	// Configure radio
	NRF_RADIO->EVENTS_READY = 0U;
	NRF_RADIO->PACKETPTR = (uint32_t) &g_enc;
    NRF_RADIO->SHORTS = (1 << RADIO_SHORTS_READY_START_Pos) | // Shortcut between READY event and START task
                        (1 << RADIO_SHORTS_END_DISABLE_Pos);  // Shortcut between END event and DISABLE task

	// Start timer
	NRF_RTC0->CC[1] = SECONDS(10UL);
	NRF_RTC0->TASKS_START = 1UL;

	// Loop 4ever
	while (true) {

		// Print packet about to be sent
		printf_env(&g_env);

		// Encrypt packet
		if (aes_encr(&g_env, &g_enc, sizeof(g_env), SIGNATURE_SIZE) == 0) {

			// Send and wait for end
			NRF_RADIO->EVENTS_DISABLED = 0U;
			NRF_RADIO->TASKS_TXEN = 1;
			while (NRF_RADIO->EVENTS_DISABLED == 0U);

		} else {

			printf("AES encryption error!\r\n");

		}

		// Delay randomly
		nrf_delay_ms(100 + (rng(16) % 42));

		// Refresh values
		g_env.batt = adc_bat(false);
		g_env.temp = temp_get(false);
		g_env.seq += 1;

	}

}
