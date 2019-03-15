#include "servo.h"
#include "gpio.h"
#include "timer.h"

static unsigned int pin;

void servo_init(unsigned int servo_pin) {
    pin = servo_pin;
    gpio_set_output(servo_pin);
    servo_set_0();
}

//angle should range from 1-120
void servo_set_angle(unsigned int angle) {
  angle = angle % 180 + 1;
  unsigned int time_off = angle * 2500 / 230;

  for(int i = 0; i < 100; i++) {
    gpio_write(SERVO_PIN, 1);
    timer_delay_us(2500 - time_off);
    gpio_write(SERVO_PIN, 0);
    timer_delay_us(time_off);
  }
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
