#include "beathalyzer.h"

static unsigned int data_gpio;

void breathalyzer_init(unsigned int data_pin)
{
    data_gpio = data_pin;
}

void breathalyzer_read(void) {

}
