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
        #ifdef NASM
        case NASMBE: genCode(&AST, destFileName, genCodeNasm); break;
        #endif /* NASM */
        #ifdef SPU
        case SPUBE:  genCode(&AST, destFileName, genCodeSpu ); break;
        #endif /* SPU */
        default: printf("компиляция данным способом не поддерживается!"); break;
    }
    
    treeDtor(&AST);   
}