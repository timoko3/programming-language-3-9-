#include "spuNames.h"

#include "../core/DSL.h"

#include "../general/strFunc.h"
#include "../general/debug.h"

#include <malloc.h>
#include <assert.h>

static int createVarAddr(spuNameTable_t* spuNameTable, const char* name);
static spuNameTable_t* reallocateSpuNameTable(spuNameTable_t* spuNameTable);

spuNameTable_t* spuNameTableCtor(spuNameTable_t* spuNameTable){
    spuNameTable = (spuNameTable_t*) calloc(1, sizeof(spuNameTable_t));

    spuNameTable->data = (spuName_t*) calloc(1, sizeof(spuName_t));
    assert(spuNameTable->data);

    

    spuNameTable->capacity = 1;
    spuNameTable->size = 0;

    return spuNameTable;
}

int getVarAddr(spuNameTable_t* spuNameTable, const char* name){
    assert(spuNameTable);
    assert(name);

    LPRINTF("spuNameTable=%p data=%p size=%zu cap=%zu",
        spuNameTable,
        spuNameTable->data,
        spuNameTable->size,
        spuNameTable->capacity);

    for(size_t curNameInd = 0; curNameInd < _SPU_NAME_TABLE_SIZE(spuNameTable); curNameInd++){
        if(isEqualStrings(_SPU_NAME_DATA_STR(spuNameTable->data[curNameInd]), name) ){
            return _SPU_NAME_ADDR(spuNameTable->data[curNameInd]);
        }
    }

    return createVarAddr(spuNameTable, name);
}

static int createVarAddr(spuNameTable_t* spuNameTable, const char* name){
    assert(spuNameTable);
    assert(name);

    if(_SPU_NAME_TABLE_SIZE(spuNameTable) >= _SPU_NAME_TABLE_CAPACITY(spuNameTable)){
        reallocateSpuNameTable(spuNameTable);
    }

    int addr = _SPU_NAME_TABLE_SIZE(spuNameTable);

    size_t curSize = _SPU_NAME_TABLE_SIZE(spuNameTable);

    LPRINTF("WRITE slot addr=%p", &spuNameTable->data[curSize]);

    _SPU_NAME_DATA_STR(spuNameTable->data[curSize])  = myStrDup(name);
    _SPU_NAME_DATA_TYPE(spuNameTable->data[curSize]) = VARIABLE;
    _SPU_NAME_ADDR(spuNameTable->data[curSize])      = addr;

    _SPU_NAME_TABLE_SIZE(spuNameTable)++;

    return addr;
}

static spuNameTable_t* reallocateSpuNameTable(spuNameTable_t* spuNameTable){
    assert(spuNameTable);

    LPRINTF("start reallocation");

    size_t startSize = _SPU_NAME_TABLE_CAPACITY(spuNameTable);

    spuName_t* temp = (spuName_t*) realloc(_SPU_NAME_TABLE_DATA(spuNameTable), _SPU_NAME_TABLE_CAPACITY(spuNameTable) * 2 * sizeof(spuName_t));
    assert(temp);

    _SPU_NAME_TABLE_DATA(spuNameTable) = temp;
    _SPU_NAME_TABLE_CAPACITY(spuNameTable) *= 2;

    LPRINTF("ended reallocation");

    return spuNameTable;
}