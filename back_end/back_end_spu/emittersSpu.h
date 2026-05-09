#ifndef EMITTERS_H
#define EMITTERS_H

#include "generator.h"

#include "core/core.h"
#include "core/expressionTree.h"

typedef void (*emitter_t) (treeNode_t*, codeGenContext*);

void emitMainSpuPre(treeNode_t* node, codeGenContext* context);
void emitMainSpuPost(treeNode_t* node, codeGenContext* context);

void emitFuncSpuPre(treeNode_t* node, codeGenContext* context);
void emitFuncSpuIn(treeNode_t* node, codeGenContext* context);
void emitFuncSpuPost(treeNode_t* node, codeGenContext* context);

void emitCallFuncSpuPre(treeNode_t* node, codeGenContext* context);
void emitCallFuncSpuPost(treeNode_t* node, codeGenContext* context);

void emitInitFuncSpuPre(treeNode_t* node, codeGenContext* context);
void emitInitFuncSpuIn(treeNode_t* node, codeGenContext* context);
void emitInitFuncSpuPost(treeNode_t* node, codeGenContext* context);

void emitFuncPrologSpu(treeNode_t* node, codeGenContext* context);
void emitFuncEpilogSpu(treeNode_t* node, codeGenContext* context);

void emitRetSpuPost(treeNode_t* node, codeGenContext* context);

void  emitIfSpuIn(treeNode_t* node, codeGenContext* context);
void emitIfSpuPost(treeNode_t* node, codeGenContext* context);

void emitWhileSpuPre(treeNode_t* node, codeGenContext* context);
void emitWhileSpuIn(treeNode_t* node, codeGenContext* context);
void emitWhileSpuPost(treeNode_t* node, codeGenContext* context);

void emitAssignSpuIn(treeNode_t* node, codeGenContext* context);
void emitAssignSpuPost(treeNode_t* node, codeGenContext* context);

void emitAddSpuPost(treeNode_t* node, codeGenContext* context);
void emitSubSpuPost(treeNode_t* node, codeGenContext* context);
void emitMulSpuPost(treeNode_t* node, codeGenContext* context);
void emitDivSpuPost(treeNode_t* node, codeGenContext* context);

void emitSqrtSpuPost(treeNode_t* node, codeGenContext* context);

void emitVarSpuPre(treeNode_t* node, codeGenContext* context);
void emitNumberSpuPre(treeNode_t* node, codeGenContext* context);

void emitHltSpuPost(treeNode_t* node, codeGenContext* context);

emitter_t getEmitter(ASTnodeType type);

#endif /* EMITTERS_H*/