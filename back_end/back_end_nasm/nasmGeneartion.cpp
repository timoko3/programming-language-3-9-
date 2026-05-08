#include "emittersNasm.h"
#include "nasmGeneration.h"

#include "core/DSL.h"

#include "general/file.h"
#include "general/hash.h"

#include <malloc.h>
#include <assert.h>

regTableElem_t initRegTable[] = {
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
};

static void initContext(codeGenContext* context, FILE* asmFilePtr, list_t* regTable, list_t* varMap, labelsTable_t* labelsTable);

void genAsmCodeX86(tree_t* syntaxTree, const char* destFileName){
    assert(syntaxTree);

    fileDescription asmFile{
        destFileName,
        "wb"
    };

    FILE* asmFilePtr = myOpenFile(&asmFile);
    assert(asmFilePtr);

    codeGenContext context;
    list_t varMap;
    list_t regTable;
    labelsTable_t labelsTable;
    initContext(&context, asmFilePtr, &regTable, &varMap, &labelsTable);

    emitStart(syntaxTree->root, &context);

    listDtor(&varMap, varMapElemDtor);
    listDtor(_CONTEXT_REG_TABLE(&context), regTableElemDtor);
    listDtor(_CONTEXT_LABELS_TABLE(&context), labelDtor);

    fclose(asmFilePtr);
}

static void initContext(codeGenContext* context, FILE* asmFilePtr, list_t* regTable, list_t* varMap, labelsTable_t* labelsTable){
    
    listCtor(regTable, AMOUNT_REGS, regTableCmp, regTableCopy);
    regTableInit(regTable, initRegTable, sizeof(initRegTable) / sizeof(regTableElem_t));

    initVarMap(varMap, regTable);
   
    _CONTEXT_TEMP_VAR(context) = (varMapElem_t*) *data(varMap, *tail(varMap));

    listCtor(labelsTable, AMOUNT_LABELS, labelCmp, labelCopy);

    _CONTEXT_FUNC_ARGS_AMOUNT(context)      = 0;
    _CONTEXT_VAR_REG_USE_SCENERY(context)   = STORE_VAR;
    _CONTEXT_FILE_PTR(context)              = asmFilePtr;
    _CONTEXT_VAR_MAP(context)               = varMap;
    _CONTEXT_REG_TABLE(context)             = regTable;
    _CONTEXT_LABELS_TABLE(context)          = labelsTable;
    _CONTEXT_BLOCK_IM_DEPTH(context)        = 0;
    _CONTEXT_STACK_OFFSET(context)          = VARIABLE_BYTES_SIZE;
    _CONTEXT_STACK_SHIFT(context)           = 1;
    _CONTEXT_IS_L_VALUE(context)            = 0;
    _CONTEXT_IS_FUNC_ARG(context)           = 0;
    
    _CONTEXT_TEMP_REG(context) = VARIABLE_MAP_LOC_REG(_CONTEXT_TEMP_VAR(context) );

    regTableElem_t* refReg = regTableElemCtor(NONE, "", CALC, 0);
    assert(refReg);

    _CONTEXT_CALC_REG_A(context) = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    REG_TABLE_ELEM_USE_BIT(_CONTEXT_CALC_REG_A(context)) = 1;

    _CONTEXT_CALC_REG_B(context) = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    REG_TABLE_ELEM_USE_BIT(_CONTEXT_CALC_REG_B(context)) = 1;

    REG_TABLE_ELEM_USE_SCENERY(refReg) = FUNC_RET_VAL;
    _CONTEXT_FUNC_RET_REG(context) = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);

    regTableElemDtor(refReg);
}

