#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h> // only used for testing

#define ERROR_PRINT 1

#define FNV_OFFSET 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3

typedef struct entry {
    char* key;
    char* value;
} Entry;

typedef struct {
    Entry** entries;
} HashTable;

size_t htSize = 0;

unsigned int nextPrime(unsigned int n);
void flush();

unsigned int hash(const char *key);
unsigned int hash2(const char *key);

Entry* createEntry(const char *key, const char *value);

HashTable* createHT();

void insertToHT(HashTable *hashtable, const char *key, const char *value);

char* getFromHT(HashTable *hashtable, const char *key);

void deleteFromHT(HashTable *hashtable, const char *key);

void printHT(HashTable *hashtable);

int main(int argc, char **argv) {

    int bufferSize = 10;
    char inVal[bufferSize];
    unsigned int numsToAdd = 0;
    unsigned int numsToDel = 0;
    unsigned int numsToFind = 0;

    struct timespec start, end;
    double cpuTimeUsed;

    FILE *timesFile;
    timesFile = fopen("Testing/preciseTimes.txt", "w");
    if (timesFile == NULL) {
        printf("Error opening file!\n");
    }

    unsigned int repeats = 0;
    unsigned int totalInsertNums = 0;
    unsigned int totalDeleteNums = 0;
    unsigned int totalFindNums = 0;
    scanf("%u", &repeats);

    scanf("%u", &numsToAdd);
    flush();
    htSize = nextPrime((repeats * numsToAdd) + (repeats * numsToAdd)/100);
    HashTable *ht = createHT();

    for (size_t r = 0; r < repeats; r++)
    {
        printf("---------- INSERTING: ----------\n");
        if(r != 0) {
            scanf("%u", &numsToAdd); // skip first
            flush();
        }
        totalInsertNums += numsToAdd;
        clock_gettime(CLOCK_MONOTONIC, &start);  // Start time
        for (size_t i = 0; i < numsToAdd; i++)
        {
            fgets(inVal, bufferSize, stdin);
            insertToHT(ht, inVal, inVal);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);    // End time
        cpuTimeUsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
        fprintf(timesFile, "insert: %u %f\n", totalInsertNums, cpuTimeUsed);
    }
    printHT(ht);
    for (size_t r = 0; r < repeats; r++)
    {
        printf("---------- DELETING: ----------\n");
        printf("\n");
        scanf("%u", &numsToDel);
        flush();
        totalDeleteNums += numsToDel;
        clock_gettime(CLOCK_MONOTONIC, &start);  // Start time
        for (size_t i = 0; i < numsToDel; i++)
        {
            fgets(inVal, bufferSize, stdin);
            deleteFromHT(ht, inVal);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);    // End time
        cpuTimeUsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
        fprintf(timesFile, "delete: %u %f\n", totalDeleteNums, cpuTimeUsed);
    }
    printHT(ht);
    for (size_t r = 0; r < repeats; r++)
    {
        printf("---------- FINDING: ----------\n");
        printf("\n");
        scanf("%u", &numsToFind);
        flush();
        totalFindNums += numsToFind;
        clock_gettime(CLOCK_MONOTONIC, &start);  // Start time
        for (size_t i = 0; i < numsToFind; i++)
        {
            fgets(inVal, bufferSize, stdin);
            char* getVal = getFromHT(ht, inVal);
            if(getVal == NULL) {
                printf("%s - false\n", inVal);
            }
            else if(strcmp(getFromHT(ht, inVal), inVal) == 0){
                printf("%s - true\n", inVal);
            }
            else{
                printf("%s - false\n", inVal);
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);    // End time
        cpuTimeUsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
        fprintf(timesFile, "find: %u %f\n", totalFindNums, cpuTimeUsed);
    }
    fclose(timesFile);

    return 0;
}

int isPrime(int n) {
    if (n % 2 == 0) return 0;

    for (int i = 3; i <= n/2; i += 2) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}
unsigned int nextPrime(unsigned int n){
    if (n <= 2) return 2;

    while (isPrime(n) == 0) {
        n++;
    }
    return n;
}

void flush(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// uses fnv1
unsigned int hash(const char *key) {
    uint64_t hashValue = FNV_OFFSET;
    for (size_t i = 0; i < strlen(key); i++)
    {
        hashValue *= FNV_PRIME;
        hashValue ^= key[i];
    }
    return hashValue % htSize;
}

// uses fnv1a
unsigned int hash2(const char *key){
    uint64_t hashValue = FNV_OFFSET;
    for (size_t i = 0; i < strlen(key); i++)
    {
        hashValue ^= key[i];
        hashValue *= FNV_PRIME;
    }
    return hashValue % htSize;
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
    unsigned int x = 1;
    unsigned int keyHash = hash(key);
    unsigned int index = keyHash;

    while(hashtable->entries[index] != NULL){
        printf("collision - %s\n", key);
        // check if has same key (yes == replace value)
        if (strcmp(hashtable->entries[index]->key, key) == 0){
            free(hashtable->entries[index]->value);
            hashtable->entries[index]->value = malloc(strlen(value) + 1);
            strcpy(hashtable->entries[index]->value, value);
            return;
        }
        index = (keyHash + x * hash2(key)) % htSize;
        x++;

        #if ERROR_PRINT == 1
        if(x > htSize) {
            printf("insert error - no free slot\n");
            break;
        }
        #endif
    }

    hashtable->entries[index] = createEntry(key, value);
}

char* getFromHT(HashTable *hashtable, const char *key) {
    unsigned int x = 1;
    unsigned int keyHash = hash(key);
    unsigned int index = keyHash;

    // should not be NULL
    while(hashtable->entries[index] != NULL){
        // check if has same key (yes == found)
        if (strcmp(hashtable->entries[index]->key, key) == 0){
            return hashtable->entries[index]->value;
        }
        // if no move to another index
        index = (keyHash + x * hash2(key)) % htSize;
        x++;

        // has checked every index
        if(x > htSize) {
            break;
        }
    }
    
    // did not find
    #if ERROR_PRINT == 1
    printf("get error - key not found\n");
    #endif
    return NULL;
}

void deleteFromHT(HashTable *hashtable, const char *key) {
    unsigned int x = 1;
    unsigned int keyHash = hash(key);
    unsigned int index = keyHash;

    // should not be NULL
    while(hashtable->entries[index] != NULL){
        // check if has same key (yes == delete)
        if (strcmp(hashtable->entries[index]->key, key) == 0){
            free(hashtable->entries[index]->key);
            free(hashtable->entries[index]->value);
            free(hashtable->entries[index]);
            hashtable->entries[index] = NULL;
            return;
        }
        // if no move to another index
        index = (keyHash + x * hash2(key)) % htSize;
        x++;

        // has checked every index
        if(x > htSize) {
            break;
        }
    }

    // did not find
    #if ERROR_PRINT == 1
    printf("delete error - key not found\n");
    #endif
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