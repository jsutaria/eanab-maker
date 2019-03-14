#ifndef BUTTON_H
#define BUTTON_H

void button_init(unsigned int data_pin);

void make_sound();

unsigned int detect_press();

void wait_for_press();

#endif
