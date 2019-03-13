#include "gpio.h"
#include "timer.h"
#include "stepper_driver.h"

#define NUM_ITERATIONS 4
#define NUM_GPIO_PINS 4
#define HIGH 1
#define LOW 0

#define NUMSTEPS_FACE_OVER 162
#define NUMSTEPS_FACE_UNDER 34

unsigned int inputArray[4] = { 	GPIO_PIN12,
								GPIO_PIN16,
								GPIO_PIN20,
								GPIO_PIN21
								};

void stepper_init() {
	gpio_init();
	gpio_set_output(inputArray[0]);
	gpio_set_output(inputArray[1]);
	gpio_set_output(inputArray[2]);
	gpio_set_output(inputArray[3]);
}

void stepStepper(unsigned int direction){

	unsigned int bitPattern;
	for(int i = 0; i < NUM_ITERATIONS; i++){
		if(direction){
			bitPattern = 1 << (NUM_ITERATIONS - i - 1);
		} else {
			bitPattern = 1 << i;
		}
		for(int j = 0; j < NUM_GPIO_PINS; j++){
			if((bitPattern & (1<<(NUM_GPIO_PINS - j - 1))) >>  (NUM_GPIO_PINS - j - 1)){
				gpio_write(inputArray[j], HIGH);
			} else {
				gpio_write(inputArray[j], LOW);
			}
		}
		timer_delay_ms(2);
	}
}

void turnStepper(unsigned int direction, unsigned int numTimes) {

	for (int j = 0; j < numTimes; j++) {
		for(int i = 0; i < NUMSTEPS_90; i++){
			stepStepper(direction);
		}
		timer_delay_ms(1000);
	}
}

void turnStepperFace(unsigned int direction, unsigned int numTimes){
	for (int j = 0; j < numTimes; j++) {
		for(int i = 0; i < NUMSTEPS_FACE_OVER; i++){
			stepStepper(direction);
		}
		for(int i = 0; i < NUMSTEPS_FACE_UNDER; i++){
			stepStepper(!direction);
		}
		timer_delay_ms(1000);
	}
}

void turnStepperAngle(unsigned int direction, unsigned int angle){
	for(int i = 0; i < 1.42*angle; i++){
		stepStepper(direction);
	}
}
