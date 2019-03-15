#ifndef KEYBOARDISH_H
#define KEYBOARDISH_H

#include "gpio.h"
#include "ps2.h"
#include "keyboard.h"

/*
 * Module to read keys typed on a PS/2 keyboardish.
 *
 * You implement this module in assignments 5 and 7.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Author: TAJ team
 *
 * Last updated:   February 2019
 */

/*
 * `keyboardish_init`: Required initialization for keyboardish
 *
 * The keyboardish must first be initialized before any key events can be read.
 * The first and second arguments identify which GPIO pins to use for the
 * PS/2 clock and data lines, respectively.
 *
 * Although `keyboardish_init` configures the requested clock and data GPIOs
 * to use the internal pull-up resistors, it is recommended to choose GPIO
 * pins whose default state is already pull-up, such as KEYBOARD_CLOCK and
 * KEYBOARD_DATA defined above. This avoid timing issues due to the keyboardish
 * attempting to handshake with the Pi before `keyboardish_init` has executed.
 */
void keyboardish_init(unsigned int clock_gpio, unsigned int data_gpio);


/*
 * `keyboardish_read_next`: Top level keyboardish interface.
 *
 * This function reads (blocking) the next key typed on the keyboardish.
 * The character returned reflects the current keyboardish modifier settings
 * for shift and caps lock.
 *
 * Characters returned that have value <= 0x7f '~' are printable Ascii
 * characters. Character values >= 0x90 are returned for those keys that are
 * are not associated with an Ascii character (e.g. arrow and function keys).
 * See the ps2_codes defined in ps2.h for constants used for those keys.
 * This function calls `keyboardish_read_event`.
 */
unsigned char keyboardish_read_next(void);


/*
 * `keyboardish_read_event`: Mid level keyboardish interface.
 *
 * The function reads (blocking) the next key event from the keyboardish.
 * Returns a `key_event_t` struct that represents the key event.  The
 * struct includes the key that was pressed or released and the state
 * of the modifier flags. If this event is a press or release of a
 * modifier key (CTRL, ALT, SHIFT, etc.), the modifiers field in the
 * returned event show the updated state of the modifiers after
 * incorporating this key event.
 * This function calls `keyboardish_read_sequence` to read a scancode sequence.
 */
key_event_t keyboardish_read_event(void);


/*
 * `keyboardish_read_sequence`: Low level keyboardish interface.
 *
 * Reads a sequence of scancode bytes corresponding to the press or
 * release of a single key.  Returns a `key_action_t` struct that
 * represents the key action for the sequence read. Reads 1, 2, or 3
 * scancodes:
 *    1 byte:  ordinary key press
 *    2 bytes: ordinary key release or extended key press
 *    3 bytes: extended key release
 * This function calls `keyboardish_read_scancode` to read each scancode.
 */
key_action_t keyboardish_read_sequence(void);


/*
 * `keyboardish_read_scancode`: Bottom level keyboardish interface.
 *
 * Read (blocking) a single scancode from the PS/2 keyboardish.
 * Bits are read on the falling edge of the clock.
 *
 * Tries to read 11 bits: 1 start bit, 8 data bits, 1 parity bit, and 1 stop bit
 *
 * Restarts the scancode if:
 *   (lab5) The start bit is incorrect
 *   (assign5) or if parity or stop bit is incorrect
 *
 * Returns the 8 data bits of a well-formed PS/2 scancode.
 * Will not return until it reads a complete and valid scancode.
 */
unsigned char keyboardish_read_scancode(void);


/*
 * `keyboardish_use_interrupts`
 *
 * Change keyboardish from default polling behavior to instead configure interrupts
 * for gpio events. After setting keyboardish to use interrupts, client must
 * also globally enable interrupts at system level.
 */
void keyboardish_use_interrupts(void);


#endif
