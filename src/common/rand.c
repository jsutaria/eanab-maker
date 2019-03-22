#include "rand.h"
#include "storage.h"
#include "malloc.h"
#include "timer.h"

/**
 * Generates random 32-bit integer using random time of system
 * @return  32-bit random number
 */
unsigned int rand(void)
{
    unsigned int total = 0;
    for (int i = 0; i < 32; i++) {
      int new = timer_get_ticks() % 2;
      total += (new << i);
      timer_delay_us(1);
    }
    return total;
}

/**
 * Returns a random number under the given value (exclusive)
 * @param  max maximum value for rand
 * @return     random number under max
 */
unsigned int rand_under(unsigned int max) {

  return rand() % max;
}

/**
 * Returns a random number between the given values
 * @param  a bound for random number
 * @param  b bound for random number
 * @return   random number between a and b
 */
unsigned int rand_between(unsigned int a, unsigned int b) {
  // make b the larger of the two
  if (a == b) {
    return a;
  } else if (a > b) {
    unsigned int c = b;
    b = a;
    a = c;
  }

  return a + rand_under(b - a);
}

/**
 * Returns a random character
 * @return random character
 */
unsigned char rand_char() {
  return (unsigned char) rand_under(256);
}

/**
 * Returns a random binary (0 or 1, heads or tails)
 * @return the binary state
 */
unsigned int flip_coin() {
  return (rand() % 2 == 0);
}

/**
 * Generate a random concoction of ingredients for the drink maker
 * @return  array of ingredients with randomly generated drink components
 */
storage_ingredients_t * generate_random_mix(void) {
    storage_ingredients_t *ingredients = (storage_ingredients_t *) malloc(4);
    unsigned int total = 0;
    for (int i = 0; i < 4; i++) {
      ingredients[i] = rand_char();
      total += ingredients[i];
    }
    for (int i = 0; i < 4; i++) {
      ingredients[i] = ingredients[i] * 255 / total ;
    }

    return ingredients;
}
