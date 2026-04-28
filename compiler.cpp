#include "front_end/lexicalAnalyze.h"
#include "front_end/syntaxAnalyze.h"

#include "general/debug.h"
#include "general/file.h"
#include "general/devTools/protectionTree.h"
#include "general/strFunc.h"
#include "general/tree/treeExpImp.h"

#include "compiler.h"

#include <assert.h>
#include <malloc.h>

void compilerFrontend(char* sourceFileName, char* destFileName){
    assert(sourceFileName);
    assert(destFileName);

    data_t code;
    parseStringsFile(&code, sourceFileName);
    char* curBufferPos = code.buffer;

    LPRINTF("opened codeFile for lexical analyze(tokenizing)");

    tokensSequence_t tokensSequence;
    tokenSequenceCtor(&tokensSequence);

    LPRINTF("tokenization start");
    tokenize(curBufferPos, &tokensSequence);
    LPRINTF("tokenization ended");

    tree_t AST;

    treeCtor(&AST);
    
    syntaxAnalyze(&AST, &tokensSequence);

    treeWrite(&AST, destFileName);

    treeGraphDump(&AST);

    treeDtor(&AST);

    free(code.strings);
    free(code.buffer);

    tokenSequenceDtor(&tokensSequence);
}

// void compilerBackendX86(char* sourceFileName, char* destFileName){
//     assert(sourceFileName);
//     assert(destFileName);

//     tree_t AST;
//     treeCtor(&AST);

//     treeRead(&AST, sourceFileName);
//     // logTree(&syntaxTree, "readed tree");
    
//     genAsmCode(&AST);
    
//     treeDtor(&AST);   
// }