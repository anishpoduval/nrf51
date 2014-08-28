#include "config.h"
#include "timer.h"
#include "adc.h"
#include "proto.h"
#include "radio.h"

void RTC0_IRQHandler(void) {
	// Radio send events
	 if ((NRF_RTC0->EVENTS_COMPARE[0] != 0) &&
		((NRF_RTC0->INTENSET & RTC_INTENSET_COMPARE0_Msk) != 0)) {

		 NRF_RTC0->EVENTS_COMPARE[0] = 0;

		 // Get (maybe) new battery level
		 g_env.batt = adc_bat(false);

		 // Inc sequence
		 g_env.data.tracker.seq += 1;

		 // Setup loop after
		 NRF_RTC0->CC[0] = NRF_RTC0->COUNTER + MILLISECONDS(500UL);

	}
}

