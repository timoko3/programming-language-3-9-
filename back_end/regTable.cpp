#include "regTable.h"

#include "general/debug.h"
#include "general/poison.h"
#include "core/DSL.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

const size_t MAX_REG_NAME_LEN = 64;

regTableElem_t initRegTableNasm[] = {
    {RIP, "rip", INST_POINTER, true,   CALLER_SAVED},  
    {RAX, "rax", FUNC_RET_VAL, false,  CALLER_SAVED},  
    {RBX, "rbx", TEMP_STORE,   false,  CALLEE_SAVED},  
    {RDI, "rdi", FUNC_ARGS,    false,  CALLER_SAVED},  
    {RSI, "rsi", FUNC_ARGS,    false,  CALLER_SAVED},  
    {RDX, "rdx", FUNC_ARGS,    false,  CALLER_SAVED},  
    {RCX, "rcx", FUNC_ARGS,    false,  CALLER_SAVED},  
    {R8,  "r8",  FUNC_ARGS,    false,  CALLER_SAVED},  
    {R9,  "r9",  FUNC_ARGS,    false,  CALLER_SAVED},  
    {RSP, "rsp", STACK,        true,   CALLER_SAVED},  
    {RBP, "rbp", STACK,        true,   CALLEE_SAVED},  
    {R10, "r10", CALC,         false,  CALLER_SAVED},  
    {R11, "r11", CALC,         false,  CALLER_SAVED},  
    {R12, "r12", STORE_VAR,    false,  CALLEE_SAVED},  
    {R13, "r13", STORE_VAR,    false,  CALLEE_SAVED},  
    {R14, "r14", STORE_VAR,    false,  CALLEE_SAVED},  
    {R15, "r15", STORE_VAR,    false,  CALLEE_SAVED},  
    {XMM0,"xmm0", VEC_REG,     true ,  CALLEE_SAVED},  
};

regTableElem_t initRegTableSpu[] = {
    {AX, "AX", FUNC_RET_VAL,    false,  CALLER_SAVED},  
    {BX, "BX", TEMP_STORE,      false,  CALLEE_SAVED},  
    {CX, "CX", CALC,            false,  CALLER_SAVED},  
    {DX, "DX", CALC,            false,  CALLER_SAVED},  
    {EX, "EX", NOT_REG_SCEN,    false,  CALLER_SAVED},  
    {FX, "FX", NOT_REG_SCEN,    false,  CALLER_SAVED},  
    {GX, "GX", NOT_REG_SCEN,    false,  CALLER_SAVED},  
    {HX, "HX", NOT_REG_SCEN,    false,  CALLER_SAVED},  
    {IX, "IX", STACK,           true,   CALLER_SAVED},  
    {JX, "JX", STACK,           true,   CALLEE_SAVED},  
};

const size_t INIT_REG_TABLE_NASM_SIZE = sizeof(initRegTableNasm) / sizeof(regTableElem_t);
const size_t INIT_REG_TABLE_SPU_SIZE  = sizeof(initRegTableSpu)  / sizeof(regTableElem_t);

regTableElem_t* regTableElemCtor(genPurposeRegs reg, char* name, regUseScenery useScenery, bool isUsed){
    regTableElem_t* elem = (regTableElem_t*) calloc(1, sizeof(regTableElem_t));
    assert(elem);

    REG_TABLE_ELEM_NAME(elem) = (char*) calloc(MAX_REG_NAME_LEN, sizeof(char));
    assert(REG_TABLE_ELEM_NAME(elem));

    LPRINTF("name = %s\n", name);

    strcpy(REG_TABLE_ELEM_NAME(elem), name);

    LPRINTF("REG_TABLE_ELEM_NAME(elem) = %s\n", REG_TABLE_ELEM_NAME(elem));

    REG_TABLE_ELEM_REG(elem)           = reg;
    REG_TABLE_ELEM_USE_BIT(elem)       = isUsed;
    REG_TABLE_ELEM_USE_SCENERY(elem)   = useScenery;
    return elem;
}

void regTableInit(list_t* regTable, regTableElem_t* initRegTable, size_t sizeInitRegTable){
    assert(regTable);
    assert(initRegTable);

    for(size_t curElem = 0; curElem < sizeInitRegTable; curElem++){
        regTableElem_t* curRegTableElem = regTableElemCtor(REG_TABLE_ELEM_REG((&initRegTable[curElem])), 
                                                           REG_TABLE_ELEM_NAME((&initRegTable[curElem])),
                                                           REG_TABLE_ELEM_USE_SCENERY((&initRegTable[curElem])));
        LPRINTF("curElemAddr = %p", curRegTableElem);
        listInsertToTail(regTable, (void*) curRegTableElem);
        regTableElemDtor(curRegTableElem);
    }
    // listGraphDump(regTable);
}

