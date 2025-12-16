#include "emitters.h"

#include "../core/DSL.h"

#include "../general/file.h"

#include <assert.h>

const char* ASM_FILE_NAME = "program.asm";

void genAsmCode(tree_t* syntaxTree){
    assert(syntaxTree);

    fileDescription asmFile{
        "wb",
        ASM_FILE_NAME
    };

    FILE* asmFilePtr = myOpenFile(&asmFile);
    assert(asmFilePtr);

    codeGenContext context;

    _CONTEXT_FILE_PTR(&context) = asmFilePtr;
    labelsTableCtor (_CONTEXT_LABELS(&context));
    spuNameTableCtor(_CONTEXT_NAMES(&context));
    _CONTEXT_LABELS_AMOUNT(&context) = 0;

    emitNode(syntaxTree->root, &context);
}



