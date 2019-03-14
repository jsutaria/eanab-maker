#include "gpio.h"
#include "timer.h"
#include "stepper_driver.h"

#define NUM_ITERATIONS 4
#define NUM_GPIO_PINS 4
#define HIGH 1
#define LOW 0

#define NUMSTEPS_FACE_OVER 162
#define NUMSTEPS_FACE_UNDER 34

static unsigned int pin_map[NUM_GPIO_PINS];

void stepper_init(unsigned int pin_1, unsigned int pin_2, unsigned int pin_3, unsigned int pin_4) {
		gpio_init();

		pin_map[0] = pin_1;
		pin_map[1] = pin_2;
		pin_map[2] = pin_3;
		pin_map[3] = pin_4;

		gpio_set_output(pin_1);
		gpio_set_output(pin_2);
		gpio_set_output(pin_3);
		gpio_set_output(pin_4);
}

void stepStepper(unsigned int direction) {
		unsigned int bitPattern = 0;
		for(int i = 0; i < 4; i++) {
				if(direction == FORWARDS) bitPattern = 1 << (NUM_ITERATIONS - i - 1);
				else bitPattern = 1 << i;
		}

		for(int j = 0; j < NUM_GPIO_PINS; j++) {
			if((bitPattern & (1<<(NUM_GPIO_PINS - j - 1))) >>  (NUM_GPIO_PINS - j - 1)){
				gpio_write(pin_map[j], HIGH);
			} else {
				gpio_write(pin_map[j], LOW);
			}
		}

		timer_delay_ms(2);
}

void turnStepper(unsigned int direction, unsigned int numTimes) {
	for (int j = 0; j < numTimes; j++) {
		for(int i = 0; i < NUMSTEPS_90; i++) stepStepper(direction);
		timer_delay_ms(1000);
	}
}

void turnStepperAngle(unsigned int direction, unsigned int angle){
	for(int i = 0; i < 1024 / 720 * angle; i++) stepStepper(direction);
}
