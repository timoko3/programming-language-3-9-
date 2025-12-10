#include "lexicalAnalyze.h"

#include "../general/debug.h"
#include "../general/file.h"
// #include "../general/tree.h"

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

const char* CODE_FILE_NAME = "beta.tale";

int main(){
    data_t code;
    parseStringsFile(&code, CODE_FILE_NAME);
    char* curBufferPos = code.buffer;

    LPRINTF("opened codeFile for lexical analyze(tokenizing)");

    tokensSequence_t tokensSequence;
    tokenSequenceCtor(&tokensSequence);

    LPRINTF("tokenization start");
    tokenize(curBufferPos, &tokensSequence);
    LPRINTF("tokenization ended");

    free(code.strings);
    free(code.buffer);

    tokenSequenceDtor(&tokensSequence);
}