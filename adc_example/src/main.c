#include <stdbool.h>
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "adc.h"

int main(void) {

	gpio_init();
	uart_init();
	timer_init();
	adc_init();

	while (true);

}
