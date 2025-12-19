#include "codeGeneration.h"
#include "tree/treeExpImp.h"
#include "../core/core.h"

#include "../general/devTools/protectionTree.h"

#include "../general/strFunc.h"
#include <stdio.h>

const char* TREE_FILE_NAME = "syntaxTree.txt";
int main(int argc, char* argv[]){
    const char* filename = TREE_FILE_NAME;
    if(argc == 2){
        filename = argv[1];
    }

    tree_t syntaxTree;
    treeCtor(&syntaxTree);

    treeRead(&syntaxTree, filename);
    logTree(&syntaxTree, "readed tree");
    
    genAsmCode(&syntaxTree);
    
    treeDtor(&syntaxTree);
}