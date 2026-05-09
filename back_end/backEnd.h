#ifndef BACK_END_H
#define BACK_END_H

#include "general/tree/treeExpImp.h"

enum backend_t{
    NASMBE,
    SPUBE
};

void compilerBackend(char* sourceFileName, char* destFileName, backend_t typeBackend);

#endif /* BACK_END_X86_H */