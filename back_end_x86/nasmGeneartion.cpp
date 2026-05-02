#include "emittersX86.h"
#include "nasmGeneration.h"

#include "core/DSL.h"

#include "general/file.h"
#include "general/hash.h"

#include <malloc.h>
#include <assert.h>

void genPreamble(codeGenContext* context);
void genEpilogue(codeGenContext* context);

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
    listCtor(&regTable, AMOUNT_REGS, regTableCmp, regTableCopy);
    regTableInit(&regTable);

    _CONTEXT_FILE_PTR(&context) = asmFilePtr;
    _CONTEXT_REG_TABLE(&context) = &regTable;
    _CONTEXT_BLOCK_IM_DEPTH(&context) = 0;

    genPreamble(&context);
    emitNode(syntaxTree->root, &context);
    genEpilogue(&context);

    listDtor(&regTable, regTableElemDtor);

    fclose(asmFilePtr);
}

void genPreamble(codeGenContext* context){
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "section .text\n");
    fprintf(_CONTEXT_FILE_PTR(context), "global _start\n");
}

void genEpilogue(codeGenContext* context){
    assert(context);

    // fprintf(_CONTEXT_FILE_PTR(context), "section .text\n");
    // fprintf(_CONTEXT_FILE_PTR(context), "global _start\n");
}