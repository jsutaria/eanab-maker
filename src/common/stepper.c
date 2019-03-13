#include "stepper.h"
#include "gpio.h"
#include "timer.h"
#include "timer.h"

static unsigned int pin_1, pin_2, pin_3, pin_4;
static int states[8][4] = {{1, 0, 0, 0},
                           {1, 0, 1, 0},
                           {0, 0, 1, 0},
                           {0, 1, 1, 0},
                           {0, 1, 0, 0},
                           {0, 1, 0, 1},
                           {0, 0, 0, 1},
                           {1, 0, 0, 1}};

void stepper_init(unsigned int pin1, unsigned int pin2, unsigned int pin3, unsigned int pin4) {
    pin_1 = pin1;
    pin_2 = pin2;
    pin_3 = pin3;
    pin_4 = pin4;
    gpio_set_output(pin1);
    gpio_set_output(pin2);
    gpio_set_output(pin3);
    gpio_set_output(pin4);
}

void stepper_stop(void) {
  gpio_write(pin_1, 0);
  gpio_write(pin_2, 0);
  gpio_write(pin_3, 0);
  gpio_write(pin_4, 0);
  timer_delay_us(100);
}

void stepper_goto_position(unsigned int position) {
  gpio_write(pin_1, states[position][0]);
  gpio_write(pin_2, states[position][1]);
  gpio_write(pin_3, states[position][2]);
  gpio_write(pin_4, states[position][3]);
  timer_delay_ms(1);
}

void stepper_test(void) {
    while(1) {
        for(int i = 0; i < 8; i++) {
          stepper_goto_position(i);
        }
    }
}
