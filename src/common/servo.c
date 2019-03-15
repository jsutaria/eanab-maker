#include "servo.h"
#include "gpio.h"
#include "pwm.h"

static unsigned int pin;

void servo_init(unsigned int servo_pin) {
  pin = servo_pin;

  pwm_init();
  gpio_set_function(18, GPIO_FUNC_ALT5);
  pwm_set_clock(19200000); //from the oscilator
  pwm_set_mode(0, PWM_MARKSPACE);
  pwm_set_fifo(0, 1);
  pwm_enable(0);
  pwm_set_range(0, 1023);

  pwm_set_width(0, 512);
  timer_delay(5);

  pwm_set_width(0, 256);
  timer_delay(5);
  //pwm_write(511);

  //timer_delay(3);
  pwm_write(255);
  timer_delay(3);
  pwm_write(127);

  //while(1) printf("%x\n", pwm_get_status());
  //gpio_set_output(GPIO_PIN18);
//  int i = 0;

  // while(1) {
  //   gpio_write(GPIO_PIN18, 1);
  //   timer_delay_ms(50 - (i % 50));
  //   gpio_write(GPIO_PIN18, 0);
  //   timer_delay_ms((i % 50));
  // }
  servo_set_0();
}

void servo_set_angle(unsigned int angle) {

}

void servo_set_0() {
    servo_set_angle(0);
}

void servo_set_180() {
    servo_set_angle(180);
}
