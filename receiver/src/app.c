#include "config.h"
#include "proto.h"
#include "main.h"

#define STATUS_IDLE     00
#define STATUS_RX       01
#define STATUS_TX_PROXY 02

#define PROXY_MESSAGE_RATE_MS 800

ProtoEnvelope g_env_rx ALIGN4;
volatile uint8_t g_status = STATUS_IDLE;

void RTC0_IRQHandler(void) {

	// Send Proxy TX
	if (NRF_RTC0->EVENTS_COMPARE[0]) {
		NRF_RTC0->EVENTS_COMPARE[0] = 0;

		if (g_status == STATUS_RX) {
			printf("Stop listening!\r\n");
			radio_stop();
			g_status = STATUS_TX_PROXY;

		}

		if (g_status == STATUS_TX_PROXY) {

			g_env.proto = RFPROTO_PROXY;
			g_env.batt = adc_bat(false);
			g_env.temp = temp_get(false);
			g_env.seq += 1;

			aes_encr(&g_env, &g_enc, sizeof(g_env), SIGNATURE_SIZE);
			printf_env(&g_env);

			radio_init_tx();
			radio_start((uint32_t) &g_enc);

		}

		if (g_status == STATUS_IDLE) {

			g_status = STATUS_RX;

			printf("Start listening!\r\n");
			radio_init_rx();
			radio_start((uint32_t) &g_enc);

			NRF_RTC0->CC[0] = NRF_RTC0->COUNTER + MILLISECONDS(PROXY_MESSAGE_RATE_MS);

		}

	}

	// Sample voltage, temp each 10 secs
	if (NRF_RTC0->EVENTS_COMPARE[1]) {
		NRF_RTC0->EVENTS_COMPARE[1] = 0;

		adc_start();
		temp_start();

		printf("Sample ADC/Temp\r\n");

		NRF_RTC0->CC[1] = NRF_RTC0->COUNTER + SECONDS(10UL);

	}

}

void RADIO_IRQHandler(void) {

	if (NRF_RADIO->EVENTS_END) {

		// Reset main timer
		NRF_RTC0->CC[0] = 0;

		/* acknowledge event */
		NRF_RADIO->EVENTS_END = 0;

		if (g_status == STATUS_RX) {
			if (NRF_RADIO->CRCSTATUS == 1) {

				if (!aes_decr(&g_enc, &g_env_rx, sizeof(g_env_rx), SIGNATURE_SIZE)) {

					printf_env(&g_env_rx);

				}

			} else {
				printf("Bad CRC!\r\n");
			}

		}

		radio_stop();

		g_status = STATUS_IDLE;

		// Setup loop after
		NRF_RTC0->CC[0] = NRF_RTC0->COUNTER + MILLISECONDS(50);

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

		printf("Address Event!\r\n");

	}

}
