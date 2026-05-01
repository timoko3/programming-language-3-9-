#ifndef REG_TABLE_H
#define REG_TABLE_H

#include "general/hashTable/hashTable.h"

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
    R15
};

enum regUseScenery{
    FUNC_ARGS,
    FUNC_RET_VAL,
    STACK,
    STORE_VAR
};

struct regTableElem_t{
    genPurposeRegs reg;
    regUseScenery  useScenery;
    bool           useBit;
    int            variableCode;
};

regTableElem_t* regTableElemCtor(genPurposeRegs reg, regUseScenery useScenery, int variableCode);
void* regTableCopy(void* dest, void* src);
int regTableCmp(void* a, void* b);
bool regTableElemDtor(regTableElem_t* elem);

#endif  /* REG_TABLE_H */