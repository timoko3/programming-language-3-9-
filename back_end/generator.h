#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "context.h"
#include "back_end_nasm/contextNasm.h"
#include "back_end_spu/contextSpu.h"

typedef void (*generator_t) (FILE* , tree_t* , codeGenContext* , list_t* , list_t* , labelsTable_t* );

void genCode(tree_t* syntaxTree, const char* destFileName, generator_t generator);

#ifdef NASM
void genCodeNasm(FILE* filePtr, tree_t* AST, codeGenContext* context, list_t* varMap, list_t* regTable, labelsTable_t* labelsTable);
#endif NASM

void genCodeSpu(FILE* filePtr, tree_t* AST, codeGenContext* context, list_t* varMap, list_t* regTable, labelsTable_t* labelsTable);

#endif /* CODE_GENERATION_H */