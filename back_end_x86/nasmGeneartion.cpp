#include "emittersX86.h"

#include "core/DSL.h"

#include "general/file.h"


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

    _CONTEXT_FILE_PTR(&context) = asmFilePtr;

    emitNode(syntaxTree->root, &context);

    fclose(asmFilePtr);
}

void genPreamble(codeGenContext* context){
    assert(context);

    fprintf(context->filePtr, "section .text\n");
    fprintf(context->filePtr, "global _start\n");
}