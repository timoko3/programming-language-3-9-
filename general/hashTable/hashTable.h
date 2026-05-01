#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "cashFriendlyList/list.h"
#include "general/hash.h"

#include <stddef.h>

const int SEARCH_NOT_FOUND_VALUE          = -1;

typedef hash_t (*hashFunction_t) (hashData_t str);

typedef void* hashTableElem_t;

struct hashTableCell_t{
    list_t value;
};

struct hashTable_t{
    size_t           capacity;
    size_t           amountElements; // unique and duplicate
    hashFunction_t   hashFunction;
    hashTableCell_t* cells;
    // listCopyFunc_t   listCopyFunc;
    // listCmpFunc_t    listCmpFunc;
};

bool hashTableCtor  (hashTable_t* hashTable, size_t capacity,  listCmpFunc_t cmpFunc, listCopyFunc_t copyFunc);
bool hashTableInsert(hashTable_t* hashTable, hashTableElem_t* str, char* key, int* insertCellNum             );
bool hashTableFind  (hashTable_t* hashTable, hashTableElem_t* str, char* key, int* findCellNum               );
bool hashTableDtor  (hashTable_t* hashTable                                                                  );


#endif /* HASH_TABLE_H */