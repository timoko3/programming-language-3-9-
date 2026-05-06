#ifndef CMD_SPU_H
#define CMD_SPU_H

#include "processor.h"

typedef double cmdParam_t;

typedef bool (*handler_t)(processor* spu);

typedef bool (*unaryHandler)(cmdParam_t arg,
                              cmdParam_t* result);
typedef bool (*binaryHandler)(cmdParam_t arg1, cmdParam_t arg2, 
                              cmdParam_t* result);

bool add(processor* spu);
bool sub(processor* spu);
bool mul(processor* spu);
bool myDiv(processor* spu);
bool mySqrt(processor* spu);
bool myAbs(processor* spu);

bool lt (processor* spu);
bool le (processor* spu);
bool gt (processor* spu);
bool ge (processor* spu);
bool eq (processor* spu);
bool neq(processor* spu);

bool jmp(processor* spu);
bool jl(processor* spu);
bool jle(processor* spu);
bool jg(processor* spu);
bool jge(processor* spu);
bool je(processor* spu);
bool jne(processor* spu);

bool push(processor* spu);
bool get(processor* spu);
bool set(processor* spu);
bool pushreg(processor* spu);

bool pushrt(processor* spu);
bool pushregrt(processor* spu);
bool popregrt(processor* spu);

bool popreg(processor* spu);
bool pushm(processor* spu);
bool popm(processor* spu);
bool pushmAddr(processor* spu);
bool popmAddr(processor* spu);

bool in(processor* spu);
bool out(processor* spu);
bool hlt(processor* spu);
bool callFunc(processor* spu);
bool retFunc(processor* spu);

bool draw(processor* spu);

#endif /* CMD_SPU_H */



