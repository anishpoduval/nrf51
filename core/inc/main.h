/*
 * main.h
 *
 *  Created on: Aug 29, 2014
 *      Author: valdo
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "config.h"
#include "proto.h"
#include "adc.h"
#include "aes.h"
#include "rng.h"
#include "temp.h"
#include "timer.h"
#include "radio.h"

ProtoEnvelope g_env ALIGN4;
uint8_t g_enc[sizeof(g_env)] ALIGN4;

void blink_loop(uint8_t pin);
void blink(uint8_t pin, uint8_t times);
void init(uint8_t proto);

#ifdef BOARD_PCA10000

void printf_env(ProtoEnvelope *e);

#endif

#endif /* MAIN_H_ */
