#include "config.h"
#include "proto.h"
#include "main.h"

int main(void) {

	init(RFPROTO_TRACK);

	// Start timer
	NRF_RTC0->CC[0] = MILLISECONDS(100UL);
	NRF_RTC0->CC[1] = SECONDS(1UL);

	NRF_RTC0->TASKS_START = 1;

	blink_loop(LED_RGB_GREEN);

}
