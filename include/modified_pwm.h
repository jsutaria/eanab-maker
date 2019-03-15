#ifndef PWM_TAJ_H
#define PWM_TAJ_H

typedef struct {
  unsigned int pin, width, pulse;
} pwm;

void pwm_init(unsigned int pwm_pin, unsigned int pwm_width);

void pwm_float(unsigned int iterations);

void pwm_set_pulse(unsigned int pwm_pulse);

#endif
