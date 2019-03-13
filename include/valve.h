#ifndef VALVE_H
#define VALVE_H

#include "storage.h"

/*
 * Initializes the valves by setting all pins to output
 */
void valves_init();

void test_on_off();

/*
 * Turns on all valves for a given time according to concentration
 */
 void turn_on_valves();



#endif
