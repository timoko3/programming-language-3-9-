#include "back_end_x86/nasmGeneration.h"

#include "general/debug.h"
#include "general/file.h"
#include "general/devTools/protectionTree.h"
#include "general/strFunc.h"
#include "general/tree/treeExpImp.h"

#include "backEndX86.h"

#include <assert.h>
#include <malloc.h>

void compilerBackendX86(char* sourceFileName, char* destFileName){
    assert(sourceFileName);
    assert(destFileName);

    tree_t AST;
    treeCtor(&AST);

    treeRead(&AST, sourceFileName);
    treeGraphDump(&AST);
    
    genAsmCode(&AST, destFileName);
    
    treeDtor(&AST);   
}