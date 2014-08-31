/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#ifndef PCA20006_H
#define PCA20006_H
 
#include "nrf_gpio.h"
 
#define LED_RED    16
#define LED_GREEN  12
#define LED_BLUE   15

#define BUTTON_0       8
#define BUTTON_1       18
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

#define RX_PIN_NUMBER  24
#define TX_PIN_NUMBER  9
#define CTS_PIN_NUMBER 21
#define RTS_PIN_NUMBER 11
#define HWFC           true

#endif
