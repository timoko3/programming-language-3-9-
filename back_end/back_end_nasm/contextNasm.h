#ifndef CONTEXT_NASM_H
#define CONTEXT_NASM_H

void initContextNasm(codeGenContext* context, FILE* asmFilePtr, list_t* regTable, list_t* varMap, labelsTable_t* labelsTable);

#endif /* CONTEXT_NASM_H */