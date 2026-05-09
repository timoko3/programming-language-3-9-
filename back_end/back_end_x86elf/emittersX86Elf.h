#ifndef EMITTERS_X86
#define EMITTERS_X86

#include "generator.h"

typedef void (*emitter_t) (treeNode_t*, codeGenContext*);

emitter_t getEmitter(ASTnodeType type);

void emitMainX86ElfPre(treeNode_t* node, codeGenContext* context);

void emitFuncX86ElfPre(treeNode_t* node, codeGenContext* context);
void emitFuncX86ElfIn(treeNode_t* node, codeGenContext* context);
void emitFuncX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitCallFuncX86ElfPre(treeNode_t* node, codeGenContext* context);
void emitCallFuncX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitCallFuncFreeArgRegsX86Elf(treeNode_t* node, codeGenContext* context);

void emitInitFuncX86ElfPre(treeNode_t* node, codeGenContext* context);
void emitInitFuncX86ElfIn(treeNode_t* node, codeGenContext* context);
void emitInitFuncX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitRetX86ElfPre(treeNode_t* node, codeGenContext* context);
void emitRetX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitFuncPrologX86Elf(treeNode_t* node, codeGenContext* context);
void emitFuncEpilog(treeNode_t* node, codeGenContext* context);

void emitIfX86ElfIn(treeNode_t* node, codeGenContext* context);
void emitIfX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitWhileX86ElfPre(treeNode_t* node, codeGenContext* context);
void emitWhileX86ElfIn(treeNode_t* node, codeGenContext* context);
void emitWhileX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitCmpX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitAssignX86ElfPre(treeNode_t* node, codeGenContext* context);
void emitAssignX86ElfIn(treeNode_t* node, codeGenContext* context);
void emitAssignX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitBinaryOpX86ElfIn(treeNode_t* node, codeGenContext* context);
void emitBinaryOpX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitAddX86ElfPost(treeNode_t* node, codeGenContext* context);
void emitSubX86ElfPost(treeNode_t* node, codeGenContext* context);
void emitMulX86ElfPost(treeNode_t* node, codeGenContext* context);
void emitDivX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitSqrtX86ElfPost(treeNode_t* node, codeGenContext* context);

void emitVarX86ElfPre(treeNode_t* node, codeGenContext* context);
void emitNumberX86ElfPre(treeNode_t* node, codeGenContext* context);

void emitHltX86ElfPre(treeNode_t* node, codeGenContext* context);

#endif /* EMITERS_X86 */