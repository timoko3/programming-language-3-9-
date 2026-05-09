#ifndef CONTEXT_H
#define CONTEXT_H

#include "variableMap.h"
#include "labels.h"

#include "general/tree/tree.h"

#include "visitorAST.h"

#ifdef X86ELF
#include "back_end_x86elf/elfGenerate.h"
#endif /* X86ELF */

struct codeGenContext{
    FILE*           filePtr;

    list_t*         varMap;
    varMapElem_t*   tempVar;
    varMapElem_t*   curVar;
    bool            saveCurVar;
    regUseScenery   curVarRegUseScenery;

    bool            isLValue;
    bool            isFuncArg;
    list_t*         regTable;
    regTableElem_t* tempReg;
    regTableElem_t* funcRetReg;
    regTableElem_t* calcRegA;
    regTableElem_t* calcRegB;

    labelsTable_t*  labelsTable;
    label_t*        curLabelA;
    label_t*        curLabelB;

    int             stackOffset;
    int             stackShift;
    int             curFuncArgsAmount;
    bool            countArgs;

    int             blockImmersionDepth;

    #ifdef X86ELF
    elfGenerationContext_t elfGenContext;
    #endif /* X86ELF */
};

#endif /* CONTEXT_H */