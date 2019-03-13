#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

#define NUMSTEPS_180 256
#define NUMSTEPS_90 128

void stepper_init(void);

void stepStepper(unsigned int direction);

void turnStepper(unsigned int direction, unsigned int numTimes);

void turnStepperFace(unsigned int direction, unsigned int numTimes);

void turnStepperAngle(unsigned int direction, unsigned int angle);

#endif
