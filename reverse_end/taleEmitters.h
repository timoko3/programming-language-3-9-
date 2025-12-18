#ifndef TALE_EMITTERS_H
#define TALE_EMITTERS_H

#include <stdio.h>

#include "../core/expressionTree.h"

typedef void (*taleEmitter_t) (treeNode_t*, FILE*);

taleEmitter_t getTaleEmitter(ASTnodeType type);
void taleEmitNode(treeNode_t* node, FILE* taleFilePtr);

#endif /* TALE_EMITTERS_H */