/*
 * timer.c
 *
 *  Created on: Aug 18, 2014
 *      Author: valdo
 */

#include "nrf_delay.h"
#include "timer.h"

#define LFCLK_FREQUENCY (32768UL) /*!< LFCLK frequency in Hertz, constant */
#define RTC_FREQUENCY (1UL) /*!< required RTC working clock RTC_FREQUENCY Hertz. Changable */
#define COMPARE_COUNTERTIME (60UL) /*!< Get Compare event COMPARE_TIME seconds after the counter starts from 0 */
#define COUNTER_PRESCALER ((LFCLK_FREQUENCY/RTC_FREQUENCY) - 1) /* f = LFCLK/(prescaler + 1) */

void timer_init(void) {

	/** Starts the internal LFCLK XTAL oscillator */

	NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_LFCLKSTART = 1;
	while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;

	/** Configures the RTC */

	NRF_RTC0->TASKS_START = 0; // Stop RTC0
	NRF_RTC0->PRESCALER = COUNTER_PRESCALER; // Set prescaler to a TICK of RTC_FREQUENCY
	NRF_RTC0->EVTENSET = RTC_EVTENSET_TICK_Msk; // Enable TICK event
	NRF_RTC0->TASKS_START = 1; // Start RTC0

}
