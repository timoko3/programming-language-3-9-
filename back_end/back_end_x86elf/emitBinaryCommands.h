#ifndef EMIT_BINARY_COMMANDS_H
#define EMIT_BINARY_COMMANDS_H

#include "context.h"
#include "general/binaryGenerator/binGen.h"
#include "regTable.h"

enum argInst_t{
    R64, 
    IMM,
    MEM
};

enum REX_t{
    REX_W
};


void emitMov(codeGenContext* context, argInst_t arg1, argInst_t arg2, genPurposeRegs reg1 = NONE, genPurposeRegs reg2 = NONE);

#endif /* EMIT_BINARY_COMMANDS_H */