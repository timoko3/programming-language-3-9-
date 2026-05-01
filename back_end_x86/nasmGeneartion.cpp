#include "emittersX86.h"
#include "nasmGeneration.h"

#include "core/DSL.h"

#include "general/file.h"
#include "general/hash.h"

#include <malloc.h>
#include <assert.h>

void genPreamble(codeGenContext* context);

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

    genPreamble(&context);

    emitNode(syntaxTree->root, &context);

    listDtor(&regTable, regTableElemDtor);

    fclose(asmFilePtr);
}

void genPreamble(codeGenContext* context){
    assert(context);

    fprintf(context->filePtr, "section .text\n");
    fprintf(context->filePtr, "global _start\n");
}