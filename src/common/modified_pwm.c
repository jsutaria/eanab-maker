#include "modified_pwm.h"
#include "gpio.h"
#include "timer.h"

static pwm pulse_width_modulation;

void pwm_init(unsigned int pwm_pin, unsigned int pwm_width) {
    gpio_set_output(pwm_pin);
    pulse_width_modulation.pin = pwm_pin;
    pulse_width_modulation.width = pwm_width;
}

void pwm_set_pulse(unsigned int pwm_pulse) {
    pulse_width_modulation.pulse = pwm_pulse;
}

void pwm_float(unsigned int iterations) {
  for(int i = 0; i < iterations; i++) {
      gpio_write(pulse_width_modulation.pin, 1);
      timer_delay_us(pulse_width_modulation.width - pulse_width_modulation.pulse);
      gpio_write(pulse_width_modulation.pin, 0);
      timer_delay_us(pulse_width_modulation.pulse);
  }
}
