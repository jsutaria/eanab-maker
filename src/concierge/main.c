#include "printf.h"
#include "uart.h"
#include "timer.h"
#include "interrupts.h"
#include "magstripe.h"
#include "communicator.h"
#include "storage.h"

void main(void) 
{
    timer_init();
    uart_init();
    magstripe_init(MAGSTRIPE_CLOCK, MAGSTRIPE_DATA);
    storage_init();
    communicator_init(COMMUNICATOR_MODE_CONCIERGE);
    interrupts_global_enable();

    printf("Hello from the concierge!\n");

    while (1) {
        storage_ingredients_t *ingredients = communicator_read_next_ingredients();

        // fixme: ack ingredient reciept to UI to indicate readiness for swipe
        printf("Got a fresh ingredients list! Now waiting for card...\n");

        magstripe_card_t *card = magstripe_read_next_card();

        // fixme: ack card read to UI
        char *user = card->track_2->pan;
        printf("Roger, looks like %s wants to save their drink!\n", user);

        communicator_send_drink(card->track_2->pan, ingredients);

        // fixme: ack success
        printf("Cool, drink sent to Maker!\n");
    }
}
