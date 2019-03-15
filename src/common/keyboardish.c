#include "keyboardish.h"
#include "gpio.h"
#include "gpioextra.h"
#include "keyboard.h"
#include "timer.h"
#include "ps2.h"
#include "interrupts.h"
#include "ringbuffer.h"
#include "printf.h"
#include <stdbool.h>

static unsigned int CLK, DATA;
static  keyboard_modifiers_t modifier_state = 0;
static rb_t *keyboardish_pending_scancodes;

// Internal state used by `on_clock_falling_edge` to build up scancodes.
static int current_index = 0;
static unsigned int current_data = 0;
static bool current_parity = 1;
static unsigned int last_falling_edge = 0;

static bool on_clock_falling_edge(unsigned int pc)
{
    if (gpio_check_and_clear_event(CLK)) {
        // If more than 3ms have elapsed between falling clock edges,
        // consider it timed out and assume new edge is for a start bit.
        unsigned int now = timer_get_ticks();
        if (current_index != 0 && now - last_falling_edge > 3000) { current_index = 0; }
        last_falling_edge = now;

        unsigned char bit = gpio_read(DATA);

        switch (current_index) {
            case 0:
                // start bit
                if (bit != 0) { current_index = -1; }
                current_data = 0;
                break;
            case 9:;
                // parity bit
                if (bit != current_parity) { current_index = -1; }
                break;
            case 10:
                // stop bit
                current_index = -1;
                if (bit == 1) {
                    rb_enqueue(keyboardish_pending_scancodes, current_data);
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

void keyboardish_init(unsigned int clock_gpio, unsigned int data_gpio)
{
    CLK = clock_gpio;
    gpio_set_input(CLK);
    gpio_set_pullup(CLK);

    DATA = data_gpio;
    gpio_set_input(DATA);
    gpio_set_pullup(DATA);

    keyboardish_pending_scancodes = rb_new();

    gpio_enable_event_detection(CLK, GPIO_DETECT_FALLING_EDGE);
    interrupts_attach_handler(on_clock_falling_edge);
    interrupts_enable_source(INTERRUPTS_GPIO3);
}

unsigned char keyboardish_read_scancode(void)
{
    int code = 0;
    while (!rb_dequeue(keyboardish_pending_scancodes, &code)) { /* spin */ }
    return (unsigned char)code;
}

key_action_t keyboardish_read_sequence(void)
{
    key_action_t action;
    action.what = KEY_PRESS;

    while (1) {
        unsigned char code = keyboardish_read_scancode();

        if (code == PS2_CODE_EXTENDED) {
            // fall through; for our purposes we're considering left
            // and right modifier keys as equivalent

        } else if (code == PS2_CODE_RELEASE) {
            action.what = KEY_RELEASE;

        } else {
            action.keycode = code;
            break;
        }
    }

    return action;
}

/**
 * Updates the global modifier state based on a key action & key.
 */
void update_global_modifier_state(key_action_t action, ps2_key_t key) {
    unsigned int mod_flag;
    bool mod_enabling;

    // Determine relevant modifier
    if (key.ch == PS2_KEY_CAPS_LOCK) {
        mod_flag = KEYBOARD_MOD_CAPS_LOCK;
    } else if (key.ch == PS2_KEY_NUM_LOCK) {
        mod_flag = KEYBOARD_MOD_NUM_LOCK;
    } else if (key.ch == PS2_KEY_SCROLL_LOCK) {
        mod_flag = KEYBOARD_MOD_SCROLL_LOCK;
    } else if (key.ch == PS2_KEY_SHIFT) {
        mod_flag = KEYBOARD_MOD_SHIFT;
    } else if (key.ch == PS2_KEY_ALT) {
        mod_flag = KEYBOARD_MOD_ALT;
    } else if (key.ch == PS2_KEY_CTRL) {
        mod_flag = KEYBOARD_MOD_CTRL;
    } else {
        mod_flag = 0;
    }

    // Handle toggle keys
    if (key.ch == PS2_KEY_CAPS_LOCK ||
        key.ch == PS2_KEY_NUM_LOCK ||
        key.ch == PS2_KEY_SCROLL_LOCK
    ) {
        if (action.what != KEY_PRESS) { mod_flag = 0; } // only toggle on keypress
        mod_enabling = !(modifier_state & mod_flag);
    } else {
        mod_enabling = action.what == KEY_PRESS;
    }

    // Update state if necessary
    if (mod_flag) {
        if (mod_enabling) {
            modifier_state |= mod_flag;
        } else {
            modifier_state &= ~mod_flag;
        }
    }
}

key_event_t keyboardish_read_event(void)
{
    key_action_t action = keyboardish_read_sequence();
    ps2_key_t key = ps2_keys[action.keycode];

    update_global_modifier_state(action, key);

    key_event_t event = { action, key, modifier_state };
    return event;
}

static bool is_valid_next_event(key_event_t event) {
    return event.action.what == KEY_PRESS &&
        (event.key.ch <= 0x7f || event.key.ch > PS2_KEY_CAPS_LOCK);
}

/**
 * Returns the value of a modifier.
 */
bool keyboardish_get_modifier( keyboard_modifiers_t mod) {
    return modifier_state & mod;
}

unsigned char keyboardish_read_next(void)
{
    key_event_t event = keyboardish_read_event();

    // Keep waiting for a valid next event
    while (!is_valid_next_event(event)) {
        event = keyboardish_read_event();
    }

    if (event.key.other_ch && (

        // shift works for all keys
        (event.modifiers & KEYBOARD_MOD_SHIFT) ||

        // caps lock does not affect top num row
        (event.modifiers & KEYBOARD_MOD_CAPS_LOCK &&
            (event.key.ch < '0' || event.key.ch > '9') &&
            event.key.ch != '-' && event.key.ch != '=' && event.key.ch != '\\'
        )
    )) {
        return event.key.other_ch;
    }

    return event.key.ch;
}
