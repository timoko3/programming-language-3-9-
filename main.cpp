
#include "front_end/lexicalAnalyze.h"
#include "front_end/syntaxAnalyze.h"

#include "general/debug.h"
#include "general/file.h"
#include "general/devTools/protectionTree.h"
#include "general/strFunc.h"
#include "general/tree/treeExpImp.h"

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>

const size_t MAX_FILE_NAME_SIZE = 64;

void cmdFlagsHandle(int argc, char* argv[], char* sourceFileName, char* destFileName);

int main(int argc, char* argv[]){
    char sourceFilename[MAX_FILE_NAME_SIZE] = "examples/factorial.tale";
    char destFileName  [MAX_FILE_NAME_SIZE] = "syntaxTree.txt";

    cmdFlagsHandle(argc, argv, sourceFilename, destFileName);

    data_t code;
    parseStringsFile(&code, sourceFilename);
    char* curBufferPos = code.buffer;

    LPRINTF("isalpha('a') = %d, isalpha('и') = %d, isalpha(',') = %d", isalphaUtf8("a"), isalphaUtf8("и"), isalphaUtf8(","));

    LPRINTF("opened codeFile for lexical analyze(tokenizing)");

    tokensSequence_t tokensSequence;
    tokenSequenceCtor(&tokensSequence);

    LPRINTF("tokenization start");
    tokenize(curBufferPos, &tokensSequence);
    LPRINTF("tokenization ended");

    tree_t syntaxTree;

    treeCtor(&syntaxTree);
    
    syntaxAnalyze(&syntaxTree, &tokensSequence);

    treeWrite(&syntaxTree, destFileName);

    treeGraphDump(&syntaxTree);

    treeDtor(&syntaxTree);

    free(code.strings);
    free(code.buffer);

    tokenSequenceDtor(&tokensSequence);
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