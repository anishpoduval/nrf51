#include "config.h"
#include "proto.h"
#include "main.h"
#include "store.h"

ProtoEnvelope g_env ALIGN4, g_rec ALIGN4;
uint8_t g_enc[sizeof(g_env)] ALIGN4;

#define STATUS_IDLE  0x0
#define STATUS_RX    0x1
#define STATUS_TX    0x2

#define TX_RATE_MS   800
#define TX_REPEAT    3

volatile uint8_t g_status = STATUS_RX;
uint8_t g_tx_numb = TX_REPEAT;

/**
 * Timer interrupt handler
 */
void RTC0_IRQHandler(void) {

	if (NRF_RTC0->EVENTS_COMPARE[0]) {
		NRF_RTC0->EVENTS_COMPARE[0] = 0;

		if (g_status == STATUS_RX) {

			g_status = STATUS_IDLE;

			NRF_RADIO->TASKS_STOP = 1U;
			NRF_RADIO->TASKS_DISABLE = 1U;

			// Refresh values
			g_env.batt = adc_bat(false);
			g_env.temp = temp_get(false);
			g_env.seq += 1;

			store_get(&g_env);

			// Encrypt packet
			if (aes_encr(&g_env, &g_enc, sizeof(g_env), SIGNATURE_SIZE) == 0) {

				printf_env(&g_env);

				g_status = STATUS_TX;
				g_tx_numb = TX_REPEAT;
				NRF_RADIO->TASKS_TXEN = 1;

			} else {

				printf("AES encryption error!\r\n");

			}

		}

	}

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
 * Radio interrupt handler
 */
void RADIO_IRQHandler(void) {
	if (NRF_RADIO->EVENTS_END) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_END = 0;

		if (g_status == STATUS_RX) {

			// Handle package
			if (NRF_RADIO->CRCSTATUS == 1U) {

				uint8_t aes_result = aes_decr(&g_enc, &g_rec, sizeof(g_rec), SIGNATURE_SIZE);
				if (aes_result == 0) {

					// Read RSSI
					while (NRF_RADIO->EVENTS_RSSIEND == 0U);
					int8_t rssi = (int8_t) NRF_RADIO->RSSISAMPLE;
					NRF_RADIO->TASKS_RSSISTOP = 1;

					printf_env(&g_rec);

					store_save(&g_rec, rssi);

				} else {

					printf("AES unpack error: %u!\r\n", aes_result);

				}

			} else {

				printf("Bad CRC!\r\n");

			}

		}

	}

	if (NRF_RADIO->EVENTS_DISABLED) {

		/* acknowledge event */
		NRF_RADIO->EVENTS_DISABLED = 0;

		if (g_status != STATUS_IDLE) {

			bool was_tx = false;

			// Sending few times
			if (g_status == STATUS_TX) {

				was_tx = true;

				g_tx_numb -= 1;
				if (g_tx_numb > 0) {

					// Send again!
					NRF_RADIO->TASKS_TXEN = 1U;
					return;

				}

			}

			// Listening
			g_status = STATUS_RX;
			NRF_RADIO->TASKS_RXEN = 1U;

			if (was_tx) {

				NRF_RTC0->CC[0] = NRF_RTC0->COUNTER + MILLISECONDS(TX_RATE_MS);

			}

		}
	}

}

int main(void) {

	// Main init
    uint32_t oid = init();

    store_init();

	// Initialize values
    g_env.proto = RFPROTO_PROXY;
    g_env.oid   = oid;
	g_env.batt  = adc_bat(true);
	g_env.temp  = temp_get(true);
	g_env.seq   = 0;

	// Initialize radio

    NRF_RADIO->SHORTS = (RADIO_SHORTS_ADDRESS_RSSISTART_Enabled << RADIO_SHORTS_ADDRESS_RSSISTART_Pos) |
    					(RADIO_SHORTS_READY_START_Enabled << RADIO_SHORTS_READY_START_Pos) | // Shortcut between READY event and START task
                        (RADIO_SHORTS_END_DISABLE_Enabled << RADIO_SHORTS_END_DISABLE_Pos);  // Shortcut between END event and DISABLE task


	NRF_RADIO->INTENSET = (RADIO_INTENSET_END_Enabled << RADIO_INTENSET_END_Pos) |
						  (RADIO_INTENSET_DISABLED_Enabled << RADIO_INTENSET_DISABLED_Pos);

	NVIC_SetPriority(RADIO_IRQn, IRQ_PRIORITY_RADIO);
	NVIC_ClearPendingIRQ(RADIO_IRQn);
	NVIC_EnableIRQ(RADIO_IRQn);

	NRF_RADIO->PACKETPTR = (uint32_t) &g_enc;

	// Start timer
	NRF_RTC0->CC[0] = SECONDS(1UL);
	NRF_RTC0->CC[1] = SECONDS(10UL);
	NRF_RTC0->TASKS_START = 1;

	// Start listening radio
	NRF_RADIO->TASKS_RXEN = 1U;

	blink_loop(LED_RGB_GREEN);

}
