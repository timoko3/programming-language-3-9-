#ifndef EMITTERS_H
#define EMITTERS_H

#include "spuGeneration.h"

#include "core/core.h"
#include "core/expressionTree.h"

typedef void (*emitter_t) (treeNode_t*, codeGenContext*);

void emitNode(treeNode_t* node, codeGenContext* context);
emitter_t getEmitter(ASTnodeType type);

#endif /* EMITTERS_H*/