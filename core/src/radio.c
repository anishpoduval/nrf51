#include "config.h"
#include "main.h"

void radio_start(uint32_t pointer) {
	NRF_RADIO->PACKETPTR = pointer;
	NRF_RADIO->TASKS_START = 1U;
}

void radio_stop(void) {
	NRF_RADIO->TASKS_STOP = 1;
	NRF_RADIO->TASKS_DISABLE = 1;
}

void radio_init_rx(void) {

	NRF_RADIO->RXADDRESSES = RADIO_RXADDRESSES;
	NRF_RADIO->PCNF1 = RADIO_RX_PCNF1;

	NRF_RADIO->TASKS_DISABLE = 0;
	NRF_RADIO->EVENTS_READY = 0U;
	NRF_RADIO->TASKS_RXEN = 1;

}

void radio_init_tx(void) {

	NRF_RADIO->TXPOWER = RADIO_TXPOWER;
	NRF_RADIO->TXADDRESS = RADIO_TXADDRESS;
	NRF_RADIO->PCNF1 = RADIO_TX_PCNF1;

	NRF_RADIO->TASKS_DISABLE = 0;
	NRF_RADIO->EVENTS_READY = 0U;
	NRF_RADIO->TASKS_TXEN = 1;

}

void radio_init(void) {

	/* setup default radio settings for track mode */
	NRF_RADIO->MODE = RADIO_MODE_MODE_Nrf_2Mbit << RADIO_MODE_MODE_Pos;
	NRF_RADIO->FREQUENCY = RADIO_CHANNEL;
	NRF_RADIO->TXPOWER = RADIO_TXPOWER;
	NRF_RADIO->PCNF1 = RADIO_TX_PCNF1;

	/* generic radio setup */
	NRF_RADIO->PREFIX0 = 0x46D7UL;
	NRF_RADIO->BASE0 = 0xEA8AF0B1UL;
	NRF_RADIO->BASE1 = 0xCC864569UL;
	NRF_RADIO->RXADDRESSES = 1;
	NRF_RADIO->PCNF0 = 0x0;
	NRF_RADIO->CRCCNF = (RADIO_CRCCNF_LEN_One << RADIO_CRCCNF_LEN_Pos);
	NRF_RADIO->CRCINIT = 0xFFUL;
	NRF_RADIO->CRCPOLY = 0x107UL;

	NRF_RADIO->INTENSET = ((RADIO_INTENSET_END_Enabled << RADIO_INTENSET_END_Pos) |
						   (RADIO_INTENSET_DISABLED_Enabled << RADIO_INTENSET_DISABLED_Pos) |
						   (RADIO_INTENSET_READY_Enabled << RADIO_INTENSET_READY_Pos) |
						   (RADIO_INTENSET_ADDRESS_Enabled << RADIO_INTENSET_ADDRESS_Pos));

	NVIC_SetPriority(RADIO_IRQn, IRQ_PRIORITY_RADIO);
	NVIC_ClearPendingIRQ(RADIO_IRQn);
	NVIC_EnableIRQ(RADIO_IRQn);

}
