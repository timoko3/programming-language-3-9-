#include "spuNames.h"

#include "../core/DSL.h"

#include "../general/strFunc.h"
#include "../general/debug.h"

#include <malloc.h>
#include <assert.h>

spuNameTable_t* spuNameTableCtor(spuNameTable_t* spuNameTable){
    spuNameTable->data = (spuName_t*) calloc(1, sizeof(spuName_t));
    assert(spuNameTable->data);

    spuNameTable->capacity = 1;
    spuNameTable->size = 0;
}

int getVarAddr(spuNameTable_t* spuNameTable, const char* name){
    assert(spuNameTable);
    assert(name);

    for(size_t curNameInd = 0; curNameInd < _SPU_NAME_TABLE_SIZE(spuNameTable); curNameInd++){
        if(isEqualStrings(_SPU_NAME_DATA_STR(spuNameTable->data[curNameInd]), name) ){
            return _SPU_NAME_ADDR(spuNameTable->data[curNameInd]);
        }
    }

    if(_SPU_NAME_TABLE_SIZE(spuNameTable) >= _SPU_NAME_TABLE_CAPACITY(spuNameTable)){
        reallocateSpuNameTable(spuNameTable);
    }

    int addr = _SPU_NAME_TABLE_SIZE(spuNameTable);

    size_t curSize = _SPU_NAME_TABLE_SIZE(spuNameTable);

    _SPU_NAME_DATA_STR(spuNameTable->data[curSize])  = myStrDup(name);
    _SPU_NAME_DATA_TYPE(spuNameTable->data[curSize]) = VARIABLE;
    _SPU_NAME_ADDR(spuNameTable->data[curSize])      = addr;

    _SPU_NAME_TABLE_SIZE(spuNameTable)++;

    return addr;
}

static int createVarAddr(spuNameTable_t* spuNameTable, const char* name){
    assert(spuNameTable);
    assert(name);
}

spuNameTable_t* reallocateSpuNameTable(spuNameTable_t* spuNameTable){
    assert(spuNameTable);

    LPRINTF("start reallocation");

    size_t startSize = _SPU_NAME_TABLE_CAPACITY(spuNameTable);

    name_t* temp = (name_t*) realloc(_SPU_NAME_TABLE_DATA(spuNameTable), _SPU_NAME_TABLE_CAPACITY(spuNameTable) * 2 * sizeof(spuName_t));
    assert(temp);

    _SPU_NAME_TABLE_DATA(spuNameTable) = temp;
    _SPU_NAME_TABLE_CAPACITY(spuNameTable) *= 2;

    LPRINTF("ended reallocation");

    return spuNameTable;
}