#ifndef CONTEXT_X86ELF_H
#define CONTEXT_X86ELF_H

#include "context.h"

void initContextX86Elf(codeGenContext* context, FILE* asmFilePtr, list_t* regTable, list_t* varMap, labelsTable_t* labelsTable, list_t* fixUpLabels);

#endif /* CONTEXT_NASM_H */