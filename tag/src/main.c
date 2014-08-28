#include "config.h"
#include "adc.h"
#include "uart.h"
#include "timer.h"

int main(void) {

	timer_init();
	uart_init();
	adc_init();

	printf("Labas!\n");

	// flash red LED and blue LED alternately
	//nrf_gpio_cfg_output(LED_RGB_RED);
	//nrf_gpio_cfg_output(LED_RGB_BLUE);

	while (true) {
		//nrf_gpio_pin_clear(LED_RGB_RED);
		//nrf_gpio_pin_set(LED_RGB_BLUE);
		//nrf_delay_us(1000000);
		//nrf_gpio_pin_clear(LED_RGB_BLUE);
		//nrf_gpio_pin_set(LED_RGB_RED);

		adc_start();
		uint8_t bat = adc_bat();
		printf("%d\n\r", bat);

		nrf_delay_us(1000000);
	}
}
