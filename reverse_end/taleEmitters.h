#ifndef TALE_EMITTERS_H
#define TALE_EMITTERS_H

#include <stdio.h>

#include "../general/nametable.h"

#include "../core/expressionTree.h"

typedef void (*taleEmitter_t) (treeNode_t*, FILE*, variableScope scope, stack* nameTables);

taleEmitter_t getTaleEmitter(ASTnodeType type);
void taleEmitNode(treeNode_t* node, FILE* taleFilePtr, variableScope scope, stack* nameTables = NULL);

#endif /* TALE_EMITTERS_H */