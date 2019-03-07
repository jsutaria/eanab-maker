#ifndef MAGSTRIPE_H
#define MAGSTRIPE_H

#include "keyboard.h"

/**
 * Interface for reading swiped magstripe cards over PS/2 protocol.
 * Implemented for Magtek 21080204 but may work for other generally
 * similar swipers as well.
 */

#define MAGSTRIPE_CLOCK KEYBOARD_CLOCK
#define MAGSTRIPE_DATA KEYBOARD_DATA

void magstripe_init(unsigned int clock_gpio, unsigned int data_gpio);

#endif
