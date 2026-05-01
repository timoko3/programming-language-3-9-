#include "regTable.h"

#include "general/poison.h"
#include "core/DSL.h"

#include <assert.h>
#include <malloc.h>

regTableElem_t initRegTable[] = {
    {RAX, FUNC_RET_VAL, false,  PZN_VARIABLE_CODE},  
    {RBX, STORE_VAR,    false,  PZN_VARIABLE_CODE},  
    {RCX, FUNC_ARGS,    false,  PZN_VARIABLE_CODE},  
    {RDX, FUNC_ARGS,    false,  PZN_VARIABLE_CODE},  
    {RDI, FUNC_ARGS,    false,  PZN_VARIABLE_CODE},  
    {RSI, FUNC_ARGS,    false,  PZN_VARIABLE_CODE},  
    {RSP, STACK,        true,   PZN_VARIABLE_CODE},  
    {RBP, STACK,        true,   PZN_VARIABLE_CODE},  
    {R8,  STORE_VAR,    false,  PZN_VARIABLE_CODE},  
    {R9,  STORE_VAR,    false,  PZN_VARIABLE_CODE},  
    {R10, STORE_VAR,    false,  PZN_VARIABLE_CODE},  
    {R11, STORE_VAR,    false,  PZN_VARIABLE_CODE},  
    {R12, STORE_VAR,    false,  PZN_VARIABLE_CODE},  
    {R13, STORE_VAR,    false,  PZN_VARIABLE_CODE},  
    {R14, STORE_VAR,    false,  PZN_VARIABLE_CODE},  
    {R15, STORE_VAR,    false,  PZN_VARIABLE_CODE},  
};

regTableElem_t* regTableElemCtor(genPurposeRegs reg, regUseScenery useScenery, int variableCode){
    regTableElem_t* elem = (regTableElem_t*) calloc(1, sizeof(regTableElem_t));
    assert(elem);

    REG_TABLE_ELEM_REG(elem)           = reg;
    REG_TABLE_ELEM_USE_BIT(elem)       = false;
    REG_TABLE_ELEM_USE_SCENERY(elem)   = useScenery;
    REG_TABLE_ELEM_VARIABLE_CODE(elem) = variableCode;
    return elem;
}

void regTableInit(hashTable_t* regTable){
    assert(regTable);

    for(size_t curElem = 0; curElem < sizeof(initRegTable) / sizeof(regTableElem_t); curElem++){
        regTableElem_t* curRegTableElem = regTableElemCtor(REG_TABLE_ELEM_REG((&initRegTable[curElem])), 
                                                           REG_TABLE_ELEM_USE_SCENERY((&initRegTable[curElem])), 
                                                           REG_TABLE_ELEM_VARIABLE_CODE((&initRegTable[curElem])));
        char curKey[3] = "";
        snprintf(curKey, 3, "%d", REG_TABLE_ELEM_REG((&initRegTable[curElem])));
        hashTableInsert(regTable, (void**) curRegTableElem, curKey, NULL);
    }
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