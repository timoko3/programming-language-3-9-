#ifndef TREE_WRITE_H
#define TREE_WRITE_H

#include "../general/tree.h"

#include <stdio.h>

void treeWrite(tree_t* syntaxTree);
void printPreOrder(const treeNode_t* node, FILE* stream);

#endif /* TREE_WRITE_H */