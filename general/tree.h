#ifndef TREE_H
#define TREE_H

#include "../expressionTree.h"

#include <stdio.h>

struct tree_t{ 
    treeNode_t*   root;
    size_t        amountNodes;
};

treeNode_t* treeCtor(tree_t* expression);
treeNode_t* treeDtor(tree_t* expression);

// void treeRead(const char* expressionFileName);
void treeWrite(tree_t* syntaxTree, const char* treeFileName);
void printPreOrder(const treeNode_t* node, FILE* stream);

treeNode_t* createNewNode(treeNode_t* left, treeNode_t* right);
treeNode_t* copyNode(treeNode_t* node);
bool setParent(treeNode_t* curNode);

void freeNode(treeNode_t* node, bool withoutRoot);
void freeLeftSubtree(treeNode_t* node,  bool withoutRoot);
void freeRightSubtree(treeNode_t* node, bool withoutRoot);

#endif /* TREE_H */