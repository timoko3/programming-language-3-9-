#include "stack.h"
#include "../poison.h"
#include "../hash.h"

#include <stdint.h>

#define ON_DEBUG_LEVEL_4(expression) if(DEBUG_LEVEL > 3){expression};


void* POISON_NUMBER = NULL;
// in .h
struct stackErrorDescription errors[]{ 
    {PROCESS_OK,                    "Все хорошо\n"},
    {CAPACITY_EXCEEDS_LIMIT,        "Значение capacity превышает максимально возможное\n"}, 
    {NULL_POINTER,                  "Указатели не должны быть нулевыми\n"},
    {CAPACITY_IS_ZERO,              "Capacity равно 0\n"}, 
    {BAD_MEMORY_ALLOCATION,         "Некорректное выделение памяти\n"},
    {SIZE_EXCEEDS_CAPACITY,         "Размер стека превышает объем выделяемой памяти\n"},
    {CANARY_TORTURE,                "Канарейка была замучена до смерти\n"},
    {EMPTY_STACK,                   "Стэк опустел и получение элемента не возможно\n"}, 
    {UNPLANNED_STACK_CHANGE,        "Стек несакнкционированно изменен извне\n"},
    {UNPLANNED_STACK_STRUCT_CHANGE, "Структура стека несанкционированно изменена\n"}
};

static void InitializeStackBuffer(stack* stk, size_t startStackInd);

#if DEBUG_LEVEL > 0

static stackError verifyStack(stack* stk, const char* function, const char* file, const int line);
static void assignErrorStruct(stack* stk, stackError type);

#endif /* DEBUG */

#if DEBUG_LEVEL > 1

static void setCanaryProtection(stack* stk);
static stackError canaryCheck(stack* stk);

#endif /* DEBUG */

#if DEBUG_LEVEL > 2

static bool genStackHash(stack* stk);
static stackError checkHash(stack* stk);

#endif /* DEBUG */

#if DEBUG_LEVEL > 3

void print_mem_hex(const void *ptr, size_t size);

#endif /* DEBUG */

stackError stackCtor(stack* stk, size_t capacity){
    assert(stk);
    assert(capacity > 0);
    assert(capacity < STACK_MAX_CAPACITY);

    #if DEBUG_LEVEL > 1
    stk->capacity = capacity + CANARY_PROTECTION_SIZE * 2;
    #else 
    stk->capacity = capacity;
    #endif /* DEBUG */

    #if DEBUG_LEVEL == 0
    stk->error = errors[0];
    #endif

    stk->size = 0;
    stk->data = (stackData_t*) calloc(stk->capacity, sizeof(stackData_t));
    assert(stk->data);

    InitializeStackBuffer(stk, 0);
    
    #if DEBUG_LEVEL > 1
    setCanaryProtection(stk);
    #endif

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    return PROCESS_OK;
}

