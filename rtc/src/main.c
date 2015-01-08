#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include <stdio.h>

extern void uart_init(void);

#define LF_FREQUENCY 32768UL
#define SECONDS(x) ((uint32_t)((LF_FREQUENCY * x) + 0.5))

static volatile bool g_timer_wait = false;

void timer_wait(const uint32_t ticks) {

	g_timer_wait = true;

	uint32_t cc = NRF_RTC1->COUNTER + ticks + 5;
	printf("COUNTER = %u, CC = %u\n\r", NRF_RTC1->COUNTER, cc);
	NRF_RTC1->CC[0] = cc;

	/* start timer and wait */
	while (g_timer_wait) __WFI();
}

void RTC1_IRQHandler(void) {

	/* acknowledge interrupt */
	NRF_RTC1->EVENTS_COMPARE[0] = 0U;

	/* allow wait loop to exit */
	g_timer_wait = false;

}

void RTC0_IRQHandler(void) {

	if (NRF_RTC0->EVENTS_COMPARE[0]) {
		NRF_RTC0->EVENTS_COMPARE[0] = 0;

	    nrf_gpio_pin_clear(LED_RGB_BLUE);
	    nrf_gpio_pin_set(LED_RGB_RED);
	    timer_wait(SECONDS(1));
	    nrf_gpio_pin_clear(LED_RGB_RED);
	    nrf_gpio_pin_set(LED_RGB_BLUE);

	    NRF_RTC0->CC[0] = NRF_RTC0->COUNTER + SECONDS(1);

	}

}

int main(void)
{

	nrf_gpio_cfg_output(LED_RGB_BLUE);
	nrf_gpio_cfg_output(LED_RGB_RED);

	uart_init();

    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

	/* start 32kHz crystal oscillator */
	NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRCCOPY_SRC_Xtal << CLOCK_LFCLKSRCCOPY_SRC_Pos);
	NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_LFCLKSTART = 1;
	while (!NRF_CLOCK->EVENTS_LFCLKSTARTED);

	/* setup delay routine */
	NRF_RTC0->TASKS_STOP = 1;
	//NRF_RTC0->COUNTER = 0;
	NRF_RTC0->PRESCALER = 0;
	NRF_RTC0->INTENSET = (RTC_INTENSET_COMPARE0_Enabled << RTC_INTENSET_COMPARE0_Pos) |
						 (RTC_INTENSET_COMPARE1_Enabled << RTC_INTENSET_COMPARE1_Pos) |
						 (RTC_INTENSET_COMPARE2_Enabled << RTC_INTENSET_COMPARE2_Pos) |
						 (RTC_INTENSET_COMPARE3_Enabled << RTC_INTENSET_COMPARE3_Pos);

	NRF_RTC0->CC[0] = SECONDS(1);
	NRF_RTC0->CC[1] = 0;
	NRF_RTC0->CC[2] = 0;
	NRF_RTC0->CC[3] = 0;

	NVIC_SetPriority(RTC0_IRQn, 1);
	NVIC_EnableIRQ(RTC0_IRQn);

	/* setup delay routine */

	//NRF_RTC1->COUNTER = 0;
	NRF_RTC1->TASKS_STOP = 1;
	NRF_RTC1->PRESCALER = 0;
	NRF_RTC1->CC[0] = 0;

	NRF_RTC1->INTENSET = (RTC_INTENSET_COMPARE0_Enabled << RTC_INTENSET_COMPARE0_Pos);

	NVIC_SetPriority(RTC1_IRQn, 1);
	NVIC_EnableIRQ(RTC1_IRQn);

	NRF_RTC0->TASKS_START = 1;
	NRF_RTC1->TASKS_START = 1;

  
  while(true)
  {

	  /*
	    nrf_gpio_pin_clear(LED_RGB_BLUE);
	    nrf_gpio_pin_set(LED_RGB_RED);
	    timer_wait(SECONDS(1));
	    nrf_gpio_pin_clear(LED_RGB_RED);
	    nrf_gpio_pin_set(LED_RGB_BLUE);
	    timer_wait(SECONDS(1));
	    */

  }
}
