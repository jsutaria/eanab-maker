#include "modified_pwm.h"
#include "gpio.h"
#include "timer.h"

static pwm pulse_width_modulation;

/**
 * Initializes a modified 
 * @param pwm_pin   [description]
 * @param pwm_width [description]
 */
void pwm_init(unsigned int pwm_pin, unsigned int pwm_width) {
    gpio_set_output(pwm_pin);
    pulse_width_modulation.pin = pwm_pin;
    pulse_width_modulation.width = pwm_width;
}

/**
 * Sets the pulse length for the pwm signal.
 * @param pwm_pulse the pulse for the pwm
 */
void pwm_set_pulse(unsigned int pwm_pulse) {
    pulse_width_modulation.pulse = pwm_pulse;
}

/**
 * Sends the pwm signal (pulse, no pulse) for the given amount of iterations.
 * @param iterations how many iterations you want the pulse for
 */
void pwm_float(unsigned int iterations) {
  for(int i = 0; i < iterations; i++) {
      gpio_write(pulse_width_modulation.pin, 1);
      timer_delay_us(pulse_width_modulation.width - pulse_width_modulation.pulse);
      gpio_write(pulse_width_modulation.pin, 0);
      timer_delay_us(pulse_width_modulation.pulse);
  }
}
