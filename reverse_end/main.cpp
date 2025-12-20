#include "reverseEnd.h"
#include "tree/treeExpImp.h"
#include "../core/core.h"

#include "../general/devTools/protectionTree.h"

#include "../general/strFunc.h"

const char* TREE_FILE_NAME      = "syntaxTree.txt";
const char* TALE_CODE_FILE_NAME = "reversed.tale";
int main(int argc, char* argv[]){
    const char* treeFileName = TREE_FILE_NAME;
    const char* taleFileName = TALE_CODE_FILE_NAME;
    if(argc == 2){
        treeFileName = argv[1];
    }
    else if(argc == 3){
        treeFileName = argv[1];
        taleFileName = argv[2];
    }

    tree_t syntaxTree;
    treeCtor(&syntaxTree);

    treeRead(&syntaxTree, treeFileName);

    logTree(&syntaxTree, "reverseEnd");
    
    genTaleCode(&syntaxTree, taleFileName);
    
    treeDtor(&syntaxTree);

}