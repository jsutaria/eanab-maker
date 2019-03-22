#ifndef MAGSTRIPE_H
#define MAGSTRIPE_H

/**
 * Interface for reading swiped magstripe cards over PS/2 protocol.
 * Implemented for Magtek 21080204 but may work for other generally
 * similar PS/2 swipers as well.
 */

#include "keyboard.h"

#define MAGSTRIPE_DEBUG 1

#define MAGSTRIPE_CLOCK KEYBOARD_CLOCK
#define MAGSTRIPE_DATA KEYBOARD_DATA

#define MAGSTRIPE_PAN_SIZE 20
#define MAGSTRIPE_NAME_SIZE 27
#define MAGSTRIPE_EXPIRATION_DATE_SIZE 5
#define MAGSTRIPE_SERVICE_CODE_SIZE 4
#define MAGSTRIPE_DISCRETIONARY_DATA_SIZE 30

// ref: https://en.wikipedia.org/wiki/Magnetic_stripe_card
typedef struct {
    char pan[MAGSTRIPE_PAN_SIZE];
    char name[MAGSTRIPE_NAME_SIZE];
    char expiration_date[MAGSTRIPE_EXPIRATION_DATE_SIZE];
    char service_code[MAGSTRIPE_SERVICE_CODE_SIZE];
    char discretionary_data[MAGSTRIPE_DISCRETIONARY_DATA_SIZE];
} magstripe_track_t;

typedef struct {
    magstripe_track_t *track_1;
    magstripe_track_t *track_2;
} magstripe_card_t;

typedef enum {
    MAGSTRIPE_STATE_TRACK_1 = 1 << 0,
    MAGSTRIPE_STATE_TRACK_2 = 1 << 1,
} magstripe_states_t;

typedef enum {
    MAGSTRIPE_FIELD_PAN = 1 << 0,
    MAGSTRIPE_FIELD_NAME = 1 << 1,
    MAGSTRIPE_FIELD_EXPIRATION_DATE = 1 << 2,
    MAGSTRIPE_FIELD_SERVICE_CODE = 1 << 3,
    MAGSTRIPE_FIELD_DISCRETIONARY_DATA = 1 << 4,
} magstripe_fields_t;

/**
 * Initializes the magstripe library.
 */
void magstripe_init(unsigned int clock_gpio, unsigned int data_gpio);

void magstripe_reset(unsigned int clock_gpio, unsigned int data_gpio);

/**
 * Blocks until a card is swiped, then returns a `magstripe_card_t` object that
 * contains the contents of the card.
 */
magstripe_card_t * magstripe_read_next_card(void);

/**
 * Prints a human-readable copy of the contents of a `magstripe_card_t` to the console.
 */
void magstripe_dump_card(magstripe_card_t * card);

#endif
