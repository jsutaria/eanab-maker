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

/**
 * Get what's stored at index
 * @param  key key to get
 * @return     what's stored there
 */
static int storage_get_index(char *key)
{
    for (int i = 0; i < stored_entries; i++) {
        if (strcmp(store[i].key, key) == 0) {
            return i;
        }
    }

    return -1;
}

/**
 * Initialize storage using malloc
 */
void storage_init()
{
    store = (storage_entry_t *)malloc(sizeof(storage_entry_t) * STORAGE_ENTRIES);
    load_from_fs();
}

/**
 * Check if key is in the storage
 * @param  key key to check
 * @return     if the key is stored
 */
bool storage_has_key(char *key)
{
    return storage_get_index(key) != -1;
}

/**
 * Get what's stored at key in storage
 * @param  key key to retrieve
 * @return     value stored at key
 */
storage_ingredients_t * storage_get_key(char *key)
{
    int i = storage_get_index(key);
    if (i == -1) return NULL;
    return store[i].ingredients;
}

/**
 * Puts a key drink combination in storage
 * @param key         key to map to ingredients
 * @param ingredients ingredients to store
 */
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
