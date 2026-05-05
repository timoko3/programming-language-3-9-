#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

// #include "variableMap.h"
// #include "labels.h"

#include "general/tree/tree.h"

const int MAX_IM_DEPTH = 64;

struct codeGenContext{
    FILE*           filePtr;

    // list_t*         varMap;
    // varMapElem_t*   tempVar;
    // regUseScenery   curVarRegUseScenery;

    // list_t*         regTable;
    // regTableElem_t* tempReg;
    // regTableElem_t* funcRetReg;
    // regTableElem_t* calcRegA;
    // regTableElem_t* calcRegB;

    // labelsTable_t*  labelsTable;

    // int             stackOffset;
    // int             curFuncArgsAmount;

    // int             blockImmersionDepth;
};

void genAsmCodeSpu(tree_t* syntaxTree, const char* destFileName);

#endif /* CODE_GENERATION_H */