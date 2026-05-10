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
    MEM64
};

enum REX_t{
    REX_W
};

enum instr_t{
    MOV,
    PUSH
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


struct instructionInfo{
    instr_t instrType;
    size_t amountArgs;

    instrArg_t args[2];
};

void prepareInstructionEndcodeInfo(codeGenContext* context, instr_t instrType, size_t amountArgs, const char* strArg1, const char* strArg2);

void emitMov(codeGenContext* context, instructionInfo* instrInfo);

#endif /* EMIT_BINARY_COMMANDS_H */