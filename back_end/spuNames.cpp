// #include "spuNames.h"

// #include "../core/DSL.h"

// #include "../general/strFunc.h"
// #include "../general/debug.h"

// #include <malloc.h>
// #include <assert.h>

// const size_t GLOBAL_VARIABLE_SHIFT = 950;

// static int createGlobalVarAddr(spuNameTable_t* spuNameTable, const char* name);
// static int createLocaleVarAddr(spuNameTable_t* spuNameTable, const char* name, size_t* stackFrameOffset);
// static void createVarAddr(spuNameTable_t* spuNameTable, const char* name, int addr);

// static spuNameTable_t* reallocateSpuNameTable(spuNameTable_t* spuNameTable);

// spuNameTable_t* spuNameTableCtor(spuNameTable_t* spuNameTable){
//     spuNameTable = (spuNameTable_t*) calloc(1, sizeof(spuNameTable_t));

//     spuNameTable->data = (spuName_t*) calloc(1, sizeof(spuName_t));
//     assert(spuNameTable->data);

//     spuNameTable->capacity = 1;
//     spuNameTable->size = 0;

//     return spuNameTable;
// }

// spuNameTable_t* spuNameTableDtor(spuNameTable_t* spuNameTable){
//     assert(spuNameTable);

//     for(size_t curSpuNameInd = 0; curSpuNameInd < spuNameTable->size; curSpuNameInd++){
//         free(spuNameTable->data[curSpuNameInd].str);
//     }

//     free(spuNameTable->data);

//     free(spuNameTable);

//     return NULL;
// }

// int getLocalVarAddr(spuNameTable_t* spuNameTable, const char* name, size_t* stackFrameOffset){
//     assert(spuNameTable);
//     assert(name);

//     LPRINTF("зашел в getLocalVar");

//     for(size_t curNameInd = 0; curNameInd < _SPU_NAME_TABLE_SIZE(spuNameTable); curNameInd++){
//         if(isEqualStrings(_SPU_NAME_DATA_STR(spuNameTable->data[curNameInd]), name) && 
//            _SPU_NAME_SCOPE(spuNameTable->data[curNameInd]) == LOCALE){
//             LPRINTF("сущетвует такая переменная в getLocalVar");
            
//             return _SPU_NAME_ADDR(spuNameTable->data[curNameInd]);
//         }
//     }

//     return createLocaleVarAddr(spuNameTable, name, stackFrameOffset); 
// } 

// int getGlobalVarAddr(spuNameTable_t* spuNameTable, const char* name){ 
//     assert(spuNameTable);
//     assert(name);

//     LPRINTF("spuNameTable=%p data=%p size=%zu cap=%zu",
//         spuNameTable,
//         spuNameTable->data,
//         spuNameTable->size,
//         spuNameTable->capacity);

//     for(size_t curNameInd = 0; curNameInd < _SPU_NAME_TABLE_SIZE(spuNameTable); curNameInd++){
//         if(isEqualStrings(_SPU_NAME_DATA_STR(spuNameTable->data[curNameInd]), name) && 
//            _SPU_NAME_SCOPE(spuNameTable->data[curNameInd]) == GLOBAL){
//             LPRINTF("сущетвует такая переменная в getGloballVar");
//             return _SPU_NAME_ADDR(spuNameTable->data[curNameInd]);
//         }
//     }

//     return createGlobalVarAddr(spuNameTable, name);
// }

// static int createGlobalVarAddr(spuNameTable_t* spuNameTable, const char* name){
//     assert(spuNameTable);
//     assert(name);

//     int addr = _SPU_NAME_TABLE_SIZE(spuNameTable) + GLOBAL_VARIABLE_SHIFT;
//     size_t curSize = _SPU_NAME_TABLE_SIZE(spuNameTable);

//     createVarAddr(spuNameTable, name, addr);
    
//     _SPU_NAME_SCOPE(spuNameTable->data[curSize]) = GLOBAL;

//     LPRINTF("Создал новую глобальную переменную");

//     return addr;
// }

// static int createLocaleVarAddr(spuNameTable_t* spuNameTable, const char* name, size_t* stackFrameOffset){
//     assert(spuNameTable);
//     assert(name);
//     assert(stackFrameOffset);
    
//     LPRINTF("stackFrameOffset = %lu", *stackFrameOffset);
//     int addr = *stackFrameOffset;
//     size_t curSize = _SPU_NAME_TABLE_SIZE(spuNameTable);
    
//     createVarAddr(spuNameTable, name, addr);
    
//     _SPU_NAME_SCOPE(spuNameTable->data[curSize]) = LOCALE;
//     (*stackFrameOffset)++;

//     LPRINTF("Создал новую локальную переменную");

//     return addr;
// }

// static void createVarAddr(spuNameTable_t* spuNameTable, const char* name, int addr){
//     assert(spuNameTable);
//     assert(name);

//     if(_SPU_NAME_TABLE_SIZE(spuNameTable) >= _SPU_NAME_TABLE_CAPACITY(spuNameTable)){
//         reallocateSpuNameTable(spuNameTable);
//     }

//     size_t curSize = _SPU_NAME_TABLE_SIZE(spuNameTable);

//     _SPU_NAME_DATA_STR(spuNameTable->data[curSize])  = myStrDup(name);
//     _SPU_NAME_DATA_TYPE(spuNameTable->data[curSize]) = VARIABLE;
//     _SPU_NAME_ADDR(spuNameTable->data[curSize])      = addr;

//     _SPU_NAME_TABLE_SIZE(spuNameTable)++;
// }

// static spuNameTable_t* reallocateSpuNameTable(spuNameTable_t* spuNameTable){
//     assert(spuNameTable);

//     LPRINTF("start reallocation");

//     size_t startSize = _SPU_NAME_TABLE_CAPACITY(spuNameTable);

//     spuName_t* temp = (spuName_t*) realloc(_SPU_NAME_TABLE_DATA(spuNameTable), _SPU_NAME_TABLE_CAPACITY(spuNameTable) * 2 * sizeof(spuName_t));
//     assert(temp);

//     _SPU_NAME_TABLE_DATA(spuNameTable) = temp;
//     _SPU_NAME_TABLE_CAPACITY(spuNameTable) *= 2;

//     LPRINTF("ended reallocation");

//     return spuNameTable;
// }

