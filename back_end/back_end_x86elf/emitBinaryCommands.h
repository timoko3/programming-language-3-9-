#ifndef EMIT_BINARY_COMMANDS_H
#define EMIT_BINARY_COMMANDS_H

#include "context.h"
#include "general/binaryGenerator/binGen.h"
#include "regTable.h"
#include "DSLbinCom.h"

enum argInst_t{
    R64, 
    RM64,
    IMM32,
    MEM64,
    LABEL,
    XMM,
    NONE_ARG
};

enum REX_t{
    REX_W
};

enum instr_t{
    MOV_I,
    PUSH_I,
    POP_I,
    CALL_I,
    RET_I,
    SYSCALL_I,

    JMP_I,
    JG_I,
    JGE_I,
    JL_I,
    JLE_I,
    JE_I,
    JNE_I,

    ADD_I,
    SUB_I,
    IMUL_I,
    IDIV_I,
    CMP_I,

    CVTSI2SS_I
};

struct  instrArg_t{
    argInst_t argType; 
    union {
        regTableElem_t* reg;
        int             number;
    } argValue;
    bool isMemCase;
    int memShift;
};

enum instrMode_t{
    R64TORM64,
    IMM32TORM64,
    MEM64TOR64,
    R64TOMEM64,
    RM64TOR64,
    IMM32TOMEM64,

    RM64MODE,
    MEM64MODE,
    IMM32MODE,

    LABELMODE,

    RM64TOXMM,

    NOMODE
};


struct instructionInfo{
    instr_t instrType;
    instrMode_t instrMode; 

    size_t amountArgs;



    instrArg_t args[2];
};

void prepareInstructionEndcodeInfo(codeGenContext* context, instr_t instrType, size_t amountArgs, const char* strArg1, const char* strArg2);

#endif /* EMIT_BINARY_COMMANDS_H */