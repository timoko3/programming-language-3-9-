#include "emittersX86.h"
#include "nasmGeneration.h"

#include "core/DSL.h"

#include "general/file.h"
#include "general/hash.h"

#include <malloc.h>
#include <assert.h>

static void initContext(codeGenContext* context, FILE* asmFilePtr, list_t* regTable);
static void genPreamble(codeGenContext* context);
static void genEpilogue(codeGenContext* context);

void genAsmCode(tree_t* syntaxTree, const char* destFileName){
    assert(syntaxTree);

    fileDescription asmFile{
        destFileName,
        "wb"
    };

    FILE* asmFilePtr = myOpenFile(&asmFile);
    assert(asmFilePtr);

    codeGenContext context;
    list_t regTable;
    initContext(&context, asmFilePtr, &regTable);

    genPreamble(&context);
    emitNode(syntaxTree->root, &context);
    genEpilogue(&context);

    listDtor(_CONTEXT_REG_TABLE(&context), regTableElemDtor);

    fclose(asmFilePtr);
}

static void initContext(codeGenContext* context, FILE* asmFilePtr, list_t* regTable){
    
    listCtor(regTable, AMOUNT_REGS, regTableCmp, regTableCopy);
    regTableInit(regTable);

    _CONTEXT_FILE_PTR(context) = asmFilePtr;
    _CONTEXT_REG_TABLE(context) = regTable;
    _CONTEXT_BLOCK_IM_DEPTH(context) = 0;

    regTableElem_t* refReg = regTableElemCtor(NONE, "", STORE_VAR, PZN_VARIABLE_CODE, 0);
    assert(refReg);
    _CONTEXT_TEMP_REG(context) = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    REG_TABLE_ELEM_USE_BIT(_CONTEXT_TEMP_REG(context)) = 1;

    REG_TABLE_ELEM_USE_SCENERY(refReg) = CALC;
    _CONTEXT_CALC_REG_A(context) = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    REG_TABLE_ELEM_USE_BIT(_CONTEXT_CALC_REG_A(context)) = 1;

    _CONTEXT_CALC_REG_B(context) = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    REG_TABLE_ELEM_USE_BIT(_CONTEXT_CALC_REG_B(context)) = 1;

    regTableElemDtor(refReg);
}

static void genPreamble(codeGenContext* context){
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "section .text\n");
    fprintf(_CONTEXT_FILE_PTR(context), "global _start\n");
}

static void genEpilogue(codeGenContext* context){
    assert(context);

    // fprintf(_CONTEXT_FILE_PTR(context), "section .text\n");
    // fprintf(_CONTEXT_FILE_PTR(context), "global _start\n");
}