#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "stepper.h"
#include "valve.h"

void main(void) {
    uart_init();
    printf("Hello, world!\n");

    valves_init();

    // printf("Testing on and off on gpio4\n");
    // test_on_off();

    printf("testing all 4 valves simultaneously\n");
    turn_on_valves(200, 100, 50, 400);
    //stepper_init(GPIO_PIN19, GPIO_PIN16, GPIO_PIN20, GPIO_PIN21);
    //stepper_test();
}
