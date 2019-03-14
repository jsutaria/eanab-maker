#ifndef VALVE_H
#define VALVE_H

#define ON  1
#define OFF 0

#define VALVE_1 0
#define VALVE_2 1
#define VALVE_3 2
#define VALVE_4 3

#define VALVE_1_PIN GPIO_PIN4
#define VALVE_2_PIN GPIO_PIN5
#define VALVE_3_PIN GPIO_PIN6
#define VALVE_4_PIN GPIO_PIN7

/*
 * Initializes the valves by setting all pins to output
 */
void valves_init(unsigned int pin_1, unsigned int pin_2, unsigned int pin_3, unsigned int pin_4);
/*
 * Turns on all valves for a given time according to concentration
 */

void valve_on(int valve);
void valve_off(int valve);

void all_valves_on(void);
void all_valves_off(void);

void turn_on_valves(int time_1, int time_2, int time_3, int time_4);

void test_on_off(void);

#endif
