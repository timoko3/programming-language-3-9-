#ifndef VARIABLE_MAP_H
#define VARIABLE_MAP_H

#include "general/cashFriendlyList/list.h"
#include "regTable.h"

enum varLocationType{
    LOCK_REG,
    LOCK_STACK,
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

void* varMapCopy(void* dest, void* src);
int varMapCmp(void* a, void* b);

void varMapElemDtor(void* ptr);

#endif /* VARIABLE_MAP_H */