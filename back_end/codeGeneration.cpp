#include "codeGeneration.h"

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

    
}