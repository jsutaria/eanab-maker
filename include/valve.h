#ifndef VALVE_H
#define VALVE_H

#define VALVE_1 GPIO_PIN4
#define VALVE_2 GPIO_PIN5
#define VALVE_3 GPIO_PIN6
#define VALVE_4 GPIO_PIN7

#include "storage.h"

/*
 * Initializes the valves by setting all pins to output
 */
void valves_init();

void test_on_off();

/*
 * Turns on all valves for a given time according to concentration
 */
void turn_on_valves(int time_1, int time_2, int time_3, int time_4);

#endif
