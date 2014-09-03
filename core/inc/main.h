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

void blink_loop(uint8_t pin);
void blink(uint8_t pin, uint8_t times);
uint32_t init();

void printf_env(const ProtoEnvelope *e);

#endif /* MAIN_H_ */
