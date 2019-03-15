#include "printf.h"
#include "uart.h"
#include "timer.h"
#include "keyboard.h"
#include "magstripe.h"
#include "interrupts.h"
#include "malloc.h"
#include "storage.h"
#include "communicator.h"
#include "rand.h"

static storage_ingredients_t * generate_random_mix(void)
{
    storage_ingredients_t *ingredients = (storage_ingredients_t *)malloc(4);
    unsigned int total = 0;
    for (int i = 0; i < 4; i++) {
      ingredients[i] = rand() % 255;
      total += ingredients[i];
    }
    for (int i = 0; i < 4; i++) {
      ingredients[i] = ingredients[i] / total * 255;
    }

    return ingredients;
}

void main(void)
{
    timer_init();
    uart_init();
    magstripe_init(MAGSTRIPE_CLOCK, MAGSTRIPE_DATA);
    storage_init();
    communicator_init(COMMUNICATOR_CLOCK, COMMUNICATOR_DATA, COMMUNICATOR_MODE_MAKER);
    interrupts_global_enable();

    printf("Hello from the maker!\n");

    while (1) {
        // Swipe ==> start pipeline for drink prep
        magstripe_card_t *card = magstripe_read_next_card();
        char *user = card->track_2->pan;

        printf("Looks like %s wants to get a drink!\n", user);

        storage_ingredients_t *ingredients;

        if (!storage_has_key(user)) {
            ingredients = generate_random_mix();
        } else {
            ingredients = storage_get_key(user);
        }

        printf("Nice, we're gonna make %d/%d/%d/%d!\n", ingredients[0], ingredients[1], ingredients[2], ingredients[3]);

        // fixme
        // - check breathalyzer
        // - make drink
    }
}
