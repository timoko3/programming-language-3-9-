#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H

#include "core.h"

struct treeNode_t{
    ASTnodeData_t*  data;
    treeNode_t* right;
    treeNode_t* left;
    treeNode_t* parent;
};

bool freeExpressionNodeData(treeNode_t* node, bool withoutRoot, int depth);

#endif /* EXPRESSION_TREE_H */