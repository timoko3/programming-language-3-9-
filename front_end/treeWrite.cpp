#include "treeWrite.h"

#include "../general/file.h"

#include <assert.h>

const char* TREE_FILE_NAME = "syntaxTree.txt";

void treeWrite(tree_t* syntaxTree){
    assert(syntaxTree);

    fileDescription treeFile{
        TREE_FILE_NAME,
        "wb"
    };

    FILE* treeFilePtr = myOpenFile(&treeFile);
    assert(treeFilePtr);

    printPreOrder(syntaxTree->root, treeFilePtr);

    fclose(treeFilePtr);
}

void printPreOrder(const treeNode_t* node, FILE* stream){
    assert(node);
    assert(stream);

    fprintf(stream, "(");
    fprintf(stream, "\"%s\" ", node->writeFile);
    
    if(node->left){
        printPreOrder(node->left, stream);
    }
    else{
        fprintf(stream, "nil ");
    }

    if(node->right){
        printPreOrder(node->right, stream);
    }
    else{
        fprintf(stream, "nil ");
    }

    fprintf(stream, ")");
}