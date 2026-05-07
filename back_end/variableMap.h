#ifndef VARIABLE_MAP_H
#define VARIABLE_MAP_H

#include "general/cashFriendlyList/list.h"
#include "regTable.h"

const int TEMP_VARIABLE_CODE = -33;

const size_t VARIABLE_BYTES_SIZE = 1;
const size_t LOCAL_VARIABLE_MAX_AMOUNT = 10;

enum varLocationType{
    LOCK_REG,
    LOCK_STACK,
    LOCK_ANY
};

struct varMapElem_t{
    int  variableCode;
    varLocationType locType; 
    
    union {
        regTableElem_t* reg;
        int             stackOffset;
    } loc;
};

list_t* initVarMap(list_t* varMap, list_t* regTable);

varMapElem_t* varMapElemCtor(int variableCode, varLocationType locType, regTableElem_t* reg = NULL, int stackOffset = 0);

varMapElem_t* varMapFind(list_t* varMap, listCmpFunc_t findRule, varMapElem_t* refElem);
varMapElem_t* varMapAddVar(list_t* varMap, list_t* regTable, int variableCode, int stackOffset, regUseScenery regUseScenery = STORE_VAR);

void* varMapCopy(void* dest, void* src);
int varMapCmp(void* a, void* b);

void varMapElemDtor(void* ptr);

#endif /* VARIABLE_MAP_H */