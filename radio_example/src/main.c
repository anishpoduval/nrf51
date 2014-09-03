#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "radio.h"
#include "uart.h"
#include "aes.h"
#include "crc32.h"

static Packet p;
static uint8_t enc[sizeof(p)];

int main(void) {

    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

    nrf_gpio_cfg_output(LED_RGB_GREEN);
    nrf_gpio_pin_set(LED_RGB_GREEN);

    uart_init(TX_PIN_NUMBER, RX_PIN_NUMBER);
	radio_init();

	uint32_t oid = crc32(&NRF_FICR->DEVICEID, sizeof(NRF_FICR->DEVICEID));
	aes_init(oid);

    // Set payload pointer
    NRF_RADIO->PACKETPTR = (uint32_t) &enc;

	while (true) {

		//nrf_gpio_pin_set(LED_RGB_BLUE);

		//radio_send();

		//uart_putstring((const uint8_t *) "Sent ");
		//uart_put(packet[0]);
		//uart_putstring("\n\r");

		//nrf_gpio_pin_clear(LED_RGB_BLUE);
		//nrf_delay_us(1000000);

		//nrf_gpio_pin_set(LED_RGB_RED);

		if (radio_receive()) {
			if (aes_decr(&enc, &p, sizeof(p), SIGNATURE_SIZE) == 0) {
				printf("Received: seq = %u, rand = %u, batt = %u, rssi = %u\n\r", p.seq, p.oid, p.batt, NRF_RADIO->RSSISAMPLE);
			}
		} else {
			printf("Bad CRC");
		}

		//nrf_gpio_pin_clear(LED_RGB_RED);
		//nrf_delay_us(1000000);

		nrf_gpio_pin_clear(LED_RGB_GREEN);
		nrf_delay_ms(5);
		nrf_gpio_pin_set(LED_RGB_GREEN);

	}
}
