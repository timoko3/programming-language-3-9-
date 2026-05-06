#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <limits.h>
#include <stdlib.h>

#include "../genConsoleColors/consoleColors.h"

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif /* DEBUG_LEVEL */

#if DEBUG_LEVEL == 0
#define NDEBUG
#endif

typedef double stackData_t;

const double POISON_NUMBER = INT_MAX / 10;
const size_t STACK_MAX_CAPACITY = 1e9;

#if DEBUG_LEVEL > 1
const size_t  CANARY_PROTECTION_SIZE   = 1;  
const stackData_t CANARY_PROTECTION_NUMBER = INT_MAX / 20;
#endif /* DEBUG */

#if DEBUG_LEVEL > 0

#define verify if(verifyStack(stk, __FUNCTION__, __FILE__, __LINE__)!=PROCESS_OK)return stk->error.type;
#define dump(stk) stackDump(stk, function, file, line);

#endif /* DEBUG */

enum stackError{
    CAPACITY_EXCEEDS_LIMIT,
    NULL_POINTER,
    CAPACITY_IS_ZERO,
    BAD_MEMORY_ALLOCATION,
    SIZE_EXCEEDS_CAPACITY,
    CANARY_TORTURE,
    EMPTY_STACK,
    UNPLANNED_STACK_CHANGE,
    UNPLANNED_STACK_STRUCT_CHANGE,
    PROCESS_OK
};

struct stackErrorDescription{
    stackError type;
    const char* text;
};

struct stack{
    stackData_t* data;
    size_t size;
    size_t capacity;
    stackErrorDescription error;
    #if DEBUG_LEVEL > 1
    bool canaryStatus = false;
    #endif
    #if DEBUG_LEVEL > 2
    unsigned long hashStruct;
    unsigned long hashData;
    #endif
};

stackError stackCtor(stack* stk, size_t capacity);
stackError stackGet(stack* stk, size_t elemIndex, stackData_t* stackElem);
stackError stackSet(stack* stk, size_t elemIndex, stackData_t stackElem);
stackError stackPush(stack* stk, stackData_t value);
stackError stackPop(stack* stk, stackData_t* stackElem);
stackError stackShrink(stack* stk);
stackError stackExpand(stack* stk);
stackError stackDtor(stack* stk);
void stackDump(stack* stk, const char* function, const char* file, const int line);

#endif /* STACK_H */