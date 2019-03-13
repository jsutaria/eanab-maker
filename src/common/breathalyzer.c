<<<<<<< HEAD
#include "breathalyzer.h"
#include "malloc.h"
#include "printf.h"
#include "strings.h"
#include "gpio.h"

void breathalyzer_init(unsigned int breathalyzer_pin) {
  gpio_set_input(breathalyzer_pin);
}
unsigned int read_alcohol_level() {
=======
#include "beathalyzer.h"

static unsigned int data_gpio;

void breathalyzer_init(unsigned int data_pin)
{
    data_gpio = data_pin;
}

void breathalyzer_read(void) {
>>>>>>> 7215af588dd80397edf44e8b5e7189310a1f45eb

}
