#ifndef TREE_PROTECTION_H
#define TREE_PROTECTION_H

#define logTree(expression, text, ...) htmlLog(expression, __FILE__, __FUNCTION__, __LINE__, text, ##__VA_ARGS__)

#include <stdio.h>

void htmlLog(tree_t* expression, const char* callFileName, const char* callFuncName, int callLine,
                           const char* dumpDescription, ...);
void treeGraphDump(tree_t* expression);
// treeStatus verifyTree(tree_t* tree, const char* function, const char* file, const int line);

#endif /* TREE_PROTECTION_H */