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
    // printf("testing all 4 valves simultaneously\n");
    // turn_on_valves(200, 100, 50, 400);
    // //stepper_init(GPIO_PIN19, GPIO_PIN16, GPIO_PIN20, GPIO_PIN21);
    // //stepper_test();
}

void stepper_test() {
    while(1) turnStepper(FORWARDS, NUMSTEPS_90);
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
