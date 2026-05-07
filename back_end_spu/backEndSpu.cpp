#include "back_end_spu/spuGeneration.h"

#include "general/debug.h"
#include "general/file.h"
#include "general/devTools/protectionTree.h"
#include "general/strFunc.h"
#include "general/tree/treeExpImp.h"

#include "backEndSpu.h"

#include <assert.h>
#include <malloc.h>

void compilerBackendSpu(char* sourceFileName, char* destFileName){
    assert(sourceFileName);
    assert(destFileName);

    tree_t AST;
    treeCtor(&AST);

    treeRead(&AST, sourceFileName);
    treeGraphDump(&AST);
    
    genAsmCodeSpu(&AST, destFileName);
    
    treeDtor(&AST);   
}