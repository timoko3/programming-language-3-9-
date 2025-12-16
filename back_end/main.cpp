#include "codeGeneration.h"
#include "tree/treeExpImp.h"
#include "../core/core.h"

#include "../general/devTools/protectionTree.h"

int main(){
    tree_t syntaxTree;
    treeCtor(&syntaxTree);

    
    treeRead(&syntaxTree, TREE_FILE_NAME);
    // logTree(&syntaxTree, "readed tree");
    
    genAsmCode(&syntaxTree);
    
    treeDtor(&syntaxTree);
}