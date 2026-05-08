#include "general/debug.h"
#include "general/file.h"
#include "general/devTools/protectionTree.h"
#include "general/strFunc.h"

#include "backEnd.h"
#include "generator.h"

#include <assert.h>
#include <malloc.h>

void compilerBackend(char* sourceFileName, char* destFileName, backend_t typeBackend){
    assert(sourceFileName);
    assert(destFileName);

    tree_t AST;
    treeCtor(&AST);

    treeRead(&AST, sourceFileName);
    treeGraphDump(&AST);
    
    switch (typeBackend){
        case NASM: genCode(&AST, destFileName, genCodeNasm); break;
        case SPU:  genCode(&AST, destFileName, genCodeSpu ); break;
        default: printf("компиляция данным спосбом не поддерживается!"); break;
    }
    
    treeDtor(&AST);   
}