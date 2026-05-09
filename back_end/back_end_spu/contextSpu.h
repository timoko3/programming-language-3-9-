#ifndef CONTEXT_SPU_H
#define CONTEXT_SPU_H

#include "context.h"

void initContextSpu(codeGenContext* context, FILE* asmFilePtr, list_t* regTable, list_t* varMap, labelsTable_t* labelsTable);

#endif /* CONTEXT_SPU_H */