void* regTableCopy(void* dest, void* src){
    if(dest == NULL || src == NULL) return NULL;

    regTableElem_t* destRegT = (regTableElem_t*) calloc(1, sizeof(regTableElem_t));
    assert(destRegT);

    regTableElem_t* srcRegT  = (regTableElem_t*) src;

    REG_TABLE_ELEM_REG(destRegT)           = REG_TABLE_ELEM_REG(srcRegT); 
    REG_TABLE_ELEM_USE_BIT(destRegT)       = REG_TABLE_ELEM_USE_BIT(srcRegT); 
    REG_TABLE_ELEM_USE_SCENERY(destRegT)   = REG_TABLE_ELEM_USE_SCENERY(srcRegT); 

    REG_TABLE_ELEM_NAME(destRegT) = (char*) calloc(MAX_REG_NAME_LEN, sizeof(char));
    strcpy(REG_TABLE_ELEM_NAME(destRegT), REG_TABLE_ELEM_NAME(srcRegT));

    return (void*) destRegT;
}

int regTableCmp(void* a, void* b){
    if(a == NULL && b == NULL) return 0;
    if(a == NULL || b == NULL) return 1;

    regTableElem_t* regTa  = (regTableElem_t*) a;
    regTableElem_t* regTb  = (regTableElem_t*) b;

    int result = 1;

    if((REG_TABLE_ELEM_REG(regTa) == REG_TABLE_ELEM_REG(regTb))){
        result = 0;
    }

    return result;
}

regTableElem_t* regTableFind(list_t* regTable, listCmpFunc_t findRule, regTableElem_t* refElem){
    assert(regTable);

    listCmpFunc_t saveCmp = regTable->cmpFunc;
    
    regTable->cmpFunc = findRule;

    int result = 0;
    listFind(regTable, (void**) refElem, &result);

    if(result == SEARCH_NOT_FOUND_VALUE) return NULL;

    regTable->cmpFunc = saveCmp;

    return (regTableElem_t*) regTable->elem[result].data;
}

void freeTypeRegs(list_t* regTable, regUseScenery scenery){
    assert(regTable);

    regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 1);

    regTableElem_t* foundReg = NULL;
    
    while(foundReg = regTableFind(regTable, findTypeRegFree, refReg)){
        REG_TABLE_ELEM_USE_BIT(foundReg) = 0;
    }

    regTableElemDtor(refReg);
}

int findTypeRegFree(void* a, void* b){
    
    regTableElem_t* regTa  = (regTableElem_t*) a;
    regTableElem_t* regTb  = (regTableElem_t*) b;
    
    int result = 1;

    if((REG_TABLE_ELEM_USE_BIT(regTa) == REG_TABLE_ELEM_USE_BIT(regTb)) &&
        REG_TABLE_ELEM_USE_SCENERY(regTa) == REG_TABLE_ELEM_USE_SCENERY(regTb)){
        result = 0;
    }

    return result;
}

int findIndRegRule(void* a, void* b){
    
    regTableElem_t* regTa  = (regTableElem_t*) a;
    regTableElem_t* regTb  = (regTableElem_t*) b;
    
    int result = 1;

    if((REG_TABLE_ELEM_REG(regTa) == REG_TABLE_ELEM_REG(regTb))){
        result = 0;
    }

    return result;
}

int findNameRegRule(void* a, void* b){
    
    regTableElem_t* regTa  = (regTableElem_t*) a;
    regTableElem_t* regTb  = (regTableElem_t*) b;
    
    int result = 1;

    if(!strcmp(REG_TABLE_ELEM_NAME(regTa), REG_TABLE_ELEM_NAME(regTb))){
        result = 0;
    }

    return result;
}

// int findVar(void* a, void* b){
//     regTableElem_t* regTa  = (regTableElem_t*) a;
//     regTableElem_t* regTb  = (regTableElem_t*) b;

//     int result = 1;

//     if((REG_TABLE_ELEM_VARIABLE_CODE(regTa) == REG_TABLE_ELEM_VARIABLE_CODE(regTb))){
//         result = 0;
//     }

//     return result;
// }

// int findTypeReg(void* a, void* b){
//     regTableElem_t* regTa  = (regTableElem_t*) a;
//     regTableElem_t* regTb  = (regTableElem_t*) b;

//     int result = 1;

//     if((REG_TABLE_ELEM_USE_SCENERY(regTa) == REG_TABLE_ELEM_USE_SCENERY(regTb))){
//         result = 0;
//     }

//     return result;
// }   


void regTableElemDtor(void* ptr){
    regTableElem_t* elem = (regTableElem_t*) ptr;

    free(REG_TABLE_ELEM_NAME(elem));

    poisonMemory(elem, sizeof(elem));
    free(elem);
}
