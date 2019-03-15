#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "stepper_driver.h"
#include "servo.h"
#include "timer.h"
#include "valve.h"
#include "mcp3008.h"
#include "breathalyzer.h"
#include "button.h"

void initialize(void);
void stepper_test(void);
void valve_test(void);
void button_test(void);
void servo_test(void);

void main(void) {
    initialize();
    printf("Hello, world!\n");

    // stepper_test();
    // valve_test();
    // button_test();
    servo_test();
}

void stepper_test() {
    int i = 0;
    printf("Turning: ");
    while(1) {
      printf("%04d°", 15 * i);
      turn_stepper_angle(FORWARDS, 15 * i);
      timer_delay(1);
      turn_stepper_angle(BACKWARDS, 15 * i++);
      timer_delay(1);
      printf("\b\b\b\b\b");
    }
}

void valve_test() {
    printf("Valves ---");
    while (1) {
      printf("\b\b\bOn");
      all_valves_on();
      timer_delay(1);
      printf("\b\bOff");
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
    int i = 0;
    // while(1) {
    //   i = i % 180;
    //   servo_set_angle(i);
    //   timer_delay(1);
    //   i += 15;
    // }
    while(1) {
      servo_set_0();
      timer_delay_ms(500);
      servo_set_90();
      timer_delay_ms(500);
      servo_set_180();
      timer_delay_ms(500);
    }
}

void initialize(void) {
   uart_init();
   gpio_init();
   servo_init(SERVO_PIN);
   stepper_init(STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);
   valves_init(VALVE_1_PIN, VALVE_2_PIN, VALVE_3_PIN, VALVE_4_PIN);
   button_init(GPIO_PIN17);
   breathalyzer_init(BREATHALYZER_CHANNEL);
}
