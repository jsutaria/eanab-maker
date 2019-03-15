#ifndef SERVO_H
#define SERVO_H

#define SERVO_PIN GPIO_PIN18

void servo_init(unsigned int servo_pin);

void servo_set_angle(unsigned int angle);

void servo_set_0();

void servo_set_180();

#endif
