/*
 * timer.c
 *
 *  Created on: Aug 24, 2014
 *      Author: valdo
 */

#include "nrf_delay.h"
#include "boards.h"
#include "nrf51.h"
#include "timer.h"

void timer_init(void) {

	/** Starts the internal LFCLK XTAL oscillator */

	// Place external 32KiHz crystal as source
	NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
	// Reset clock events
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
	// Start oscillator source
	NRF_CLOCK->TASKS_LFCLKSTART = 1;
	// Wait for oscillator to be started
	while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);
	// Reset clock events
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;

	/** Configures the RTC */

	// Set prescaler
	NRF_RTC0->PRESCALER = 0;

	// Voltage measurement on CC[0] after TICKS
	NRF_RTC0->CC[0] = LFCLK_FREQUENCY / 4UL;

	// Enable events
	NRF_RTC0->EVTENSET = RTC_EVTEN_COMPARE0_Msk;
	NRF_RTC0->INTENSET = RTC_INTENSET_COMPARE0_Msk;

	// Start RTC0
	NRF_RTC0->TASKS_START = 1;

}
