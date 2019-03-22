#include "stepper.h"
#include "gpio.h"
#include "timer.h"
#include "photoresistor.h"
#include "printf.h"

static unsigned int pin_step, pin_direction;
#define THRESHOLD 1.25

/**
 * Initializes stepper given the direction and step pin
 * @param direction_pin direction GPIO pin
 * @param step_pin      stepper GPIO pin
 */
void stepper_init(unsigned int direction_pin, unsigned int step_pin) {
  pin_direction = direction_pin;
  pin_step = step_pin;

  gpio_set_output(direction_pin);
  gpio_set_output(step_pin);
}

/**
 * Sets direction of stepper to forward or backward
 * @param direction direction of stepper
 */
void stepper_set_direction(unsigned int direction) {
  gpio_write(pin_direction, direction);
}

/**
 * Step 1 step in stored direction at the given speed
 * @param speed speed of stepper
 */
void stepper_step(unsigned int speed) {
  gpio_write(pin_step, HIGH);
  timer_delay_us(800 * speed);
  gpio_write(pin_step, LOW);
  timer_delay_us(800 * speed);
}

/**
 * Step amount steps in the given direction at a given speed
 * @param speed     motor speed
 * @param direction direction for movement
 * @param amount    number of steps
 */
void stepper_step_steps(unsigned int speed, unsigned int direction, unsigned int amount) {
  stepper_set_direction(direction);
  for(int i = 0; i < amount; i++) stepper_step(speed);
}

/**
 * Turn stepper by a given angle
 * @param speed     speed of motor
 * @param direction direction to turn
 * @param angle     angle in degrees
 */
void stepper_turn_angle(unsigned int speed, unsigned int direction, unsigned int angle) {
		stepper_step_steps(speed, direction, (angle * NUMSTEPS_360) / 360);
}

/**
 * Turn stepper 90 degrees in given direction
 * @param speed     speed of stepper
 * @param direction direction of movement
 */
void stepper_turn_90(unsigned int speed, unsigned int direction) {
		stepper_turn_angle(speed, direction, 90);
}

/**
 * Turn stepper 180 degrees in given direction
 * @param speed     speed of stepper
 * @param direction direction of movement
 */

void stepper_turn_180(unsigned int speed, unsigned int direction) {
		stepper_turn_angle(speed, direction, 180);
}

/**
 * Turn stepper 360 degrees in given direction
 * @param speed     speed of stepper
 * @param direction direction of movement
 */

void stepper_turn_360(unsigned int speed, unsigned int direction) {
		stepper_turn_angle(speed, direction, 360);
}

/**
 * Turn stepper a given amount of full rotations (360) in a given direction
 * @param speed     speed for motor
 * @param direction direction for movement
 * @param rotations number of rotations to rotate
 */
void stepper_turn_rotations(unsigned int speed, unsigned int direction, unsigned int rotations) {
		for(int i = 0; i < rotations; i++) stepper_turn_360(speed, direction);
}

static unsigned int step_amount;

/**
 * Step until the laser is blocked
 */
void step_until_laser() {
    stepper_set_direction(FORWARDS);
    int start_val = photoresistor_read();
    int i;
    for(i = 0; photoresistor_read() < start_val * THRESHOLD; i++) stepper_step(1);
    step_amount = i;
}

/** 
 * Step in the opposite direction
 */
void step_backwards() {
    stepper_set_direction(BACKWARDS);
    for(int i = 0; i < step_amount; i++) stepper_step(2);
}
