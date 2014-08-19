/*
 * adc.c
 *
 *  Created on: Aug 19, 2014
 *      Author: valdo
 */

#include "nrf51.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "adc.h"
#include "uart.h"
#include "gpio.h"

#define 	ADC_PRE_SCALING_COMPENSATION   3
#define     ADC_REF_VOLTAGE_IN_MILLIVOLTS   1200
#define     ADC_RESULT_IN_MILLI_VOLTS(ADC_VALUE) 	   ((((ADC_VALUE) * ADC_REF_VOLTAGE_IN_MILLIVOLTS) / 255) * ADC_PRE_SCALING_COMPENSATION)

void adc_init(void) {

	/* Enable interrupt on ADC sample ready event*/
	NRF_ADC->INTENSET = ADC_INTENSET_END_Msk;
	NVIC_EnableIRQ(ADC_IRQn);

	// config ADC
	NRF_ADC->CONFIG = (ADC_CONFIG_EXTREFSEL_None << ADC_CONFIG_EXTREFSEL_Pos) /* Bits 17..16 : ADC external reference pin selection. */
	| (ADC_CONFIG_PSEL_AnalogInput6 << ADC_CONFIG_PSEL_Pos) /*!< Use analog input 6 as analog input (P0.05). */
	| (ADC_CONFIG_REFSEL_VBG << ADC_CONFIG_REFSEL_Pos) /*!< Use internal 1.2V bandgap voltage as reference for conversion. */
	| (ADC_CONFIG_INPSEL_AnalogInputNoPrescaling << ADC_CONFIG_INPSEL_Pos) /*!< Analog input specified by PSEL with no prescaling used as input for the conversion. */
	| (ADC_CONFIG_RES_8bit << ADC_CONFIG_RES_Pos); /*!< 8bit ADC resolution. */

	/* Enable ADC*/
	NRF_ADC->ENABLE = ADC_ENABLE_ENABLE_Enabled;

	// Configure PPI channel 0 to start ADC task
	NRF_PPI->CH[0].EEP = (uint32_t) &NRF_RTC0->EVENTS_TICK;
	NRF_PPI->CH[0].TEP = (uint32_t) &NRF_ADC->TASKS_START;

	// Enable PPI channel 0
	NRF_PPI->CHEN = (PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos);

}

/**@brief ADC interrupt handler.
 * @details  This function will fetch the conversion result from the ADC, convert the value into mV
  */
void ADC_IRQHandler(void) {
	if (NRF_ADC->EVENTS_END != 0) {
		NRF_ADC->EVENTS_END = 0;

		batt_lvl_in_milli_volts = ADC_RESULT_IN_MILLI_VOLTS(NRF_ADC->RESULT); //+ DIODE_FWD_VOLT_DROP_MILLIVOLTS;

		dump_uint16(&batt_lvl_in_milli_volts);
		blink_red();

		NRF_ADC->TASKS_STOP = 1;
	}
}
