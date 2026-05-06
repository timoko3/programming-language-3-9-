#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "general/spu/generalSpu/stack/stack.h"
#include "general//spu/generalSpu/strFunc.h" 
#include "../ram.h"

typedef bool workStatus;

typedef double regParam_t;

struct processor{
    stack stk;
    stack funcRetAddr;
    regParam_t* regs;
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

bool spuGet(processor* spu, size_t elemIndex, stackData_t* data);
bool spuSet(processor* spu, size_t elemIndex, stackData_t data);

bool spuJump(processor* spu, stackData_t data);
bool spuGetArg(processor* spu, stackData_t* data);

bool spuPushReg(processor* spu, int* regNumber);
bool spuPopReg(processor* spu, int* regNumber);

bool spuPushRt(processor* spu, stackData_t data);
bool spuPopRt(processor* spu, stackData_t* data);
bool spuPushRegRt(processor* spu, int* regNumber);
bool spuPopRegRt(processor* spu, int* regNumber);

bool spuPushM(processor* spu, int* memCellNum);
bool spuPopM(processor* spu, int* memCellNum);

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