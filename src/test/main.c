#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "stepper_driver.h"
#include "timer.h"
#include "valve.h"
#include "mcp3008.h"
#include "breathalyzer.h"
#include "button.h"


void main(void) {
    uart_init();
    printf("Hello, world!\n");

    button_init(GPIO_PIN17);
    wait_for_press();
    // while (1) {
    //   wait_for_press();
    // }

    //HOW TO INITIALIZE VALVE:
    //valve_init(VALVE_1_PIN, VALVE_2_PIN, VALVE_3_PIN, VALVE_4_PIN);

    // mcp3008_init();



    // stepper_init(STEPPER_PIN1, STEPPER_PIN2, STEPPER_PIN3, STEPPER_PIN4);
    //
    // while(1) turnStepper(FORWARDS, NUMSTEPS_90);


    // mcp3008_init();
    breathalyzer_init(0);

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
