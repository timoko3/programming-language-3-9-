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
    // treeGraphDump(&AST);
    
    switch (typeBackend){
        #ifdef NASM
        case NASMBE: genCode(&AST, destFileName, genCodeNasm); break;
        #endif /* NASM */
        #ifdef SPU
        case SPUBE:  genCode(&AST, destFileName, genCodeSpu ); break;
        #endif /* SPU */
        #ifdef X86ELF
        case X86BE:  genCode(&AST, destFileName, genCodeX86ELF); break;
        #endif /* X86ELF */
        default: printf("компиляция данным способом не поддерживается!"); break;
    }
    
    treeDtor(&AST);   
}