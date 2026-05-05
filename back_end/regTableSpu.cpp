#include "regTable.h"

#include "general/debug.h"
#include "general/poison.h"
#include "core/DSL.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

const size_t MAX_REG_NAME_LEN = 5;

regTableElem_t initRegTable[] = {
    {RAX, "AX", STORE_VAR,    false,  CALLER_SAVED},  
    {RBX, "BX", TEMP_STORE,   false,  CALLEE_SAVED},  
    {RDI, "CX", CALC,         false,  CALLER_SAVED},  
    {RSI, "DX", CALC,         false,  CALLER_SAVED},  
    {RDX, "EX", STORE_VAR,    false,  CALLER_SAVED},  
    {RCX, "FX", STORE_VAR,    false,  CALLER_SAVED},  
    {R8,  "GX", STORE_VAR,    false,  CALLER_SAVED},  
    {R9,  "HX", STORE_VAR,    false,  CALLER_SAVED},  
    {RSP, "IX", STACK,        true,   CALLER_SAVED},  
    {RBP, "JX", STACK,        true,   CALLEE_SAVED},  
};

regTableElem_t* regTableElemCtor(genPurposeRegs reg, char* name, regUseScenery useScenery, bool isUsed){
    regTableElem_t* elem = (regTableElem_t*) calloc(1, sizeof(regTableElem_t));
    assert(elem);

    REG_TABLE_ELEM_NAME(elem) = (char*) calloc(MAX_REG_NAME_LEN, sizeof(char));
    assert(REG_TABLE_ELEM_NAME(elem));

    LPRINTF("name = %s\n", name);

    strcpy(REG_TABLE_ELEM_NAME(elem), name);

    LPRINTF("REG_TABLE_ELEM_NAME(elem) = %s\n", REG_TABLE_ELEM_NAME(elem));

    REG_TABLE_ELEM_REG(elem)           = reg;
    REG_TABLE_ELEM_USE_BIT(elem)       = isUsed;
    REG_TABLE_ELEM_USE_SCENERY(elem)   = useScenery;
    return elem;
}

void regTableInit(list_t* regTable){
    assert(regTable);

    for(size_t curElem = 0; curElem < sizeof(initRegTable) / sizeof(regTableElem_t); curElem++){
        regTableElem_t* curRegTableElem = regTableElemCtor(REG_TABLE_ELEM_REG((&initRegTable[curElem])), 
                                                           REG_TABLE_ELEM_NAME((&initRegTable[curElem])),
                                                           REG_TABLE_ELEM_USE_SCENERY((&initRegTable[curElem])));
        LPRINTF("curElemAddr = %p", curRegTableElem);
        listInsertToTail(regTable, (void*) curRegTableElem);
        regTableElemDtor(curRegTableElem);
    }
    // listGraphDump(regTable);
}

void* regTableCopy(void* dest, void* src){
    if(dest == NULL || src == NULL) return NULL;

    regTableElem_t* destRegT = (regTableElem_t*) calloc(1, sizeof(regTableElem_t));
    assert(destRegT);

    regTableElem_t* srcRegT  = (regTableElem_t*) src;

    REG_TABLE_ELEM_REG(destRegT)           = REG_TABLE_ELEM_REG(srcRegT); 
    REG_TABLE_ELEM_USE_BIT(destRegT)       = REG_TABLE_ELEM_USE_BIT(srcRegT); 
    REG_TABLE_ELEM_USE_SCENERY(destRegT)   = REG_TABLE_ELEM_USE_SCENERY(srcRegT); 

    REG_TABLE_ELEM_NAME(destRegT) = (char*) calloc(MAX_REG_NAME_LEN, sizeof(char));
    strcpy(REG_TABLE_ELEM_NAME(destRegT), REG_TABLE_ELEM_NAME(srcRegT));

    return (void*) destRegT;
}

int regTableCmp(void* a, void* b){
    if(a == NULL && b == NULL) return 0;
    if(a == NULL || b == NULL) return 1;

    regTableElem_t* regTa  = (regTableElem_t*) a;
    regTableElem_t* regTb  = (regTableElem_t*) b;

    int result = 1;

    if((REG_TABLE_ELEM_REG(regTa) == REG_TABLE_ELEM_REG(regTb))){
        result = 0;
    }

    return result;
}

regTableElem_t* regTableFind(list_t* regTable, listCmpFunc_t findRule, regTableElem_t* refElem){
    assert(regTable);

    listCmpFunc_t saveCmp = regTable->cmpFunc;
    
    regTable->cmpFunc = findRule;

    int result = 0;
    listFind(regTable, (void**) refElem, &result);

    if(result == SEARCH_NOT_FOUND_VALUE) return NULL;

    regTable->cmpFunc = saveCmp;

    return (regTableElem_t*) regTable->elem[result].data;
}

void freeTypeRegs(list_t* regTable, regUseScenery scenery){
    assert(regTable);

    regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 1);

    regTableElem_t* foundReg = NULL;
    
    while(foundReg = regTableFind(regTable, findTypeRegFree, refReg)){
        REG_TABLE_ELEM_USE_BIT(foundReg) = 0;
    }

    regTableElemDtor(refReg);
}

int findTypeRegFree(void* a, void* b){
    
    regTableElem_t* regTa  = (regTableElem_t*) a;
    regTableElem_t* regTb  = (regTableElem_t*) b;
    
    int result = 1;

    if((REG_TABLE_ELEM_USE_BIT(regTa) == REG_TABLE_ELEM_USE_BIT(regTb)) &&
        REG_TABLE_ELEM_USE_SCENERY(regTa) == REG_TABLE_ELEM_USE_SCENERY(regTb)){
        result = 0;
    }

    return result;
}

int findIndRegRule(void* a, void* b){
    
    regTableElem_t* regTa  = (regTableElem_t*) a;
    regTableElem_t* regTb  = (regTableElem_t*) b;
    
    int result = 1;

    if((REG_TABLE_ELEM_REG(regTa) == REG_TABLE_ELEM_REG(regTb))){
        result = 0;
    }

    return result;
}

// int findVar(void* a, void* b){
//     regTableElem_t* regTa  = (regTableElem_t*) a;
//     regTableElem_t* regTb  = (regTableElem_t*) b;

//     int result = 1;

//     if((REG_TABLE_ELEM_VARIABLE_CODE(regTa) == REG_TABLE_ELEM_VARIABLE_CODE(regTb))){
//         result = 0;
//     }

//     return result;
// }

// int findTypeReg(void* a, void* b){
//     regTableElem_t* regTa  = (regTableElem_t*) a;
//     regTableElem_t* regTb  = (regTableElem_t*) b;

//     int result = 1;

//     if((REG_TABLE_ELEM_USE_SCENERY(regTa) == REG_TABLE_ELEM_USE_SCENERY(regTb))){
//         result = 0;
//     }

//     return result;
// }   


void regTableElemDtor(void* ptr){
    regTableElem_t* elem = (regTableElem_t*) ptr;

    free(REG_TABLE_ELEM_NAME(elem));

    poisonMemory(elem, sizeof(elem));
    free(elem);
}
