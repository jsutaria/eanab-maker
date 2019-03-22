#include "stepper.h"
#include "gpio.h"
#include "timer.h"
#include "photoresistor.h"
#include "printf.h"

static unsigned int pin_step, pin_direction;
#define THRESHOLD 1.25

void stepper_init(unsigned int direction_pin, unsigned int step_pin) {
  pin_direction = direction_pin;
  pin_step = step_pin;

  gpio_set_output(direction_pin);
  gpio_set_output(step_pin);
}

void stepper_set_direction(unsigned int direction) {
  gpio_write(pin_direction, direction);
}

void stepper_step(unsigned int speed) {
  gpio_write(pin_step, HIGH);
  timer_delay_us(800 * speed);
  gpio_write(pin_step, LOW);
  timer_delay_us(800 * speed);
}

void stepper_step_steps(unsigned int speed, unsigned int direction, unsigned int amount) {
  stepper_set_direction(direction);
  for(int i = 0; i < amount; i++) stepper_step(speed);
}

void stepper_turn_angle(unsigned int speed, unsigned int direction, unsigned int angle) {
		stepper_step_steps(speed, direction, (angle * NUMSTEPS_360) / 360);
}

void stepper_turn_90(unsigned int speed, unsigned int direction) {
		stepper_turn_angle(speed, direction, 90);
}

void stepper_turn_180(unsigned int speed, unsigned int direction) {
		stepper_turn_angle(speed, direction, 180);
}

void stepper_turn_360(unsigned int speed, unsigned int direction) {
		stepper_turn_angle(speed, direction, 360);
}

void stepper_turn_rotations(unsigned int speed, unsigned int direction, unsigned int rotations) {
		for(int i = 0; i < rotations; i++) stepper_turn_360(speed, direction);
}

static unsigned int step_amount;

void step_until_laser() {
    stepper_set_direction(FORWARDS);
    int start_val = photoresistor_read();
    int i;
    for(i = 0; photoresistor_read() < start_val * THRESHOLD; i++) stepper_step(1);
    step_amount = i;
}

void step_backwards() {
    stepper_set_direction(BACKWARDS);
    for(int i = 0; i < step_amount; i++) stepper_step(2);
}
