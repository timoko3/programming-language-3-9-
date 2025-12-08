#include "expressionTree.h"

#include <stdio.h>

enum TOKENS{
    NUMBER,
    IDENT,
    CYCLE,
    COND
};

treeNode_t* tokenize(FILE* codeFilePtr);