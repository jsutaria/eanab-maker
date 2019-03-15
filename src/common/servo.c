#include "servo.h"
#include "gpio.h"
#include "pwm.h"

void servo_init() {
  pwm_init();
  pwm_set_clock(19200000);
  pwm_enable(0);
  pwm_set_mode(0, 1);
  pwm_set_fifo(0, 1);
  pwm_set_range(0, 4095);
  pwm_write(100);
  //while(1) printf("%x\n", pwm_get_status());
  //gpio_set_output(GPIO_PIN18);
  int i = 0;

  while(1) {
    gpio_write(GPIO_PIN18, 1);
    timer_delay_ms(50 - (i % 50));
    gpio_write(GPIO_PIN18, 0);
    timer_delay_ms((i % 50));
  }

}
