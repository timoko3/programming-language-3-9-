#include "nametable.h"

#include "../general/debug.h"
#include "../general/strFunc.h"

nametable_t* nameTableCtor(nametable_t* nametable){

    nametable->data = (name_t*) calloc(1, sizeof(name_t));
    assert(nametable->data);

    nametable->capacity = 1;
    nametable->size = 0;

    LPRINTF("successfully allocated memory for nameTable");

    return nametable;
}

nametable_t* nameTableDtor(nametable_t* nametable){
    assert(nametable);

    for(size_t curNameInd = 0; curNameInd < nametable->size; curNameInd++){
        free(&nametable->data[curNameInd]);
    }

    free(nametable->data);

    return NULL;
}

void nameTableAddElem(stack* nametables, char* stringName, nameType type){
    assert(nametables);
    assert(stringName);

    nametable_t* curNameTable;

    stackPop(nametables, (void**)&curNameTable);

    if(curNameTable->size <= curNameTable->capacity){
        reallocateNameTable(curNameTable);
    }
    
    myStrCpy(curNameTable->data[curNameTable->size].nameString, stringName);
    curNameTable->data[curNameTable->size].type = type;

    (curNameTable->size)++;
    
    stackPush(nametables, curNameTable);
}

bool checkExistsName(stack* nametables, char* name){
    assert(nametables);

    nametable_t* curNameTable;

    stackPop(nametables, (void**) &curNameTable);
    
    for(size_t curNameInd = 0; curNameInd < curNameTable->size; curNameInd++){
        if(nametables->size != 0){
            return checkExistsName(nametables, name);
        }
        if(isEqualStrings(curNameTable->data[curNameInd].nameString, name)){
            stackPush(nametables, curNameTable);
            return true;
        }
    }

    stackPush(nametables, curNameTable);

    return false;
}

nametable_t* reallocateNameTable(nametable_t* nametable){
    assert(nametable);

    LPRINTF("start reallocation");

    size_t startSize = nametable->capacity;

    name_t* temp = (name_t*) realloc(nametable->data, nametable->capacity * 2 * sizeof(name_t));
    assert(temp);

    nametable->data = temp;
    nametable->capacity *= 2;

    initTokensSequence(nametable, startSize);

    LPRINTF("ended reallocation");

    return nametable;
}

void initTokensSequence(nametable_t* nametable, size_t startInd){
    assert(nametable);

    for(size_t curNameTableInd = startInd; curNameTableInd < nametable->capacity; curNameTableInd++){
        nametable->data = NULL;
    }
}

