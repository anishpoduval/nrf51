/*
 * config.h
 *
 *  Created on: Aug 27, 2014
 *      Author: valdo
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <nrf.h>
#include <nrf51.h>
#include <nrf_gpio.h>
#include <nrf_delay.h>

#if defined(BOARD_PCA10000)
  #include "pca10000.h"
#elif defined(BOARD_PCA20006)
  #include "pca20006.h"
#else
#error "Board is not defined"
#endif


#define PACKED __attribute__((packed))
#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))
#define LINKTO(f) __attribute__ ((alias (#f)))
#define ALIGN4 __attribute__ ((aligned (4)))

static inline uint16_t
htons (uint16_t x)
{
  __asm__ ("rev16 %0, %1": "=r" (x):"r" (x));
	return x;
}

static inline uint32_t
htonl (uint32_t x)
{
  __asm__ ("rev %0, %1": "=r" (x):"r" (x));
	return x;
}

#define ntohl(l) htonl(l)
#define ntohs(s) htons(s)

#define BIT_REVERSE(x) ((unsigned char)(__RBIT(x)>>24))

#include "crc8.h"
#include "crc16.h"
#include "crc32.h"
#include "xxtea.h"
#include "uart.h"
#include "error.h"

/* only two priority bits available ! */

#define IRQ_PRIORITY_HIGH        0
#define IRQ_PRIORITY_AES         (IRQ_PRIORITY_HIGH)
#define IRQ_PRIORITY_RNG         (IRQ_PRIORITY_HIGH+1)
#define IRQ_PRIORITY_POWER_CLOCK (IRQ_PRIORITY_HIGH+1)
#define IRQ_PRIORITY_RADIO       (IRQ_PRIORITY_HIGH+2)

#define IRQ_PRIORITY_LOW         (IRQ_PRIORITY_HIGH+3)
#define IRQ_PRIORITY_RTC0        (IRQ_PRIORITY_LOW)
#define IRQ_PRIORITY_RTC1        (IRQ_PRIORITY_LOW)
#define IRQ_PRIORITY_ADC         (IRQ_PRIORITY_LOW)
#define IRQ_PRIORITY_UART0       (IRQ_PRIORITY_LOW)
#define IRQ_PRIORITY_TEMP        (IRQ_PRIORITY_LOW)

#endif /* CONFIG_H_ */
