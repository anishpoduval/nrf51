#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
//#include "simple_uart.h"

int main(void)
{
  // flash red LED and blue LED alternately
  nrf_gpio_cfg_output(18);
  //nrf_gpio_cfg_output(LED_RED);
  
  //simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, true);

  while(true)
  {

	//simple_uart_put(8);

    nrf_gpio_pin_clear(18);
    //nrf_gpio_pin_set(LED_BLUE);
    nrf_delay_us(1000000);
    //nrf_gpio_pin_clear(LED_BLUE);
    nrf_gpio_pin_set(18);
    nrf_delay_us(1000000);
  }
}
