#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "cashFriendlyList/list.h"
#include "general/hash.h"

#include <stddef.h>

const int SEARCH_NOT_FOUND_VALUE          = -1;

typedef hash_t (*hashFunction_t) (hashData_t str);

struct hashTableCell_t{
    list_t value;
};

struct hashTable_t{
    size_t           capacity;
    size_t           amountElements; // unique and duplicate
    hashFunction_t   hashFunction;
    hashTableCell_t* cells;
};

bool hashTableCtor(hashTable_t* hashTable, size_t capacity);
bool hashTableInsert(hashTable_t* hashTable, char* str);
bool hashTableFind(hashTable_t* hashTable, char* str, int* findCellNum);
bool hashTableDtor(hashTable_t* hashTable);


#endif /* HASH_TABLE_H */