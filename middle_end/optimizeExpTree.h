#ifndef OPTIMIZE_EXP_TREE_H
#define OPTIMIZE_EXP_TREE_H

#include "../general/tree/tree.h"
#include "../core/expressionTree.h"

bool optimizeExpression(tree_t* derivative, treeNode_t* subTreeRoot);

#endif /* OPTIMIZE_EXP_TREE_H */