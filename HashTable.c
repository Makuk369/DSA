#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // only used for testing

typedef struct data{
    int value;
} Data;

unsigned int hash(const char* key, unsigned int htSize);

Data** createHashTable(unsigned int size);

void printHT(Data** hashTable, unsigned int htSize);

int main(){
    unsigned int htSize = 10;
    Data** hashTable = createHashTable(htSize);

    printHT(hashTable, htSize);

    // printf("%s - %u\n", "11", hash("11", htSize));

    return 0;
}

unsigned int hash(const char* key, unsigned int htSize){
    unsigned int keyLength = strlen(key);
    // printf("key len = %d\n", keyLength);
    unsigned int hashValue = 0;
    for (size_t i = 0; i < keyLength; i++)
    {
        hashValue += key[i];
        // printf("hashValue = %u\n", hashValue);
        hashValue *= key[i];
        // printf("post multi hashValue = %u\n", hashValue);
        hashValue %= htSize;
        // printf("final hashValue = %u\n", hashValue);
    }
    
    return hashValue;
}

Data** createHashTable(unsigned int size){
    Data** hashTable = malloc(sizeof(Data*) * size);

    for (size_t i = 0; i < size; i++)
    {
        hashTable[i] = NULL;
    }

    return hashTable;
}

void printHT(Data** hashTable, unsigned int htSize){
    for (size_t i = 0; i < htSize; i++)
    {
        if(hashTable[i] == NULL){
            printf("%u\t - NULL\n", i);
        }
        else{
            printf("%u\t - %d\n", i, hashTable[i]);
        }
    }
    
}