#include "emitters.h"
#include "spuGeneration.h"

#include "core/DSL.h"

#include "general/file.h"
#include "general/hash.h"

#include <malloc.h>
#include <assert.h>

static void initContext(codeGenContext* context, FILE* asmFilePtr /*, list_t* regTable, list_t* varMap, labelsTable_t* labelsTable */);
static void genPreamble(codeGenContext* context);
static void genEpilogue(codeGenContext* context);

void genAsmCodeSpu(tree_t* syntaxTree, const char* destFileName){
    assert(syntaxTree);

    fileDescription asmFile{
        destFileName,
        "wb"
    };

    FILE* asmFilePtr = myOpenFile(&asmFile);
    assert(asmFilePtr);

    codeGenContext context;
    // list_t varMap;
    // list_t regTable;
    // labelsTable_t labelsTable;
    initContext(&context, asmFilePtr/*, &regTable, &varMap, &labelsTable*/);

    // genPreamble(&context);
    emitNode(syntaxTree->root, &context);
    // genEpilogue(&context);

    // listDtor(&varMap, varMapElemDtor);
    // listDtor(_CONTEXT_REG_TABLE(&context), regTableElemDtor);
    // listDtor(_CONTEXT_LABELS_TABLE(&context), labelDtor);

    fclose(asmFilePtr);
}

static void initContext(codeGenContext* context, FILE* asmFilePtr/*, list_t* regTable, list_t* varMap, labelsTable_t* labelsTable*/){
    
    // listCtor(regTable, AMOUNT_REGS, regTableCmp, regTableCopy);
    // regTableInit(regTable);

    // initVarMap(varMap, regTable);
   
    // _CONTEXT_TEMP_VAR(context) = (varMapElem_t*) *data(varMap, *tail(varMap));

    // listCtor(labelsTable, AMOUNT_LABELS, labelCmp, labelCopy);

    // _CONTEXT_FUNC_ARGS_AMOUNT(context)      = 0;
    // _CONTEXT_VAR_REG_USE_SCENERY(context)   = STORE_VAR;
    _CONTEXT_FILE_PTR(context)              = asmFilePtr;
    // _CONTEXT_VAR_MAP(context)               = varMap;
    // _CONTEXT_REG_TABLE(context)             = regTable;
    // _CONTEXT_LABELS_TABLE(context)          = labelsTable;
    // _CONTEXT_BLOCK_IM_DEPTH(context)        = 0;
    // _CONTEXT_STACK_OFFSET(context)          = VARIABLE_BYTES_SIZE;
    
    // _CONTEXT_TEMP_REG(context) = VARIABLE_MAP_LOC_REG(_CONTEXT_TEMP_VAR(context) );

    // regTableElem_t* refReg = regTableElemCtor(NONE, "", CALC, 0);
    // assert(refReg);

    // _CONTEXT_CALC_REG_A(context) = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    // REG_TABLE_ELEM_USE_BIT(_CONTEXT_CALC_REG_A(context)) = 1;

    // _CONTEXT_CALC_REG_B(context) = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    // REG_TABLE_ELEM_USE_BIT(_CONTEXT_CALC_REG_B(context)) = 1;

    // REG_TABLE_ELEM_USE_SCENERY(refReg) = FUNC_RET_VAL;
    // _CONTEXT_FUNC_RET_REG(context) = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);

    // regTableElemDtor(refReg);
}

static void genPreamble(codeGenContext* context){
    assert(context);

    // fprintf(_CONTEXT_FILE_PTR(context), "section .text\n");
    // fprintf(_CONTEXT_FILE_PTR(context), "global _start\n");
}

static void genEpilogue(codeGenContext* context){
    assert(context);


}