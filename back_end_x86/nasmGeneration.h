#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "general/tree/tree.h"

// #include "labels.h"

struct codeGenContext{
    FILE*           filePtr;
    // labelsTable_t*  labels; 
    // size_t          stackFrameBase;
    // size_t          stackFrameOffset;
};

void genAsmCode(tree_t* syntaxTree, const char* destFileName);

#endif /* CODE_GENERATION_H */