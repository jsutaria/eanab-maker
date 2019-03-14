#include "communicator.h"
#include "uart.h"
#include "interrupts.h"
#include "malloc.h"
#include "printf.h"
#include "strings.h"
#include "storage.h"
#include "timer.h"
#include "gpio.h"
#include "gpioextra.h"
#include "ps2_helpers.h"

static unsigned int CLK, DATA;

// Internal state used by `on_character_received` to build up chars.
static int communicator_mode = 0;
static int current_transmission_index = -1;
static char current_transmission[COMMUNICATOR_MAX_LEN];

// Internal state used to buffer pending ingredient requests.
static storage_ingredients_t *pending_ingredients = NULL;

// Internal state used by `on_clock_falling_edge` to build up scancodes.
static int current_index = 0;
static unsigned int current_data = 0;
static bool current_parity = 1;

/**
 * Handler for new complete character received.
 */
static void on_character_received(char ch)
{
    uart_putchar(ch);

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
}

/**
 * Interrupt handler for PS/2-based communication.
 */
static bool on_clock_falling_edge(unsigned int pc) 
{
    // printf("interrup state: clk=%d, dat=%d\n", gpio_read(CLK), gpio_read(DATA));

    if (gpio_check_and_clear_event(CLK)) {

        unsigned char bit = gpio_read(DATA);

        switch (current_index) {
            case 0:
                // start bit
                if (bit != 0) { current_index = -1; }
                current_data = 0;
                break;
            case 9:;
                // parity bit - ignore for now
                break;
            case 10:
                // stop bit
                current_index = -1;
                if (bit == 1) {
                    on_character_received(current_data);
                }
                break;
            default:
                // data bits
                current_data |= bit << (current_index - 1);
                if (bit) { current_parity = !current_parity; }
                break;
        }

        current_index++;
        return true;
    }
    return false;
}

void communicator_init(unsigned int clock_gpio, unsigned int data_gpio, communicator_mode_t mode)
{
    // set up ps/2 interface
    CLK = clock_gpio;
    gpio_set_input(CLK);
    gpio_set_pullup(CLK); 

    DATA = data_gpio;
    gpio_set_input(DATA);
    gpio_set_pullup(DATA);

    printf("Curr state: clk=%d, dat=%d\n", gpio_read(CLK), gpio_read(DATA));

    gpio_enable_event_detection(CLK, GPIO_DETECT_FALLING_EDGE);
    interrupts_attach_handler(on_clock_falling_edge);
    interrupts_enable_source(INTERRUPTS_GPIO3);

    // save mode
    communicator_mode = mode;
}

static void communicator_putchar(unsigned char c)
{
    uart_putchar(c);
    ps2_write(CLK, DATA, c);
}

void communicator_send_drink(char *user, storage_ingredients_t *ingredients)
{
    printf("putting data: ");
    communicator_putchar(COMMUNICATOR_START_BYTE);
    communicator_putchar(COMMUNICATOR_TRANSMISSION_MODE_MAKER_UPDATE);

    while (*user) communicator_putchar(*user++);
    communicator_putchar(COMMUNICATOR_SENTINEL);
    while (*ingredients) communicator_putchar(*ingredients++);

    communicator_putchar(COMMUNICATOR_END_BYTE);
    printf("\n");
}

storage_ingredients_t * communicator_read_next_ingredients() {
    while (!pending_ingredients) { /* spin */ }
    storage_ingredients_t * result = pending_ingredients;
    pending_ingredients = NULL;
    return result;
}

void communicator_process_transmission(char* transmission)
{
    printf("processing tx %s\n", transmission);
    communicator_transmission_mode_t mode = *transmission++; // consume first byte

    if (communicator_mode == COMMUNICATOR_MODE_MAKER &&
        mode == COMMUNICATOR_TRANSMISSION_MODE_MAKER_UPDATE)
    {
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

    } else if (communicator_mode == COMMUNICATOR_MODE_CONCIERGE &&
        mode == COMMUNICATOR_TRANSMISSION_MODE_CONCIERGE_CLIENT_MESSAGE)
    {
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
