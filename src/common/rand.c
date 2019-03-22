#include "rand.h"
#include "storage.h"
#include "malloc.h"
#include "timer.h"

// Source: http://stackoverflow.com/questions/1167253/implementation-of-rand
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

unsigned int rand_under(unsigned int max) {

  return rand() % max;
}

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

unsigned char rand_char() {
  return (unsigned char) rand_under(256);
}

unsigned int flip_coin() {
  return (rand() % 2 == 0);
}

storage_ingredients_t * generate_random_mix(void) {
    storage_ingredients_t *ingredients = (storage_ingredients_t *) malloc(4);
    unsigned int total = 0;
    for (int i = 0; i < 4; i++) {
      ingredients[i] = rand_char();
      printf("%c", ingredients[i]);
      total += ingredients[i];
    }
    for (int i = 0; i < 4; i++) {
      ingredients[i] = ingredients[i] * 255 / total ;
    }

    return ingredients;
}
