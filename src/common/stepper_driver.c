#include "stepper_driver.h"
#include "gpio.h"
#include "timer.h"

#define NUM_ITERATIONS 4
#define NUM_GPIO_PINS 4

static unsigned int inputArray[NUM_GPIO_PINS];

void stepper_init(unsigned int pin_1, unsigned int pin_2, unsigned int pin_3, unsigned int pin_4) {
		inputArray[0] = pin_1;
		inputArray[1] = pin_2;
		inputArray[2] = pin_3;
		inputArray[3] = pin_4;

		gpio_set_output(pin_1);
		gpio_set_output(pin_2);
		gpio_set_output(pin_3);
		gpio_set_output(pin_4);
}

void stepper_off() {
		for(int i = 0; i < NUM_GPIO_PINS; i++) gpio_write(inputArray[i], LOW);
}

void step_stepper(unsigned int direction) {
		unsigned int bitPattern;
		for(int i = 0; i < NUM_ITERATIONS; i++) {
			if(direction == FORWARDS) bitPattern = 1 << (NUM_ITERATIONS - i - 1);
			else bitPattern = 1 << i;

			for(int j = 0; j < NUM_GPIO_PINS; j++) {
				if((bitPattern & (1<<(NUM_GPIO_PINS - j - 1))) >> (NUM_GPIO_PINS - j - 1)) gpio_write(inputArray[j], HIGH);
				else gpio_write(inputArray[j], LOW);
			}

			timer_delay_ms(2);
		}

		stepper_off();
}

void step_stepper_steps(unsigned int direction, unsigned int steps) {
		for(int i = 0; i < steps; i++) step_stepper(direction);
}

void turn_stepper_angle(unsigned int direction, unsigned int angle) {
		step_stepper_steps(direction, (angle * NUMSTEPS_180) / 180);
}

void turn_stepper_90(unsigned int direction) {
		turn_stepper_angle(direction, 90);
}

void turn_stepper_180(unsigned int direction) {
		turn_stepper_angle(direction, 180);
}

void turn_stepper_360(unsigned int direction) {
		turn_stepper_angle(direction, 360);
}

void turn_stepper_rotations(unsigned int direction, unsigned int rotations) {
		for(int i = 0; i < rotations; i++) turn_stepper_360(direction);
}
