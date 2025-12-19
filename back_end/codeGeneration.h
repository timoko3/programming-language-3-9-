#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "../general/tree/tree.h"

#include "labels.h"
#include "spuNames.h"

struct codeGenContext{
    FILE*           filePtr;
    spuNameTable_t* names; 
    labelsTable_t*  labels; 
    size_t          stackFrameBase;
    size_t          stackFrameOffset;
};

void genAsmCode(tree_t* syntaxTree);

#endif /* CODE_GENERATION_H */