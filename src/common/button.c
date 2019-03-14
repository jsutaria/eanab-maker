#include "gpio.h"
#include "printf.h"
#include "button.h"

#define BUTTON_HOLD 500
unsigned int button_pin;

void button_init(unsigned int data_pin) {
  button_pin = data_pin;
  gpio_set_input(button_pin);
}

void make_sound() {
  uart_putchar('\a');
}

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

void wait_for_press() {
  while (detect_press() == 0) { }
}
