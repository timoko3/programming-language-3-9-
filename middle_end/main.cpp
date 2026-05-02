#include "tree/treeExpImp.h"
#include "core/core.h"

#include "general/devTools/protectionTree.h"
#include "general/strFunc.h"

#include "optimizeExpTree.h"
#include "middleEnd.h"

#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#include <string.h>

const size_t MAX_FILE_NAME_SIZE = 64;

void cmdFlagsHandle(int argc, char* argv[], char* sourceFileName, char* destFileName);

int main(int argc, char* argv[]){
    char sourceFileName       [MAX_FILE_NAME_SIZE]  = "syntaxTree.txt";
    char destMiddleEndFileName [MAX_FILE_NAME_SIZE] = "syntaxTree.txt";

    cmdFlagsHandle(argc, argv, sourceFileName, destMiddleEndFileName);

    compilerMiddleEnd(sourceFileName, destMiddleEndFileName);
}

void cmdFlagsHandle(int argc, char* argv[], char* sourceFileName, char* destFileName){
    assert(argv);
    assert(sourceFileName);
    assert(destFileName);

    int opt;
    while((opt = getopt(argc, argv, "i:o:")) != -1){
        switch(opt){
        case 'i':
            strncpy(sourceFileName, optarg, MAX_FILE_NAME_SIZE);
            break;
        
        case 'o':
            strncpy(destFileName,   optarg, MAX_FILE_NAME_SIZE);
            break;

        default:
            break;
        }
    }
}