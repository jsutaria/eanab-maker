#include "rand.h"
#include "storage.h"
#include "malloc.h"

unsigned int rand(void);

unsigned int rand_under(unsigned int max);

unsigned int rand_between(unsigned int a, unsigned int b);

unsigned char rand_char();

unsigned int flip_coin();

storage_ingredients_t * generate_random_mix(void);
