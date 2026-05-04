#include "variableMap.h"

#include "general/debug.h"
#include "general/poison.h"
#include "core/DSL.h"

#include <assert.h>
#include <malloc.h>

list_t* initVarMap(list_t* varMap, list_t* regTable){
    listCtor(varMap, 3, varMapCmp, varMapCopy);

    regTableElem_t* refReg = regTableElemCtor(NONE, "", TEMP_STORE, 0);
    assert(refReg);
    regTableElem_t* foundReg = regTableFind(regTable, findTypeRegFree, refReg);        
    assert(foundReg);

    varMapElem_t* tempVar = varMapElemCtor(TEMP_VARIABLE_CODE, LOCK_REG, foundReg);
    listInsertToTail(varMap, tempVar);

    varMapElemDtor(tempVar);
    regTableElemDtor(refReg);

    return varMap;
}

varMapElem_t* varMapElemCtor(int variableCode, varLocationType locType, regTableElem_t* reg, int stackOffset){
    varMapElem_t* elem = (varMapElem_t*) calloc(1, sizeof(varMapElem_t));
    assert(elem);

    VARIABLE_MAP_VARIABLE_CODE(elem) = variableCode;
    VARIABLE_MAP_LOC_TYPE(elem)      = locType;

    switch (VARIABLE_MAP_LOC_TYPE(elem)){
        case LOCK_REG:   VARIABLE_MAP_LOC_REG(elem)          = reg;         break;
        case LOCK_STACK: VARIABLE_MAP_LOC_STACK_OFFSET(elem) = stackOffset; break;
        case LOCK_ANY:   break;
        default: printf("ошибка!"); break;
    }

    return elem;
}

void* varMapCopy(void* dest, void* src){
    if(dest == NULL || src == NULL) return NULL;

    varMapElem_t* destRegT = (varMapElem_t*) calloc(1, sizeof(varMapElem_t));
    assert(destRegT);

    varMapElem_t* srcRegT  = (varMapElem_t*) src;

    VARIABLE_MAP_VARIABLE_CODE(destRegT)   = VARIABLE_MAP_VARIABLE_CODE(srcRegT);
    VARIABLE_MAP_LOC_TYPE(destRegT)        = VARIABLE_MAP_LOC_TYPE(srcRegT); 

    switch (VARIABLE_MAP_LOC_TYPE(srcRegT)){
        case LOCK_REG:   VARIABLE_MAP_LOC_REG(destRegT)          = VARIABLE_MAP_LOC_REG(srcRegT);           break;
        case LOCK_STACK: VARIABLE_MAP_LOC_STACK_OFFSET(destRegT) = VARIABLE_MAP_LOC_STACK_OFFSET(srcRegT);  break;
        case LOCK_ANY:   break;
        default: printf("ошибка!"); break;
    }

    return (void*) destRegT;
}

int varMapCmp(void* a, void* b){
    if(a == NULL && b == NULL) return 0;
    if(a == NULL || b == NULL) return 1;

    varMapElem_t* vmpElemA  = (varMapElem_t*) a;
    varMapElem_t* vmpElemB  = (varMapElem_t*) b;

    int result = 1;

    if((VARIABLE_MAP_VARIABLE_CODE(vmpElemA) == VARIABLE_MAP_VARIABLE_CODE(vmpElemB))){
        result = 0;
    }

    return result;
}

varMapElem_t* varMapAddVar(list_t* varMap, list_t* regTable, int variableCode, int stackOffset, regUseScenery regUseScenery){
    assert(varMap);

    regTableElem_t* foundReg = NULL;

    if(regUseScenery != NOT_REG_SCEN){
        regTableElem_t* refReg = regTableElemCtor(NONE, "", regUseScenery, 0);
        assert(refReg);

        foundReg = regTableFind(regTable, findTypeRegFree, refReg);

        regTableElemDtor(refReg);
    }

    if(foundReg) printf("foundReg = %s\n", REG_TABLE_ELEM_NAME(foundReg));

    varMapElem_t* curVar = NULL;
    if(foundReg){
        curVar = varMapElemCtor(variableCode, LOCK_REG, foundReg);
        REG_TABLE_ELEM_USE_BIT(foundReg) = 1;
    }
    else{
        curVar = varMapElemCtor(variableCode, LOCK_STACK, NULL, stackOffset);
    }

    listInsertToTail(varMap, curVar);

    varMapElemDtor(curVar);

    return (varMapElem_t*) *data(varMap, *tail(varMap));
}

varMapElem_t* varMapFind(list_t* varMap, listCmpFunc_t findRule, varMapElem_t* refElem){
    assert(varMap);

    listCmpFunc_t saveCmp = varMap->cmpFunc;
    
    varMap->cmpFunc = findRule;

    int result = 0;
    listFind(varMap, (void**) refElem, &result);

    if(result == SEARCH_NOT_FOUND_VALUE) return NULL;

    varMap->cmpFunc = saveCmp;

    return (varMapElem_t*) varMap->elem[result].data;
}

void varMapElemDtor(void* ptr){
    varMapElem_t* elem = (varMapElem_t*) ptr;

    poisonMemory(elem, sizeof(elem));
    free(elem);
}

// genPurposeRegs getReg(varMapElem_t* variableMap) {
//     assert(variableMap->locType == LOCK_REG);

//     return variableMap->loc.reg;
// }

// int getStackOffset(varMapElem_t* variableMap) {
//     assert(variableMap->locType == LOCK_STACK);

//     return variableMap->loc.stackOffset;
// }


