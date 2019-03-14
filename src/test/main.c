#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "stepper_driver.h"
#include "timer.h"
#include "valve.h"
#include "mcp3008.h"
#include "breathalyzer.h"
#include "button.h"

void initialize(void);
void stepper_test(void);

void main(void) {
    initialize();
    printf("Hello, world!\n");

    stepper_test();
    valve_test();
  //  wait_for_press();
    // while (1) {
    //   wait_for_press();
    // }

    //HOW TO INITIALIZE VALVE: valve_init(VALVE_1_PIN, VALVE_2_PIN, VALVE_3_PIN, VALVE_4_PIN);

    // mcp3008_init();



    // stepper_init(STEPPER_PIN1, STEPPER_PIN2, STEPPER_PIN3, STEPPER_PIN4);
    //
    // while(1) turnStepper(FORWARDS, NUMSTEPS_90);


    // mcp3008_init();


    while(1) {
      printf("Value: %d\n", breathalyzer_read());
      // int val = mcp3008_read(0);
      // int RS = (1024.0 - val) * 1000 / val;
      // int R0 = RS / 60.0;
      // printf("%d %d", RS, R0);
      // timer_delay(1);
      // printf("\n");
    }
    // valves_init();
    //
    // // printf("Testing on and off on gpio4\n");
    // // test_on_off();
    //
    //
}

void stepper_test() {
    int i = 0;
    while(1) {
      printf("Turning: %d\n", 15 * i);
      turn_stepper_angle(FORWARDS, 15 * i);
      timer_delay(1);
      turn_stepper_angle(BACKWARDS, 15 * i++);
      timer_delay(1);
    }
}

void valve_test() {
    while (1) {
      all_valves_on();
      timer_delay(1);
      all_valves_off();
      timer_delay(1);
    }
}

void initialize(void) {
   uart_init();
   gpio_init();
   mcp3008_init();
   button_init(GPIO_PIN17);
   stepper_init(STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);
   valves_init(VALVE_1_PIN, VALVE_2_PIN, VALVE_3_PIN, VALVE_4_PIN);
   breathalyzer_init(BREATHALYZER_CHANNEL);
}
