#ifndef REG_TABLE_H
#define REG_TABLE_H

#include "general/hashTable/hashTable.h"

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
    STORE_VAR
};

struct regTableElem_t{
    genPurposeRegs reg;
    regUseScenery  useBit     : 1;
    regUseScenery  useScenery : 7;
    int            variableCode;
};

#endif  /* REG_TABLE_H */