#include "storage.h"
#include "strings.h"
#include "malloc.h"

static storage_entry_t *store;
static int stored_entries = 0;

static void load_from_fs(void)
{
    // fall through
}

static void update_fs(void)
{
    // fall through
}

static int storage_get_index(char *key)
{
    for (int i = 0; i < stored_entries; i++) {
        if (strcmp(store[i].key, key) == 0) {
            return i;
        }
    }

    return -1;
}

void storage_init()
{
    store = (storage_entry_t *)malloc(sizeof(storage_entry_t) * STORAGE_ENTRIES);
    load_from_fs();
}

bool storage_has_key(char *key)
{
    return storage_get_index(key) != -1;
}

storage_ingredients_t * storage_get_key(char *key)
{
    int i = storage_get_index(key);
    if (i == -1) return NULL;
    return store[i].ingredients;
}

void storage_put_key(char *key, storage_ingredients_t* ingredients)
{
    int i = storage_get_index(key);

    if (i != -1) {
        store[i].ingredients = ingredients;

    } else {
        store[stored_entries].key = key;
        store[stored_entries].ingredients = ingredients;
        stored_entries++;
    }

    update_fs();
}
