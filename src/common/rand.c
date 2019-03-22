#include "rand.h"
#include "storage.h"
#include "malloc.h"
#include "timer.h"
// From http://stackoverflow.com/questions/1167253/implementation-of-rand
unsigned int rand(void)
{
    unsigned int total = 0;
    for (int i = 0; i < 32; i++) {
      int new = timer_get_ticks() % 2;
      total += (new << i);
      timer_delay_us(1);
    }
    return total;
    // static unsigned int z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
    // unsigned int b;
    //
    // b  = ((z1 << 6) ^ z1) >> 13;
    // z1 = ((z1 & 4294967294U) << 18) ^ b;
    // b  = ((z2 << 2) ^ z2) >> 27;
    // z2 = ((z2 & 4294967288U) << 2) ^ b;
    // b  = ((z3 << 13) ^ z3) >> 21;
    // z3 = ((z3 & 4294967280U) << 7) ^ b;
    // b  = ((z4 << 3) ^ z4) >> 12;
    // z4 = ((z4 & 4294967168U) << 13) ^ b;
    // return (z1 ^ z2 ^ z3 ^ z4);
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
