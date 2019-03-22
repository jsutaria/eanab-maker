#include "valve.h"
#include "storage.h"
#include "malloc.h"
#include "printf.h"
#include "strings.h"
#include "gpio.h"
#include "timer.h"

#define TIME_TOTAL 8000
#define TIME_DELAY 150

static unsigned int valves[4];

/**
 * Initialize valves
 * @param pin_1 gpio to control valve1
 * @param pin_2 gpio to control valve2
 * @param pin_3 gpio to control valve3
 * @param pin_4 gpio to control valve4
 */
void valves_init(unsigned int pin_1, unsigned int pin_2, unsigned int pin_3, unsigned int pin_4) {
    valves[0] = pin_1;
    valves[1] = pin_2;
    valves[2] = pin_3;
    valves[3] = pin_4;

    for (int i = 0; i < 4; i++) {
			gpio_set_output(valves[i]);
		}
}

/**
 * Turn on all valves
 */
void all_valves_on() {
    for(int valve = 0; valve < 4; valve++) valve_on(valve);
}

/**
 * Turn all valves off
 */
void all_valves_off() {
    for(int valve = 0; valve < 4; valve++) valve_off(valve);
}

/**
 * Turns individual valve on
 * @param valve number of valve
 */
void valve_on(int valve) {
    gpio_write(valves[valve], ON);
}

/**
 * Turns individual valve off
 * @param valve number of valve
 */
void valve_off(int valve) {
    gpio_write(valves[valve], OFF);å
}

/**
 * Turns on the valves for the correct amount to output the right drink
 * @param quantities array denoting ingredient concentrations
 */
void turn_on_valves(storage_ingredients_t *quantities) {
  // Initially turn on all values
  all_valves_on();

  int time_1 = quantities[0];
  int time_2 = quantities[1];
  int time_3 = quantities[2];
  int time_4 = quantities[3];

  while (time_1 > 0 || time_2 > 0 || time_3 > 0 || time_4 > 0) {

      printf("Valve state: %d/%d/%d/%d", time_1, time_2, time_3, time_4);

      // Turn off valves once they have reached a value of "1" or less,
      // since this means they have been fully exhausted.
      if (--time_1 <= 1) valve_off(VALVE_1);
      if (--time_2 <= 1) valve_off(VALVE_2);
      if (--time_3 <= 1) valve_off(VALVE_3);
      if (--time_4 <= 1) valve_off(VALVE_4);

      timer_delay_ms(TIME_DELAY);
  }

  printf("\n\n All valves done pouring!");
}
