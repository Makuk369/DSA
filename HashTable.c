#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct entry {
    char* key;
    char* value;
} Entry;

typedef struct {
    Entry** entries;
} HashTable;

size_t htSize = 0;

unsigned int hash(const char *key);

Entry* createEntry(const char *key, const char *value);

HashTable* createHT();

void insertToHT(HashTable *hashtable, const char *key, const char *value);

char* getFromHT(HashTable *hashtable, const char *key);

void deleteFromHT(HashTable *hashtable, const char *key);

void printHT(HashTable *hashtable);

int main(int argc, char **argv) {
    htSize = 8;
    HashTable *ht = createHT();

    insertToHT(ht, "123", "em");
    insertToHT(ht, "456", "russian");
    insertToHT(ht, "789", "pizza");
    insertToHT(ht, "741", "doge");
    insertToHT(ht, "852", "pyro");
    insertToHT(ht, "86", "joost");
    insertToHT(ht, "987", "kalix");

    printHT(ht);

    return 0;
}

unsigned int hash(const char *key) {
    unsigned long int value = 0;
    unsigned int key_len = strlen(key);

    for (size_t i = 0; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    // make sure value is 0 <= value < htSize
    value = value % htSize;

    return value;
}

Entry* createEntry(const char* key, const char* value) {
    Entry *entry = malloc(sizeof(Entry) * 1);
    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(value) + 1);

    // copy the key and value
    strcpy(entry->key, key);
    strcpy(entry->value, value);

    return entry;
}

HashTable* createHT() {
    HashTable *hashtable = malloc(sizeof(HashTable));

    // allocate table entries
    hashtable->entries = malloc(sizeof(Entry*) * htSize);

    // set each to null (needed for proper operation)
    for (size_t i = 0; i < htSize; ++i) {
        hashtable->entries[i] = NULL;
    }

    return hashtable;
}

void insertToHT(HashTable *hashtable, const char *key, const char *value) {
    unsigned int slot = hash(key);

    // try to look up an entry set
    Entry *entry = hashtable->entries[slot];

    // no entry means slot empty, insert immediately
    if (entry == NULL) {
        hashtable->entries[slot] = createEntry(key, value);
        return;
    }

    // walk through each entry until either the end is
    // reached or a matching key is found
    while (entry != NULL) {
        // check key
        if (strcmp(entry->key, key) == 0) {
            // match found, replace value
            free(entry->value);
            entry->value = malloc(strlen(value) + 1);
            strcpy(entry->value, value);
            return;
        }
    }
}

char* getFromHT(HashTable *hashtable, const char *key) {
    unsigned int slot = hash(key);

    // try to find a valid slot
    Entry *entry = hashtable->entries[slot];

    // no slot means no entry
    if (entry == NULL) {
        return NULL;
    }

    // return value if found
    if (strcmp(entry->key, key) == 0) {
        return entry->value;
    }

    // reaching here means there was entry but no key match
    return NULL;
}

void deleteFromHT(HashTable *hashtable, const char *key) {
    unsigned int slot = hash(key);

    // try to find a valid slot
    Entry *entry = hashtable->entries[slot];

    // no slot means no entry
    if (entry == NULL) {
        return;
    }

    // key found == delete
    if (strcmp(entry->key, key) == 0) {
        free(entry->key);
        free(entry->value);
        free(entry);
        hashtable->entries[slot] = NULL;

        return;
    }
}

void printHT(HashTable *hashtable) {
    for (int i = 0; i < htSize; ++i) {
        Entry *entry = hashtable->entries[i];

        if (entry == NULL) {
            continue;
        }

        printf("%d\t - %s\n", i, entry->value);
    }
}