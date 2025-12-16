
#include "lexicalAnalyze.h"
#include "syntaxAnalyze.h"

#include "../general/debug.h"
#include "../general/file.h"
#include "../general/devTools/protectionTree.h"
#include "../general/strFunc.h"
#include "../general/tree/treeExpImp.h"

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

const char* CODE_FILE_NAME = "factorial.tale";

int main(int argc, char* argv[]){
    const char* filename = CODE_FILE_NAME;
    if(argc == 2){
        filename = argv[1];
    }

    data_t code;
    parseStringsFile(&code, filename);
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

    logTree(&syntaxTree, "created synTree");

    treeWrite(&syntaxTree, TREE_FILE_NAME);

    treeDtor(&syntaxTree);

    free(code.strings);
    free(code.buffer);

    tokenSequenceDtor(&tokensSequence);
}