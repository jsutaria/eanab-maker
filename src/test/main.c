#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "stepper.h"
#include "servo.h"
#include "timer.h"
#include "valve.h"
#include "mcp3008.h"
#include "breathalyzer.h"
#include "photoresistor.h"
#include "button.h"

void initialize(void);
void stepper_test(void);
void stepper_laser_test(void);
void valve_test(void);
void button_test(void);
void servo_test(void);
void breathalyzer_test(void);
void mcp3008_test(void);

#define LED1 GPIO_PIN16
#define LED2 GPIO_PIN12
#define LED3 GPIO_PIN1

void main(void) {
    initialize();
    printf("Hello, world!\n");

    // stepper_test();
    stepper_laser_test();
    valve_test();
    // button_test();
    //servo_test();
    mcp3008_test();
}

void stepper_test() {
    int i = 0;
    printf("Turning: ");
    while(1) {
      printf("%04d degrees", 90 * i);
      stepper_turn_angle(1, FORWARDS, 90 * i);
      timer_delay(1);
      stepper_turn_angle(1, BACKWARDS, 90 * i++);
      timer_delay(1);
      printf("\b\b\b\b\b\b\b\b\b\b\b\b");
    }
}

void stepper_laser_test() {
  step_until_laser();
  step_backwards();
}

void valve_test() {
    printf("Valves ---");
    while (1) {
      printf("\b\b\bOn ");
      all_valves_on();
      timer_delay(1);
      printf("\b\b\bOff");
      all_valves_off();
      timer_delay(1);
    }
}

void button_test() {
    while(1) {
      printf("Button Test Started\n");
      wait_for_press();
      printf("Button Pressed\n");
    }
}

void servo_test() {
    printf("Servo test\n");
    while(1) {
      servo_set_0();
      timer_delay_ms(500);
      servo_set_90();
      timer_delay_ms(500);
      servo_set_180();
      timer_delay_ms(500);
    }
}

void breathalyzer_test() {
  while (1) {
    printf("Breathe into the breathalyzer for the next 5 seconds.\n");
    unsigned int reading = breathalyzer_read();
    printf("Your reading scaled from 0 to 1023 is %d.\n", reading);
  }
}

void mcp3008_test() {
  while(1) {
    printf("Reading: %04d | %04d\n", mcp3008_read(0), mcp3008_read(1));
    timer_delay_ms(250);
  }
}

void initialize(void) {
   uart_init();
   gpio_init();
   servo_init(SERVO_PIN);
   stepper_init(STEPPER_DIRECTION_PIN, STEPPER_STEP_PIN);
   valves_init(VALVE_1_PIN, VALVE_2_PIN, VALVE_3_PIN, VALVE_4_PIN);
   button_init(GPIO_PIN17);
   breathalyzer_init(BREATHALYZER_CHANNEL);
   photoresistor_init(PHOTORESISTOR_CHANNEL);
}
