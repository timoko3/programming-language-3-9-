#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include "context.h"
#ifdef NASM
#include "back_end_nasm/contextNasm.h"
#endif /* NASM */

#ifdef SPU
#include "back_end_spu/contextSpu.h"
#endif /* SPU */

#ifdef X86ELF
#include "back_end_x86elf/contextX86Elf.h"
#endif /* X86ELF */

typedef void (*generator_t) (FILE* , tree_t* , codeGenContext* , list_t* , list_t* , labelsTable_t* );

void genCode(tree_t* syntaxTree, const char* destFileName, generator_t generator);

#ifdef NASM
void genCodeNasm(FILE* filePtr, tree_t* AST, codeGenContext* context, list_t* varMap, list_t* regTable, labelsTable_t* labelsTable);
#endif NASM

#ifdef SPU
void genCodeSpu(FILE* filePtr, tree_t* AST, codeGenContext* context, list_t* varMap, list_t* regTable, labelsTable_t* labelsTable);
#endif /* SPU */

#ifdef X86ELF
void genCodeX86ELF(FILE* filePtr, tree_t* AST, codeGenContext* context, list_t* varMap, list_t* regTable, labelsTable_t* labelsTable);
#endif /* X86ELF */

#endif /* CODE_GENERATION_H */