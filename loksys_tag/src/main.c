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

ProtoEnvelope env;

int main(void) {

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
	env.oid = crc32(&NRF_FICR->DEVICEID, sizeof(NRF_FICR->DEVICEID));
	env.proto = RFPROTO_TRACK;
	env.batt = batt_mv;
	env.data.tracker.seq = 0;

	uart_init(TX_PIN_NUMBER, RX_PIN_NUMBER);
	radio_init();

	NRF_RADIO->PACKETPTR = (uint32_t) &env;

	// Enable radio
	NRF_RADIO->EVENTS_READY = 0U;
	NRF_RADIO->TASKS_TXEN = 1;

	// Start random number generator
	NRF_RNG->TASKS_START = 1;

	while (true) {

		env.data.tracker.seq += 1;
		env.batt = batt_mv;

		while (NRF_RADIO->EVENTS_READY == 0U);

		NRF_RADIO->TASKS_START = 1U;
		NRF_RADIO->EVENTS_END = 0U;
		while (NRF_RADIO->EVENTS_END == 0U);

		nrf_delay_ms(2UL * (uint8_t) NRF_RNG->VALUE);

	}

	NRF_RADIO->EVENTS_DISABLED = 0U;

	// Disable radio
	NRF_RADIO->TASKS_DISABLE = 1U;
	while (NRF_RADIO->EVENTS_DISABLED == 0U);

}
