/*
 * timer.h
 *
 *  Created on: Aug 24, 2014
 *      Author: valdo
 */

#ifndef TIMER_H_
#define TIMER_H_

#define LFCLK_FREQUENCY (32768UL) /*!< LFCLK frequency in Hertz, constant */
#define RTC_FREQUENCY (8UL) /*!< required RTC working clock RTC_FREQUENCY Hertz. Changable */
#define COMPARE_COUNTERTIME (8UL) /*!< Get Compare event COMPARE_TIME seconds after the counter starts from 0 */
#define COUNTER_PRESCALER ((LFCLK_FREQUENCY/RTC_FREQUENCY) - 1UL) /* f = LFCLK/(prescaler + 1) */

#define TICKS_30SECS (30UL * LFCLK_FREQUENCY)

void timer_init(void);

#endif /* TIMER_H_ */
