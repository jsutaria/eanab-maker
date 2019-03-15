#include "servo.h"
#include "modified_pwm.h"

void servo_init(unsigned int servo_pin) {
    pwm_init(servo_pin, SERVO_WIDTH);
    servo_set_0();
}

//angle should range from 1-180
void servo_set_angle(unsigned int angle) {
  angle = angle % 180 + 1;
  pwm_set_pulse(angle * SERVO_WIDTH / 232);

  pwm_float(ATTEMPTS);
}

void servo_set_0() {
    servo_set_angle(0);
}

void servo_set_90() {
    servo_set_angle(89);
}

void servo_set_180() {
    servo_set_angle(179);
}
