#ifndef REG_TABLE_H
#define REG_TABLE_H

#include "general/cashFriendlyList/list.h"

const int PZN_VARIABLE_CODE = -1;

const size_t AMOUNT_REGS    = 16;

enum genPurposeRegs{
    RAX,
    RBX,
    RCX,
    RDX,
    RDI,
    RSI,
    RSP,
    RBP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    NONE
};

enum regUseScenery{
    FUNC_ARGS,
    FUNC_RET_VAL,
    STACK,
    STORE_VAR
};

struct regTableElem_t{
    genPurposeRegs reg;
    char*          name;
    regUseScenery  useScenery;
    bool           useBit;
    int            variableCode;
};

regTableElem_t* regTableElemCtor(genPurposeRegs reg, char* name, regUseScenery useScenery, int variableCode, bool isUsed = false);

void regTableInit(list_t* regTable);
void* regTableCopy(void* dest, void* src);
int regTableCmp(void* a, void* b);

regTableElem_t* regTableFind(list_t* regTable, listCmpFunc_t findRule, regTableElem_t* refElem);
int findFreeRegStoreValRule(void* a, void* b);

void regTableElemDtor(void* ptr);

#endif  /* REG_TABLE_H */