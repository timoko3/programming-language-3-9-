#include "regTable.h"

#include "general/poison.h"
#include "core/DSL.h"

#include <assert.h>
#include <malloc.h>

regTableElem_t* regTableElemCtor(genPurposeRegs reg, regUseScenery useScenery, int variableCode){
    regTableElem_t* elem = (regTableElem_t*) calloc(1, sizeof(regTableElem_t));
    assert(elem);

    REG_TABLE_ELEM_REG(elem)           = reg;
    REG_TABLE_ELEM_USE_BIT(elem)       = (regUseScenery) 0;
    REG_TABLE_ELEM_USE_SCENERY(elem)   = useScenery;
    REG_TABLE_ELEM_VARIABLE_CODE(elem) = variableCode;
    return elem;
}

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

bool regTableElemDtor(regTableElem_t* elem){
    free(elem);
    poisonMemory(elem, sizeof(elem));

    return true;
}