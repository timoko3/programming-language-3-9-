#include "emittersX86.h"

#include "core/DSL.h"

#include "general/file.h"
#include "general/hash.h"

#include <malloc.h>
#include <assert.h>

void genAsmCode(tree_t* syntaxTree, const char* destFileName){
    assert(syntaxTree);

    fileDescription asmFile{
        destFileName,
        "wb"
    };

    FILE* asmFilePtr = myOpenFile(&asmFile);
    assert(asmFilePtr);

    codeGenContext context;
    hashTable_t* regTable = (hashTable_t*) calloc(1, sizeof(hashTable_t));
    hashTableCtor(regTable, AMOUNT_REGS, regTableCmp, regTableCopy, regTableHash);

    _CONTEXT_FILE_PTR(&context) = asmFilePtr;
    _CONTEXT_REG_TABLE(&context) = regTable;

    emitNode(syntaxTree->root, &context);

    free(regTable);

    fclose(asmFilePtr);
}

void genPreamble(codeGenContext* context){
    assert(context);

    fprintf(context->filePtr, "section .text\n");
    fprintf(context->filePtr, "global _start\n");
}