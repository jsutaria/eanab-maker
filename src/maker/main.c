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

void initialize(void) {
  timer_init();
  uart_init();
  magstripe_init(MAGSTRIPE_CLOCK, MAGSTRIPE_DATA);
  storage_init();
  communicator_init(COMMUNICATOR_CLOCK, COMMUNICATOR_DATA, COMMUNICATOR_MODE_MAKER);
  interrupts_global_enable();
}

void main(void)
{
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

        timer_delay(1);
        printf("Breathe into the breathalyzer for the next 7 seconds.\n");
        timer_delay_ms(500);

        if (detect_drunk()) {
          printf("Oops! Looks like you've had a bit too many EANABs for today ;). Try again next time!\n");
        } else {
          printf("Sweet, you're good to go!\n");

          // Servo
          timer_delay(1);
          printf("Dispensing the cup right now!\n");
          servo_set_90();
          timer_delay_ms(750);
          servo_set_0();

          // Stepper
          timer_delay(1);
          printf("Moving the drink over to the loading station!\n");
          turn_stepper_rotations(FORWARDS, 10);
          printf("Great! The drink's in the right place!\n");

          // Valves
          timer_delay(1);
          printf("Dispensing now!\n");
          turn_on_valves(ingredients);
          printf("Your drink is ready to go! Enjoy!\n");

        }
    }
}
