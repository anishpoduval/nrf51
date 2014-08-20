/*
 * timer.c
 *
 *  Created on: Aug 18, 2014
 *      Author: valdo
 */

#include "nrf_delay.h"
#include "timer.h"

#define LFCLK_FREQUENCY (32768UL) /*!< LFCLK frequency in Hertz, constant */
#define RTC_FREQUENCY (8UL) /*!< required RTC working clock RTC_FREQUENCY Hertz. Changable */
#define COMPARE_COUNTERTIME (8UL) /*!< Get Compare event COMPARE_TIME seconds after the counter starts from 0 */
#define COUNTER_PRESCALER ((LFCLK_FREQUENCY/RTC_FREQUENCY) - 1UL) /* f = LFCLK/(prescaler + 1) */

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

	// Set prescaler to a TICK of RTC_FREQUENCY
	NRF_RTC0->PRESCALER = 0; //COUNTER_PRESCALER;
	// Set compare0 frequency
	NRF_RTC0->CC[0] = 5UL * LFCLK_FREQUENCY;
	// Enable events
	NRF_RTC0->EVTENSET = RTC_EVTEN_COMPARE0_Msk;
	NRF_RTC0->INTENSET = RTC_INTENSET_COMPARE0_Msk;
	// Start RTC0
	NRF_RTC0->TASKS_START = 1;

}
