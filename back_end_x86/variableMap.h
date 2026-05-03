#ifndef VARIABLE_MAP_H
#define VARIABLE_MAP_H

#include "general/cashFriendlyList/list.h"
#include "regTable.h"

enum varLocationType{
    LOCK_REG,
    LOCK_STACK,
    LOCK_ANY
};

struct varMapElem_t{
    int  variableCode;
    varLocationType locType; 
    
    union {
        genPurposeRegs reg;
        int            stackOffset;
    } loc;
};

varMapElem_t* varMapElemCtor(int variableCode, varLocationType locType, genPurposeRegs reg = NONE, int stackOffset = 0);

varMapElem_t* varMapFind(list_t* varMap, listCmpFunc_t findRule, varMapElem_t* refElem);
varMapElem_t* varMapAddVar(list_t* varMap, list_t* regTable, int variableCode, int stackOffset, regUseScenery regUseScenery = STORE_VAR);

void* varMapCopy(void* dest, void* src);
int varMapCmp(void* a, void* b);

void varMapElemDtor(void* ptr);

#endif /* VARIABLE_MAP_H */