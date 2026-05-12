#ifndef EMITTERS_NASM
#define EMITTERS_NASM

#include "generator.h"

typedef void (*emitter_t) (treeNode_t*, codeGenContext*);

void emitStartNasm(treeNode_t* node, codeGenContext* context);
emitter_t getEmitter(ASTnodeType type);

void emitMainNasmPre(treeNode_t* node, codeGenContext* context);

void emitFuncNasmPre(treeNode_t* node, codeGenContext* context);
void emitFuncNasmIn(treeNode_t* node, codeGenContext* context);
void emitFuncNasmPost(treeNode_t* node, codeGenContext* context);

void emitCallFuncNasmPre(treeNode_t* node, codeGenContext* context);
void emitCallFuncNasmPost(treeNode_t* node, codeGenContext* context);

void emitCallFuncFreeArgRegsNasm(treeNode_t* node, codeGenContext* context);

void emitInitFuncNasmPre(treeNode_t* node, codeGenContext* context);
void emitInitFuncNasmIn(treeNode_t* node, codeGenContext* context);
void emitInitFuncNasmPost(treeNode_t* node, codeGenContext* context);

void emitRetNasmPre(treeNode_t* node, codeGenContext* context);
void emitRetNasmPost(treeNode_t* node, codeGenContext* context);

void emitFuncPrologNasm(treeNode_t* node, codeGenContext* context);
void emitFuncEpilog(treeNode_t* node, codeGenContext* context);

void emitIfNasmIn(treeNode_t* node, codeGenContext* context);
void emitIfNasmPost(treeNode_t* node, codeGenContext* context);

void emitWhileNasmPre(treeNode_t* node, codeGenContext* context);
void emitWhileNasmIn(treeNode_t* node, codeGenContext* context);
void emitWhileNasmPost(treeNode_t* node, codeGenContext* context);

void emitCmpNasmPost(treeNode_t* node, codeGenContext* context);

void emitAssignNasmPre(treeNode_t* node, codeGenContext* context);
void emitAssignNasmIn(treeNode_t* node, codeGenContext* context);
void emitAssignNasmPost(treeNode_t* node, codeGenContext* context);

void emitBinaryOpNasmIn(treeNode_t* node, codeGenContext* context);
void emitBinaryOpNasmPost(treeNode_t* node, codeGenContext* context);

void emitAddNasmPost(treeNode_t* node, codeGenContext* context);
void emitSubNasmPost(treeNode_t* node, codeGenContext* context);
void emitMulNasmPost(treeNode_t* node, codeGenContext* context);
void emitDivNasmPost(treeNode_t* node, codeGenContext* context);

void emitSqrtNasmPost(treeNode_t* node, codeGenContext* context);

void emitVarNasmPre(treeNode_t* node, codeGenContext* context);
void emitNumberNasmPre(treeNode_t* node, codeGenContext* context);

void emitHltNasmPre(treeNode_t* node, codeGenContext* context);

void emitInNasmPre(treeNode_t* node, codeGenContext* context);
void emitInNasmIn(treeNode_t* node, codeGenContext* context);

#endif /* EMITERS_NASM */