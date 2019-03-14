#include "ps2_helpers.h"
#include "gpio.h"
#include "timer.h"
#include "gpioextra.h"

static unsigned int writing_data_line = 0;

void ps2_write(unsigned int clock_gpio, unsigned int data_gpio, unsigned char data)
{
    // Mark line as writing so we disregard interrupts
    writing_data_line = data_gpio;

    // Bring clock line low
    gpio_set_function(clock_gpio, GPIO_FUNC_OUTPUT);
    gpio_write(clock_gpio, 0);
    timer_delay_us(100);

    // Bring data line to low
    gpio_set_function(data_gpio, GPIO_FUNC_OUTPUT);
    gpio_write(data_gpio, 0);

    // Release clock line
    gpio_set_function(clock_gpio, GPIO_FUNC_INPUT);
    gpio_set_pullup(clock_gpio);

    // Wait for device to bring clock line low
    while(gpio_read(clock_gpio) == 1);

    // For each bit, set data line and wait for clock edge to fall
    // before sending the next bit.
    int sum = 0;
    for (int i = 0; i < 9; i++) {
      gpio_write(data_gpio, (i < 8) ? (data >> i) & 1 : (sum % 2) == 0);
      while (gpio_read(clock_gpio) == 0);
      while (gpio_read(clock_gpio) == 1);
      sum += (data >> i) & 1;
    }

    // RRelease data line
    gpio_set_function(data_gpio, GPIO_FUNC_INPUT);
    gpio_set_pullup(data_gpio);

    // Wait for the device to bring both lines low again
    while(gpio_read(data_gpio) == 1);
    while(gpio_read(clock_gpio) == 1);

    // Clear writing market
    writing_data_line = 0;
}
