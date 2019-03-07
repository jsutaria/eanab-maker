#include "keyboard.h"
#include "malloc.h"

#define MAGSTRIPE_BUF_SIZE

void magstripe_init(unsigned int clock_gpio, unsigned int data_gpio)
{
    keyboard_init(clock_gpio, data_gpio);
}
