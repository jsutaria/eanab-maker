#include "printf.h"
#include "uart.h"
#include "timer.h"
#include "keyboard.h"
#include "magstripe.h"
#include "interrupts.h"
#include "malloc.h"
#include "photoresistor.h"
#include "stepper.h"
#include "breathalyzer.h"
#include "valve.h"
#include "storage.h"
#include "communicator.h"
#include "rand.h"

void initialize(void) {
  timer_init();
  uart_init();
  valves_init(VALVE_1_PIN, VALVE_2_PIN, VALVE_3_PIN, VALVE_4_PIN);
  button_init(GPIO_PIN17);
  magstripe_init(MAGSTRIPE_CLOCK, MAGSTRIPE_DATA);
  stepper_init(STEPPER_DIRECTION_PIN, STEPPER_STEP_PIN);
  photoresistor_init(PHOTORESISTOR_CHANNEL);
  storage_init();
  communicator_init(COMMUNICATOR_CLOCK, COMMUNICATOR_DATA, COMMUNICATOR_MODE_MAKER);
  interrupts_global_enable();
}

void main(void)
{
    initialize();
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
        printf("Place your cup on the conveyor belt, then press the blue button when ready.\n");

        wait_for_press();
        printf("Great! Let's move on.\n");



        timer_delay(1);
        printf("Breathe into the breathalyzer for the next 7 seconds.\n");
        timer_delay_ms(500);

        if (detect_drunk()) {
            printf("Oops! Looks like you've had a bit too many EANABs for today ;). Try again next time!\n");
        } else {
            printf("Sweet, you're good to go!\n");
        }

        printf("Dispensing the cup right now!\n");
        timer_delay_ms(750);

        // Stepper
        timer_delay(1);
        printf("Moving the drink over to the loading station!\n");
        step_until_laser();
        printf("Great! The drink's in the right place!\n");

        // Valves
        timer_delay(1);
        printf("Dispensing now!\n");
        turn_on_valves(ingredients);
        step_backwards();
        printf("Your drink is ready to go! Enjoy!\n");
    }
}
