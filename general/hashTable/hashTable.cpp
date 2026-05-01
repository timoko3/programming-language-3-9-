#include "hashTable.h"
#include "DSL.h"

#include "general/debug.h"
#include "general/poison.h"

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

const size_t LIST_START_CAPACITY          = 3;

bool hashTableCtor(hashTable_t* hashTable, size_t capacity, listCmpFunc_t cmpFunc, listCopyFunc_t copyFunc){
    HASH_TABLE_CAPACITY(hashTable)        = capacity; 
    HASH_TABLE_AMOUNT_ELEMENTS(hashTable) = 0; 
    HASH_TABLE_FUNCTION(hashTable)        = gnuHash;

    HASH_TABLE_CELLS(hashTable)           = (hashTableCell_t*) calloc(capacity, sizeof(hashTableCell_t));
    assert(HASH_TABLE_CELLS(hashTable));

    for(size_t i = 0; i < capacity; i++){
        HASH_TABLE_CELLS(hashTable)[i].value.capacity = LIST_START_CAPACITY; 
        listCtor(&HASH_TABLE_CELLS(hashTable)[i].value, cmpFunc, copyFunc);
    }

    return true;
}

bool hashTableInsert(hashTable_t* hashTable, hashTableElem_t* elem, char* key, int* insertCellNum){
    assert(hashTable);
    assert(elem);
    assert(key);
    assert(insertCellNum);
    
    size_t cellNumber = HASH_TABLE_FUNCTION(hashTable) (key) % HASH_TABLE_CAPACITY(hashTable);
    *insertCellNum = cellNumber;
    LPRINTF("cellNumber = %llu", cellNumber);

    hashTableCell_t* curCell = &(HASH_TABLE_CELLS(hashTable)[cellNumber]);
    
    // check exists word
    int listSearchElemIndex = 0;
    listFind(&HASH_TABLE_CELL_VALUE(curCell), elem, &listSearchElemIndex);
    if(!(listSearchElemIndex == SEARCH_NOT_FOUND_VALUE)) return false;

    listInsertToTail(&HASH_TABLE_CELL_VALUE(curCell), elem);

    HASH_TABLE_AMOUNT_ELEMENTS(hashTable)++;

    LPRINTF("addrListInsFunc = %p", &curCell);

    LPRINTF("size = %llu\n", HASH_TABLE_CELL_VALUE(curCell).size);

    return true;
}

bool hashTableFind(hashTable_t* hashTable, hashTableElem_t* elem, char* key, int* findCellNum){
    assert(hashTable);
    assert(elem);
    assert(key);
    assert(findCellNum);

    *findCellNum = SEARCH_NOT_FOUND_VALUE;

    size_t cellNumber = HASH_TABLE_FUNCTION(hashTable) (key) % HASH_TABLE_CAPACITY(hashTable);

    LPRINTF("cellNumber = %llu", cellNumber);

    hashTableCell_t* curCell = &(HASH_TABLE_CELLS(hashTable)[cellNumber]);
    
    LPRINTF("addrListFindFunc = %p", curCell);

    int listSearchElemIndex = 0;
    if(listFind(&HASH_TABLE_CELL_VALUE(curCell), elem, &listSearchElemIndex)) *findCellNum = cellNumber;

    return true;
}

bool hashTableDtor(hashTable_t* hashTable){
    for(size_t i = 0; i < HASH_TABLE_CAPACITY(hashTable); i++){
        listDtor(&HASH_TABLE_CELLS(hashTable)[i].value);
    }

    free(HASH_TABLE_CELLS(hashTable));     
    HASH_TABLE_CELLS(hashTable) = NULL;
    
    poisonMemory(&HASH_TABLE_CAPACITY(hashTable),        sizeof(HASH_TABLE_CAPACITY(hashTable       )));
    poisonMemory(&HASH_TABLE_AMOUNT_ELEMENTS(hashTable), sizeof(HASH_TABLE_AMOUNT_ELEMENTS(hashTable)));
    poisonMemory(&HASH_TABLE_FUNCTION(hashTable),        sizeof(HASH_TABLE_FUNCTION(hashTable       )));


    return true;
}
