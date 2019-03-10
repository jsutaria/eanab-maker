#include "communicator.h"
#include "uart.h"
#include "interrupts.h"
#include "malloc.h"
#include "printf.h"
#include "strings.h"
#include "storage.h"

static int communicator_mode = 0;
static int current_transmission_index = -1;
static char current_transmission[COMMUNICATOR_MAX_LEN];

static storage_ingredients_t *pending_ingredients = NULL;

/**
 * Interrupt handler for new uart char received.
 */
static bool on_uart_recv(unsigned int pc)
{
    if (uart_haschar()) {
        char ch = uart_getchar();
        uart_putchar(ch); // fixme: remove after debugging, this will probably confuse stuff

        if (ch == COMMUNICATOR_START_BYTE) {
            current_transmission_index = 0;

        } else if (ch == COMMUNICATOR_END_BYTE ||
            current_transmission_index == COMMUNICATOR_MAX_LEN - 1)
        {
            current_transmission[current_transmission_index] = '\0';
            communicator_process_transmission(current_transmission);
            current_transmission_index = -1;

        } else if (current_transmission_index >= 0) {
            current_transmission[current_transmission_index++] = ch;
        }

        return true;
    }

    return false;
}

void communicator_init(communicator_mode_t mode)
{
    uart_init();
    interrupts_enable_source(INTERRUPTS_AUX);
    interrupts_attach_handler(on_uart_recv);

    // must enable mini-uart interrupts
    int *aux = (int*)COMMUNICATOR_UART_INTERRUPTS_ENABLES;
    *aux |= COMMUNICATOR_UART_INTERRUPTS_ENABLE;

    // save mode
    communicator_mode = mode;
}

void communicator_send_drink(char *user, storage_ingredients_t *ingredients)
{
    uart_putchar(COMMUNICATOR_START_BYTE);
    uart_putchar(COMMUNICATOR_TRANSMISSION_MODE_MAKER_UPDATE);

    while (*user) uart_putchar(*user++);
    uart_putchar(COMMUNICATOR_SENTINEL);
    while (*ingredients) uart_putchar(*ingredients++);

    uart_putchar(COMMUNICATOR_END_BYTE);
}

storage_ingredients_t * communicator_read_next_ingredients() {
    while (!pending_ingredients) { /* spin */ }
    storage_ingredients_t * result = pending_ingredients;
    pending_ingredients = NULL;
    return result;
}

void communicator_process_transmission(char* transmission)
{
    communicator_transmission_mode_t mode = *transmission++; // consume first byte

    if (mode == COMMUNICATOR_TRANSMISSION_MODE_MAKER_UPDATE) {

        char *user = malloc(STORAGE_USER_LENGTH);
        for (int i = 0; *transmission != COMMUNICATOR_SENTINEL; i++) {
            user[i] = *transmission++;
        }

        transmission++; // consume sentinel

        storage_ingredients_t *ingredients = malloc(STORAGE_INGREDIENTS_LENGTH);
        memset(ingredients, '\0', STORAGE_INGREDIENTS_LENGTH);
        for (int i = 0; *transmission; i++) {
            ingredients[i] = *transmission++;
        }

        storage_put_key(user, ingredients);
        printf("saved drink: %s => %s!\n", user, ingredients);

    } else if (mode == COMMUNICATOR_TRANSMISSION_MODE_CONCIERGE_CLIENT_MESSAGE) {

        storage_ingredients_t *ingredients = malloc(STORAGE_INGREDIENTS_LENGTH);
        memset(ingredients, '\0', STORAGE_INGREDIENTS_LENGTH);
        for (int i = 0; *transmission; i++) {
            ingredients[i] = *transmission++;
        }

        pending_ingredients = ingredients;
        printf("updating pending ingredients: pending => %s!\n", pending_ingredients);

    } else {
        // fall through, unsupported mode
    }
}
