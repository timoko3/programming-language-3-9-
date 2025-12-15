#ifndef TREE_H
#define TREE_H

#include "../core/expressionTree.h"

#include <stdio.h>

const size_t MAX_NODE_VALUE_SIZE = 512;

struct tree_t{ 
    treeNode_t*   root;
    size_t        amountNodes;
};

treeNode_t* treeCtor(tree_t* expression);
treeNode_t* treeDtor(tree_t* expression);

// void treeRead(const char* expressionFileName);

treeNode_t* createNewNode(treeNode_t* left, treeNode_t* right);
treeNode_t* createNewNodeStr(char* value, treeNode_t* left, treeNode_t* right);
treeNode_t* copyNode(treeNode_t* node);
bool setParent(treeNode_t* curNode);

void freeNode(treeNode_t* node, bool withoutRoot);
void freeLeftSubtree(treeNode_t* node,  bool withoutRoot);
void freeRightSubtree(treeNode_t* node, bool withoutRoot);

#endif /* TREE_H */