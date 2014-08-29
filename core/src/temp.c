/*
 * temp.c
 *
 *  Created on: Aug 29, 2014
 *      Author: valdo
 */

#include "config.h"
#include "temp.h"

static volatile uint8_t g_temp;
bool temp_wait = false;

void TEMP_IRQHandler(void) {
	if (NRF_TEMP->EVENTS_DATARDY) {

		/* acknowledge event */
		NRF_TEMP->EVENTS_DATARDY = 0;

		/* read temperature */
		g_temp = NRF_TEMP->TEMP / 4;

		/* disable ADC after sampling voltage */
		NRF_TEMP->TASKS_STOP = 1;

		temp_wait = false;
	}
}

void temp_start(void) {
	/* start temperature reading */
	NRF_TEMP->TASKS_START = 1;
	temp_wait = true;
}

uint8_t temp_get(bool wait) {
	if (wait) {
		while (temp_wait)
			__WFE();
	}
	return g_temp;
}

void temp_init(void) {
	/* initialize temperature */
	g_temp = 0;

	/* setup temperature metering interrupt */
	NRF_TEMP->INTENSET = ((TEMP_INTENSET_DATARDY_Enabled << TEMP_INTENSET_DATARDY_Pos));

	NVIC_SetPriority(TEMP_IRQn, IRQ_PRIORITY_TEMP);
	NVIC_EnableIRQ(TEMP_IRQn);
}
