#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "stepper_driver.h"
#include "timer.h"
#include "valve.h"
#include "mcp3008.h"


void main(void) {
    uart_init();
    printf("Hello, world!\n");

    //HOW TO INITIALIZE VALVE:
    //valve_init(VALVE_1_PIN, VALVE_2_PIN, VALVE_3_PIN, VALVE_4_PIN);

    //HOW TO INITIALIZE STEPPER:
    //stepper_init(STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);

    while(1) turnStepper(FORWARDS, NUMSTEPS_90);


    // mcp3008_init();
    //
    // while(1) {
    //   int val = readVals();
    //   int RS = (1024.0 - val) * 1000 / val;
    //   int R0 = RS / 60.0;
    //   printf("%d %d", RS, R0);
    //   timer_delay(1);
    //   printf("\n");
    // }
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
