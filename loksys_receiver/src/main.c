#include <stdbool.h>
#include <stdint.h>
#include "proto.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "crc32.h"
#include "uart.h"
#include "radio.h"
#include "adc.h"
#include "timer.h"

ProtoEnvelope env_rx, env_tx;

int main(void) {

	uint8_t tagno = 0;

    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

    timer_init();
    adc_init();

    /* Wait for voltage measurement to finish */
    while (batt_mv == 0);

    // Read and set Tag ID
	env_tx.oid = crc32(&NRF_FICR->DEVICEID, sizeof(NRF_FICR->DEVICEID));
	env_tx.proto = RFPROTO_PROXY;
	env_tx.batt = batt_mv;

	uart_init(TX_PIN_NUMBER, RX_PIN_NUMBER);
	radio_init();

	// Start random number generator
	NRF_RNG->TASKS_START = 1;

	while (true) {

		NRF_RADIO->PACKETPTR = (uint32_t) &env_rx;

		// Enable radio
		NRF_RADIO->EVENTS_READY = 0U;
		NRF_RADIO->TASKS_RXEN = 1U;

		while (NRF_RADIO->EVENTS_READY == 0U);

		NRF_RADIO->TASKS_START = 1U;
		NRF_RADIO->EVENTS_END = 0U;
		while (NRF_RADIO->EVENTS_END == 0U);

		if (NRF_RADIO->CRCSTATUS == 1U) {
			switch (env_rx.proto) {
				case RFPROTO_TRACK:
					break;
				case RFPROTO_PROXY:
					break;
			}
		}

	}

	NRF_RADIO->EVENTS_DISABLED = 0U;

	// Disable radio
	NRF_RADIO->TASKS_DISABLE = 1U;
	while (NRF_RADIO->EVENTS_DISABLED == 0U);

}
