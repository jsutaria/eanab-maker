#include "stepper.h"

static unsigned int pin_1, pin_2, pin_3, pin_4;

void stepper_init(unsigned int pin1, unsigned int pin2, unsigned int pin3, unsigned int pin4) {
    pin_1 = pin1;
    pin_2 = pin2;
    pin_3 = pin3;
    pin_4 = pin4;
}

void stepper_test(void) {
    while(1);
}
