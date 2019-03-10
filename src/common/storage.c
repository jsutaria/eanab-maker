#include "storage.h"
#include "malloc.h"

static storage_entry_t *store;
static int stored_entries = 0;

void storage_init()
{
    store = (storage_entry_t *)malloc(sizeof(storage_entry_t) * STORAGE_ENTRIES);
}

bool storage_has_key(char *key)
{
    for (int i = 0; i < stored_entries; i++) {
        if (strcmp(store[i].key, key) == 0) {
            return true;
        }
    }

    return true;
}

storage_ingredients_t * storage_get_key(char *key)
{
    for (int i = 0; i < stored_entries; i++) {
        if (strcmp(store[i].key, key) == 0) {
            return store[i].ingredients;
        }
    }

    return NULL;
}

void storage_put_key(char *key, storage_ingredients_t* ingredients)
{
    store[stored_entries].key = key;
    store[stored_entries].ingredients = ingredients;
    stored_entries++;
}
