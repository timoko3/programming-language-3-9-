#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "regTable.h"

#include "general/tree/tree.h"

// #include "labels.h"

const int MAX_IM_DEPTH = 64;

struct codeGenContext{
    FILE*           filePtr;
    list_t*         regTable;
    int             blockImmersionDepth;
    // size_t          stackFrameBase;
    // size_t          stackFrameOffset;
};

void genAsmCode(tree_t* syntaxTree, const char* destFileName);

#endif /* CODE_GENERATION_H */