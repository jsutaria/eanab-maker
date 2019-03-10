#include "printf.h"
#include "uart.h"
#include "timer.h"
#include "magstripe.h"

void main(void) 
{
    timer_init();
    uart_init();
    magstripe_init(MAGSTRIPE_CLOCK, MAGSTRIPE_DATA);
    printf("Hello from the concierge!\n");

    while (1) {
        magstripe_card_t *card = magstripe_read_next_card();        
        printf("Looks like %s wants to save their drink!\n", card->track_2->pan);
    }
}
