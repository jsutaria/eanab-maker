#include "printf.h"
#include "uart.h"
#include "magstripe.h"

void main(void) 
{
    magstripe_init(MAGSTRIPE_CLOCK, MAGSTRIPE_DATA);
    uart_init();
    printf("Hello, world!\n");
}
