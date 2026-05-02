#include "middleEnd.h"

#include "middle_end/optimizeExpTree.h"

#include "general/debug.h"
#include "general/file.h"
#include "general/devTools/protectionTree.h"
#include "general/strFunc.h"
#include "general/tree/treeExpImp.h"

#include <assert.h>
#include <malloc.h>


void compilerMiddleEnd(char* sourceFileName, char* destFileName){
    assert(sourceFileName);
    assert(destFileName);

    tree_t syntaxTree;
    treeCtor(&syntaxTree);

    treeRead(&syntaxTree, sourceFileName);
    logTree(&syntaxTree, "readed tree");
    
    optimizeExpression(&syntaxTree, syntaxTree.root);

    logTree(&syntaxTree, "optimized tree");
    treeWrite(&syntaxTree, destFileName);

    treeDtor(&syntaxTree);
}
