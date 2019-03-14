#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

#define NUMSTEPS_180 256
#define NUMSTEPS_90 128

#define FORWARDS  1
#define BACKWARDS 0

#define STEPPER_PIN_1 GPIO_PIN12
#define STEPPER_PIN_2 GPIO_PIN16
#define STEPPER_PIN_3 GPIO_PIN20
#define STEPPER_PIN_4 GPIO_PIN21

void stepper_init(unsigned int pin_1, unsigned int pin_2, unsigned int pin_3, unsigned int pin_4);

void step_stepper(unsigned int direction);

void turn_stepper_angle(unsigned int direction, unsigned int angle);

void turn_stepper_90(unsigned int direction);

void turn_stepper_180(unsigned int direction);

void turn_stepper_360(unsigned int direction);

void turn_stepper_rotations(unsigned int direction, unsigned int rotations);

#endif
