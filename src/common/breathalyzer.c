#include "breathalyzer.h"
#include "malloc.h"
#include "printf.h"
#include "strings.h"
#include "gpio.h"

void breathalyzer_init(unsigned int breathalyzer_pin) {
  gpio_set_input(breathalyzer_pin);
}
unsigned int read_alcohol_level() {

}
