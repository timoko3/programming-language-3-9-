#include "stack.h"

#define stackCtor(stk, capacity) if(stackCtor(stk, capacity) != PROCESS_OK) return stackSpu.error.type
#define stackPush(stk, value)    if(stackPush(stk, value)    != PROCESS_OK) return stackSpu.error.type
#define stackPop(stk, stkElem)   if(stackPop(stk, stkElem)   != PROCESS_OK) return stackSpu.error.type

int main(void){
    stack stackSpu = {0};
    stackCtor(&stackSpu, 10);

    stackDtor(&stackSpu);

    return 0;
}