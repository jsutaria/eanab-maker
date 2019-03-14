#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "storage.h"

#define COMMUNICATOR_CLOCK GPIO_PIN23
#define COMMUNICATOR_DATA GPIO_PIN24

#define COMMUNICATOR_MAX_LEN 1024
#define COMMUNICATOR_START_BYTE '%'
#define COMMUNICATOR_END_BYTE '$'
#define COMMUNICATOR_SENTINEL '^'

#define COMMUNICATOR_UART_INTERRUPTS_ENABLES 0x20215044
#define COMMUNICATOR_UART_INTERRUPTS_ENABLE  0x00000001

typedef enum {
    COMMUNICATOR_MODE_MAKER = 1,
    COMMUNICATOR_MODE_CONCIERGE = 2,
} communicator_mode_t;

typedef enum {
    COMMUNICATOR_TRANSMISSION_MODE_CONCIERGE_CLIENT_MESSAGE = 'c',
    COMMUNICATOR_TRANSMISSION_MODE_MAKER_UPDATE = 'u',
} communicator_transmission_mode_t;

/**
 * Initializes the communicator. Used for pi<->pi communication.
 */
void communicator_init(unsigned int clock_gpio, unsigned int data_gpio, communicator_mode_t mode);

/**
 * Sends a given (user, ingredients) pairing over the communicator.
 */
void communicator_send_drink(char *user, storage_ingredients_t *ingredients);

/**
 * Blocks and then returns when a new list of ingredients is read over
 * the communicator.
 */
storage_ingredients_t * communicator_read_next_ingredients();

/**
 * Processes a transmission received over the communicator.
 * (This is available for testing purposes.)
 */
void communicator_process_transmission(char* transmission);

#endif
