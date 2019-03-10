#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>

#define STORAGE_ENTRIES 1024 // parties shouldn't have >1024 unique drinkers... right?
#define STORAGE_USER_LENGTH 16
#define STORAGE_INGREDIENTS_LENGTH 10

// An ingredient is an unsigned value ranging from 0 (not included at all)
// to 255 (included, to the point of being 100% of beverage).
typedef unsigned char storage_ingredients_t;

typedef struct {
    char *key; // [STORAGE_USER_LENGTH]
    storage_ingredients_t *ingredients; // [STORAGE_INGREDIENTS_LENGTH]
} storage_entry_t;

/**
 * Initializes the storage module.
 */
void storage_init();

/**
 * Returns if a given user key is available in storage.
 */
bool storage_has_key(char *key);

/**
 * Fetches the stored ingredients list for a particular user key.
 */
storage_ingredients_t * storage_get_key(char *key);

/**
 * Updates storage to save passed ingredients list for given user key.
 */
void storage_put_key(char *key, storage_ingredients_t* ingredients);

#endif
