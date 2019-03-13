#include "valve.h"
#include "storage.h"
#include "malloc.h"
#include "printf.h"
#include "strings.h"
#include "gpio.h"

#define TIME_TOTAL 8000
#define TIME_DELAY 500
#define VALVE_1 GPIO_PIN4
#define VALVE_2 GPIO_PIN5
#define VALVE_3 GPIO_PIN6
#define VALVE_4 GPIO_PIN7

void valves_init() {
  for (int i = 0; i < 4; i++ {
    gpio_set_output(VALVE_1 + i);
  }
}

void test_on_off() {
  gpio_write(VALVE_1, 1);
  timer_delay(3);
  gpio_write(VALVE_1, 0);
}

void turn_on_valves(storage_ingredients_t *ingredients) {
  for (int i = 0; i < 4; i++) {
    gpio_write(VALVE_1 + i, 1);
  }

  int time_1 = ingredients[0];
  int time_2 = ingredients[1];
  int time_3 = ingredients[2];
  int time_4 = ingredients[3];


  while (time_1 > 0 || time_2 > 0 || time_3 > 0 || time_4 > 0) {
    time_1--;
    time_2--;
    time_3--;
    time_4--;

    if (time_1 == 1) {
      gpio_write(VALVE_1, 0);
    }

    if (time_2 == 1) {
      gpio_write(VALVE_2, 0);
    }

    if (time_3 == 1) {
      gpio_write(VALVE_3, 0);
    }

    if (time_4 == 1) {
      gpio_write(VALVE_4, 0);
    }

    timer_delay(TIME_DELAY);
}
