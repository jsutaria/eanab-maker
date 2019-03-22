#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "stepper.h"
#include "timer.h"
#include "valve.h"
#include "mcp3008.h"
#include "breathalyzer.h"
#include "photoresistor.h"
#include "button.h"
#include "rand.h"

void initialize(void);
void stepper_test(void);
void stepper_laser_test(void);
void valve_test(void);
void button_test(void);
void breathalyzer_test(void);
void mcp3008_test(void);
void rand_test(void);

#define LED1 GPIO_PIN16
#define LED2 GPIO_PIN12
#define LED3 GPIO_PIN1

void main(void) {
    initialize();
    printf("Hello, world!\n");

    // button_test();
    // breathalyzer_test();
    // stepper_test();
    // stepper_laser_test();
    valve_test();
    // button_test();
    //servo_test();
    // rand_test();
    // mcp3008_test();
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
    int i = 0;
    unsigned int j = 250;
    while(1) {
      valve_on(i % 4);
      timer_delay_ms(j--);
      valve_off(i % 4);
      printf("%d", i++);
    }
    while (1) {
      printf("\b\b\bOn ");
      all_valves_on();
      timer_delay_ms(500);
      printf("\b\b\bOff");
      all_valves_off();
      timer_delay_ms(500);
    }
}

void button_test() {
    while(1) {
      printf("Button Test Started\n");

      wait_for_press();
      printf("Button Pressed\n");
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

void rand_test() {
  int num_test = 0;
  while (1) {
    printf("TEST %d!!!!!\n", num_test++);
    printf("Random number 1 under 100 is %d\n", rand_under(100));
    printf("Random number 2 under 100 is %d\n", rand_under(100));
    printf("Random number 3 under 100 is %d\n", rand_under(100));
    printf("Random number 1 between 10 and 20 is %d\n", rand_between(10, 20));
    printf("Random number 2 between 10 and 20 is %d\n", rand_between(10, 20));
    printf("Random number 3 between 10 and 20 is %d\n", rand_between(10, 20));
    char c = rand_char();
    printf("Random char is number %d and char %c\n", c, c);
    printf("\n\n");
    timer_delay(2);
  }
}

void initialize(void) {
   uart_init();
   gpio_init();
   stepper_init(STEPPER_DIRECTION_PIN, STEPPER_STEP_PIN);
   valves_init(VALVE_1_PIN, VALVE_2_PIN, VALVE_3_PIN, VALVE_4_PIN);
   button_init(GPIO_PIN17);
   breathalyzer_init(BREATHALYZER_CHANNEL);
   photoresistor_init(PHOTORESISTOR_CHANNEL);
}
