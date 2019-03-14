#ifndef BREATHALYZER_H
#define BREATHALYZER_H

#include "gpio.h"
#include "printf.h"

void button_init(unsigned int data_pin);

void make_sound();

unsigned int detect_press();

void wait_for_press();

#endif
