#ifndef REG_TABLE_H
#define REG_TABLE_H

#include "general/cashFriendlyList/list.h"

// const int PZN_VARIABLE_CODE = -1;


extern const size_t   INIT_REG_TABLE_NASM_SIZE;
extern const size_t   INIT_REG_TABLE_SPU_SIZE;

const size_t AMOUNT_REGS    = 16;

/*don't change regs order in enum!*/
enum genPurposeRegs{
    AX,
    BX,
    CX,
    DX,
    EX,
    FX,
    GX,
    HX,
    IX,
    JX,

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
    CALC,
    TEMP_STORE,
    STACK,
    STORE_VAR,
    ANY,
    NOT_REG_SCEN
};

enum callConvention_t{
    CALLEE_SAVED,
    CALLER_SAVED
};

struct regTableElem_t{
    genPurposeRegs   reg;
    char*            name;
    regUseScenery    useScenery;
    bool             useBit;
    callConvention_t callConvention;
};

extern regTableElem_t initRegTableNasm[];
extern regTableElem_t initRegTableSpu[];

regTableElem_t* regTableElemCtor(genPurposeRegs reg, char* name = "", regUseScenery useScenery = ANY, bool isUsed = false);

void regTableInit(list_t* regTable, regTableElem_t* initRegTable, size_t sizeInitRegTable);
void* regTableCopy(void* dest, void* src);
int regTableCmp(void* a, void* b);

regTableElem_t* regTableFind(list_t* regTable, listCmpFunc_t findRule, regTableElem_t* refElem);
void freeTypeRegs(list_t* regTable, regUseScenery scenery);
int findTypeRegFree(void* a, void* b);
int findIndRegRule(void* a, void* b);
int findVar(void* a, void* b);
// int findTypeReg(void* a, void* b);
int findNameRegRule(void* a, void* b);

void regTableElemDtor(void* ptr);

#endif  /* REG_TABLE_H */