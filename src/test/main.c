#include "printf.h"
#include "uart.h"
#include "gpio.h"
#include "stepper.h"

void main(void) {
    uart_init();
    stepper_init(GPIO_PIN19, GPIO_PIN16, GPIO_PIN20, GPIO_PIN21);
    printf("Hello, world!\n");
}