stackError stackPush(stack* stk, stackData_t value){

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    #if DEBUG_LEVEL > 1
    if(stk->size == stk->capacity - CANARY_PROTECTION_SIZE * 2){
    #else   
    if(stk->size == stk->capacity){
    #endif
        #if DEBUG_LEVEL > 0
        verify
        #endif /* DEBUG */

        stk->capacity = (stk->capacity) * 2;
        stackData_t* temp = (stackData_t*) realloc(stk->data, (stk->capacity) * sizeof(stackData_t));
        assert(temp)    ;

        stk->data = temp;

        #if DEBUG_LEVEL > 0
        verify
        #endif /* DEBUG */
        
        InitializeStackBuffer(stk, stk->size + 1);

        #if DEBUG_LEVEL > 1
        setCanaryProtection(stk);
        #endif /* DEBUG */
    }

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    #if DEBUG_LEVEL > 1
    stk->data[stk->size + CANARY_PROTECTION_SIZE] = value;
    #else
    stk->data[stk->size] = value;
    #endif
    (stk->size)++;

    #if DEBUG_LEVEL > 2
    genStackHash(stk);
    #endif

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    return PROCESS_OK;
}

stackError stackPop(stack* stk, stackData_t* stackElem){
    assert(stackElem);

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    #if DEBUG_LEVEL > 1 
    if(stk->data[stk->size] == CANARY_PROTECTION_NUMBER){
        return EMPTY_STACK;
    }
    *stackElem = stk->data[stk->size];
    stk->data[stk->size] = POISON_NUMBER;
    #else

    if(stk->size <= 0){
        return EMPTY_STACK;
    }
    *stackElem = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = (stackData_t) POISON_NUMBER;
    #endif

    (stk->size)--;

    #if DEBUG_LEVEL > 2
    genStackHash(stk);
    #endif /* DEBUG */

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */
    
    return PROCESS_OK;
}

stackError stackGetTop(stack* stk, stackData_t* stackElem){
    assert(stk);

    *stackElem = stk->data[stk->size - 1];

    return PROCESS_OK;
}

stackError stackGetBottom(stack* stk, stackData_t* stackElem){
    assert(stk);

    *stackElem = stk->data[0];

    return PROCESS_OK;
}

stackError stackDtor(stack* stk){
    assert(stk);
    
    for(size_t curElemInd = 0; curElemInd < stk->size; curElemInd++){
        stk->data[curElemInd] = (stackData_t) POISON_NUMBER;           
    }
    stk->size = (size_t) rand();
    stk->capacity = (size_t) rand();
    poisonMemory(&stk->error, sizeof(stk->error));

    #if DEBUG_LEVEL > 1
    poisonMemory(&stk->canaryStatus, sizeof(stk->canaryStatus));
    #endif /* DEBUG */

    #if DEBUG_LEVEL > 2
    poisonMemory(&stk->hashData,   sizeof(stk->hashData));
    poisonMemory(&stk->hashStruct, sizeof(stk->hashStruct));
    #endif

    free(stk->data);
    stk->data = NULL;

    return PROCESS_OK;
}

static void InitializeStackBuffer(stack* stk, size_t startStackInd){
    assert(stk);

    for(size_t curStackElem = startStackInd; curStackElem < stk->capacity; curStackElem++){
        stk->data[curStackElem] = (stackData_t) POISON_NUMBER;
    }

    #if DEBUG_LEVEL > 2
    genStackHash(stk);
    #endif
}

#if DEBUG_LEVEL > 0

static stackError verifyStack(stack* stk, const char* function, const char* file, const int line){
    if(stk == NULL){
        printf("stk — нулевой указатель\n");  
    } 
    else{
        if(stk->data == NULL){  
            assignErrorStruct(stk, NULL_POINTER_LIST);
            printf("data — нулевой указатель\n");
        }

        else if(stk->capacity > (size_t) STACK_MAX_CAPACITY){
            assignErrorStruct(stk, CAPACITY_EXCEEDS_LIMIT);
        }
        else if(stk->capacity == 0){
            assignErrorStruct(stk, CAPACITY_IS_ZERO);
        }
        else if(stk->size > stk->capacity){
            assignErrorStruct(stk, SIZE_EXCEEDS_CAPACITY);
        }
        else if(malloc_usable_size(stk->data) != (sizeof(stackData_t) * stk->capacity)){
            assignErrorStruct(stk, BAD_MEMORY_ALLOCATION);
        }

        #if DEBUG_LEVEL > 1
        else if(canaryCheck(stk) != PROCESS_OK && stk->canaryStatus == true);
        #endif

        #if DEBUG_LEVEL > 2
        else if(checkHash(stk) != PROCESS_OK);
        #endif

        else{
            assignErrorStruct(stk, PROCESS_OK);
            // return PROCESS_OK;
        }
    }
    
    dump(stk);
    return stk->error.type;
}

static void assignErrorStruct(stack* stk, stackError type){
    assert(stk);

    for(size_t curErrInd = 0; curErrInd < sizeof(errors) / sizeof(stackErrorDescription); curErrInd++){
        if(errors[curErrInd].type == type){
            stk->error = errors[curErrInd];
        }
    }
}

void stackDump(stack* stk, const char* function, const char* file, const int line){
    if(stk == NULL){
        printf("Передача нулевого указателя недопустима\n");                                                                                                          
        return;
    }
    
    if(stk->error.type == PROCESS_OK){
        printf(SET_STYLE_FONT_GREEN"\nOK\n"RESET);
    }
    else{
        printf(SET_STYLE_FONT_RED"\nERROR: %s\n"RESET, stk->error.text);
    }

    printf("%s()\n", __FUNCTION__);
    printf("stack<int>[%p]\n", stk);

    printf("Called from function %s at file %s:" SET_STYLE_BOLD_FONT_PURPLE " line %d\n" RESET, function, file, line);

    printf("{\n");
    printf(SET_STYLE_BOLD_FONT_BLUE "\tsize = %lu\n" RESET, stk->size);
    printf(SET_STYLE_BOLD_FONT_BLUE "\tcapacity = %lu\n\n" RESET, stk->capacity);

    if(sizeof(stackData_t) * stk->capacity == malloc_usable_size(stk->data)){
        printf(SET_STYLE_BOLD_FONT_GREEN "\tНеобходимый размер блока data: %lu байт(-ов)\n" RESET, sizeof(stackData_t) * stk->capacity);
        printf(SET_STYLE_BOLD_FONT_GREEN "\tРазмер выделенного блока data: %lu байт(-ов)\n" RESET, malloc_usable_size(stk->data));
    }
    else{
        printf(SET_STYLE_BOLD_FONT_GREEN "\tНеобходимый размер блока data: %lu байт(-ов)\n" RESET, sizeof(stackData_t) * stk->capacity);
        printf( SET_STYLE_BOLD_FONT_RED"\tРазмер выделенного блока data:"RESET 
                SET_STYLE_BLINKING_FONT_RED" %lu байт(-ов)\n" RESET, malloc_usable_size(stk->data));
    }

    printf("\tdata[%p]\n", stk->data);
    printf("\t{\n");
    for(size_t curElemInd = 0; curElemInd < stk->capacity; curElemInd++){
        if(stk->data[curElemInd] == POISON_NUMBER) {
            printf("\t\t[%lu] = %d" SET_STYLE_FONT_RED" (POISON NUMBER)\n"RESET, curElemInd, stk->data[curElemInd]);
        }
        #if DEBUG_LEVEL > 1
        else if(stk->data[curElemInd] == CANARY_PROTECTION_NUMBER){
            printf("\t\t[%lu] = %d" SET_STYLE_FONT_YELLOW" (CANARY NUMBER)\n"RESET, curElemInd, stk->data[curElemInd]);
        }
        #endif
        else{
            printf("\t\t*[%lu] = %d\n", curElemInd, stk->data[curElemInd]);
        }
    }
    printf("\t}\n");
    printf("}\n");
}
#endif /* DEBUG */

#if DEBUG_LEVEL > 1
static void setCanaryProtection(stack* stk){
    assert(stk);

    for(size_t i = 0; i < CANARY_PROTECTION_SIZE; i++){
        stk->data[i] = CANARY_PROTECTION_NUMBER;
        stk->data[stk->capacity - 1 - i] = CANARY_PROTECTION_NUMBER;
    }

    stk->canaryStatus = true;

    #if DEBUG_LEVEL > 2
    genStackHash(stk);
    #endif
}

static stackError canaryCheck(stack* stk){
    assert(stk);

    for(size_t i = 0; i < CANARY_PROTECTION_SIZE; i++){
        if((stk->data[i] != CANARY_PROTECTION_NUMBER) || (stk->data[stk->capacity - 1 - i] != CANARY_PROTECTION_NUMBER)){
            assignErrorStruct(stk, CANARY_TORTURE);
            return CANARY_TORTURE;
        }
    }

    return PROCESS_OK;
}
#endif /* DEBUG */

#if DEBUG_LEVEL > 2

static bool genStackHash(stack* stk){
    assert(stk);
    
    #if DEBUG_LEVEL >= 4
    printf("Размер структуры staka: %d\n", sizeof(*stk));
    // ON_DEBUG_LEVEL_4(print_mem_hex(stk, sizeof(*stk));)
    #endif /* DEBUG */

    stk->hashStruct = hash(stk, sizeof(stk->data) + 2 * sizeof(size_t)); 
    stk->hashData   = hash(stk->data, sizeof(stk->data));

    #if DEBUG_LEVEL > 3
    printf("CurHash: %lu\n", hash);
    #endif /* DEBUG */

    return true;
}

static stackError checkHash(stack* stk){
    assert(stk);

    unsigned long curDataHash   = stk->hashData;
    unsigned long curStructHash = stk->hashStruct;
    genStackHash(stk);


    if(stk->hashData != curDataHash){
        assignErrorStruct(stk, UNPLANNED_STACK_CHANGE);
        return UNPLANNED_STACK_CHANGE;
    }
    else if(stk->hashStruct != curStructHash){
        assignErrorStruct(stk, UNPLANNED_STACK_STRUCT_CHANGE);
        return UNPLANNED_STACK_STRUCT_CHANGE;
    }

    return PROCESS_OK;
}

#if DEBUG_LEVEL > 3

void print_mem_hex(const void *ptr, size_t size) {
    const uint8_t *p = (const uint8_t*)ptr;
    for (size_t i = 0; i < size; ++i) {
        printf("%02X ", p[i]);
        if ((i+1) % 16 == 0) putchar('\n'); // перенос строки каждые 16 байт
    }
    if (size % 16 != 0) putchar('\n');
}

#endif /* DEBUG */

#endif /* DEBUG */
