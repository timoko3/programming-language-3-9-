#ifndef BACK_END_H
#define BACK_END_H

#include "general/tree/treeExpImp.h"

typedef void (*generator_t) (tree_t*, const char*);

void compilerBackend(char* sourceFileName, char* destFileName, generator_t codeGenerator);

#endif /* BACK_END_X86_H */