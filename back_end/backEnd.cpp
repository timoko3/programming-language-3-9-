#include "general/debug.h"
#include "general/file.h"
#include "general/devTools/protectionTree.h"
#include "general/strFunc.h"

#include "backEnd.h"

#include <assert.h>
#include <malloc.h>

void compilerBackend(char* sourceFileName, char* destFileName, generator_t codeGenerator){
    assert(sourceFileName);
    assert(destFileName);

    tree_t AST;
    treeCtor(&AST);

    treeRead(&AST, sourceFileName);
    treeGraphDump(&AST);
    
    codeGenerator(&AST, destFileName);
    
    treeDtor(&AST);   
}