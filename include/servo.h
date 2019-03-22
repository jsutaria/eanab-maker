#ifndef SERVO_H
#define SERVO_H

#define SERVO_PIN GPIO_PIN18
#define SERVO_WIDTH 2500

// Higher attempts will take longer to process but will be more accurate.
// Generally anywhere from 20-50 should suffice.
#define ATTEMPTS 30

void servo_init(unsigned int servo_pin);

#define ANGLE_CONST 232
void servo_set_angle(unsigned int angle);

void servo_set_0();

void servo_set_90();

void servo_set_180();

#endif
