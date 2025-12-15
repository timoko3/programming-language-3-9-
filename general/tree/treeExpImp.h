#ifndef TREE_EXP_IMP_H
#define TREE_EXP_IMP_H

#include "tree.h"

void treeWrite(tree_t* syntaxTree, const char* treeFileName);
void printPreOrder(const treeNode_t* node, FILE* stream);
void treeRead(tree_t* tree, const char* treeFileName);

#endif /* TREE_EXP_IMP_H */