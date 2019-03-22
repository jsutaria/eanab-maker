#include "button.h"
#include "gpio.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

#define BUTTON_HOLD 500
unsigned int button_pin;

/**
 * Initialize the button
 * @param data_pin the GPIO pin in which the button is connected to
 */
void button_init(unsigned int data_pin) {
  button_pin = data_pin;
  gpio_set_input(button_pin);
}

/**
 * Make's a sound using the UART
 */
void make_sound() {
  uart_putchar('\a');
}

/**
 * Detect whether or not the button has been pressed
 * @return a boolean indicating pressed or not
 */
unsigned int detect_press() {
  if (gpio_read(button_pin) == 1) {
    timer_delay_ms(BUTTON_HOLD);
    if (gpio_read(button_pin) == 1) {
      printf("Pressed!\n");
      make_sound();
      return 1;
    }
  }
  return 0;
}

/**
 * Loops until the button has been pressed
 */
void wait_for_press() {
  while (detect_press() == 0) { }
}
