#ifndef STEPPER_H
#define STEPPER_H

#define NUMSTEPS_360 200

#define HIGH 1
#define LOW  0

#define FORWARDS  1
#define BACKWARDS 0

#define STEPPER_STEP_PIN 20
#define STEPPER_DIRECTION_PIN 21

void stepper_init(unsigned int direction_pin, unsigned int step_pin);

void stepper_set_direction(unsigned int direction);

void stepper_step(unsigned int speed);

void stepper_step_steps(unsigned int speed, unsigned int direction, unsigned int steps);

void stepper_turn_angle(unsigned int speed, unsigned int direction, unsigned int angle);

void stepper_turn_stepper_90(unsigned int speed, unsigned int direction);

void stepper_turn_180(unsigned int speed, unsigned int direction);

void stepper_turn_360(unsigned int speed, unsigned int direction);

void stepper_turn_rotations(unsigned int speed, unsigned int direction, unsigned int rotations);

void step_until_laser();

void step_backwards();

#endif
