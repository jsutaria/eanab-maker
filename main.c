#include "printf.h"
#include "uart.h"
#include "timer.h"
#include "keyboard.h"
#include "magstripe.h"

void main(void) 
{
    timer_init();
    uart_init();
    magstripe_init(MAGSTRIPE_CLOCK, MAGSTRIPE_DATA);
    printf("Hello, world!\n");

    while (1) {
        magstripe_card_t *card = magstripe_read_next_card();

        // Enable this for fun debugging!
        // magstripe_dump_card(card);
        
        printf("Looks like %s wants to get a drink!\n", card->track_2->pan);
    }
}
