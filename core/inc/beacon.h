/*
 * beacon.h
 *
 *  Created on: Aug 27, 2014
 *      Author: valdo
 */

/**
 * SWD P2 cabel pins
 *
 * 1 (VCC)    - black
 * 2 (SWDIO)  - blue
 * 3 (PIN 9)  - green
 * 4 (SWDCLK) - red
 * 5 (PIN 11) - white
 * 6 (GND)    - yellow
 *
 */

#ifndef BEACON_H_
#define BEACON_H_

#define LED_RGB_RED    16
#define LED_RGB_GREEN  12
#define LED_RGB_BLUE   15

#define GPIO_PIN_00    00
#define GPIO_PIN_01    01
#define GPIO_PIN_02    02
#define GPIO_PIN_03    03
#define GPIO_PIN_05    05
#define GPIO_PIN_09    09
#define GPIO_PIN_11    11
#define GPIO_PIN_20    20
#define GPIO_PIN_21    21
#define GPIO_PIN_24    24

#define RX_PIN_NUMBER  11
#define TX_PIN_NUMBER  9
#define CTS_PIN_NUMBER 10
#define RTS_PIN_NUMBER 8
#define HWFC           true

#endif /* BEACON_H_ */
