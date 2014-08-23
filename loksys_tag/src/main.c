#include <stdbool.h>
#include <stdint.h>
#include "proto.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "crc32.h"
#include "uart.h"
#include "radio.h"

ProtoEnvelope env;

int main(void) {

	uint32_t seq = 0;

    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

    // Read and set Tag ID
	env.oid = crc32(&NRF_FICR->DEVICEID, sizeof(NRF_FICR->DEVICEID));
	env.proto = RFPROTO_TRACK;
	env.batt = 0;

	uart_init(TX_PIN_NUMBER, RX_PIN_NUMBER);
	radio_init(&env.oid);

	NRF_RADIO->PACKETPTR = (uint32_t) &env;

	while (true) {

	}

}
