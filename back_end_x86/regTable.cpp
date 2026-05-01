#include "regTable.h"

#include "core/DSL.h"

// void regTableInsert(hashTable_t* regTable, regTableElem_t)

void* regTableCopy(void* dest, void* src){
    regTableElem_t* destRegT = (regTableElem_t*) dest;
    regTableElem_t* srcRegT  = (regTableElem_t*) src;

    REG_TABLE_ELEM_REG(destRegT)           = REG_TABLE_ELEM_REG(srcRegT); 
    REG_TABLE_ELEM_USE_BIT(destRegT)       = REG_TABLE_ELEM_USE_BIT(srcRegT); 
    REG_TABLE_ELEM_USE_SCENERY(destRegT)   = REG_TABLE_ELEM_USE_SCENERY(srcRegT); 
    REG_TABLE_ELEM_VARIABLE_CODE(destRegT) = REG_TABLE_ELEM_VARIABLE_CODE(srcRegT); 

    return (void*) destRegT;
}

int regTableCmp(void* a, void* b){
    regTableElem_t* regTa  = (regTableElem_t*) a;
    regTableElem_t* regTb  = (regTableElem_t*) b;

    int result = 1;

    if((REG_TABLE_ELEM_REG(regTa) == REG_TABLE_ELEM_REG(regTb)) &&
        REG_TABLE_ELEM_VARIABLE_CODE(regTa) == REG_TABLE_ELEM_VARIABLE_CODE(regTb)){
        result = 0;
    }

    return result;
}