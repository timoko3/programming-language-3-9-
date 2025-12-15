#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "general/stack/stack.h"
#include "general/strFunc.h" 
#include "VM/ram.h"

typedef bool workStatus;

struct processor{
    stack stk;
    stack funcRetAddr;
    int* regs;
    size_t pc; 
    buffer_t opcode;
    workStatus isWork;
    ram_t* RAM;
};

enum processorStatus{
    SPU_PROCESS_OK,
    SPU_PROCESS_ERROR
};

bool spuPush(processor* spu, stackData_t data);
bool spuPop(processor* spu, stackData_t* data);
bool spuJump(processor* spu, stackData_t data);
bool spuGetArg(processor* spu, stackData_t* data);
bool spuPushReg(processor* spu, stackData_t* regNumber);
bool spuPopReg(processor* spu, stackData_t* regNumber);
bool spuPushM(processor* spu, stackData_t* memCellNum);
bool spuPopM(processor* spu, stackData_t* memCellNum);
bool spuHlt(processor* spu);
bool spuCall(processor* spu);
bool spuRet(processor* spu);

processorStatus processorCtor(processor* spu, ram_t* ram);
processorStatus processorDtor(processor* spu);
bool loadOpcode(processor* spu, buffer_t opcode);
bool runProcessor(processor* spu);
bool executeCommand(processor* spu);

void processorDump(processor* spu);

#endif /* PROCESSOR_H */