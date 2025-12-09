#include "lexicalAnalyze.h"

#include "../general/file.h"
#include "../general/strFunc.h"
#include "../general/debug.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

const size_t APPROXIMATE_WORD_SIZE = 10;

static token_t* isKeyword(char* curBufferPos);

token_t* tokenize(const char* codeFileName){
    assert(codeFileName);

    data_t code;
    parseStringsFile(&code, codeFileName);
    LPRINTF("opened codeFile for lexical analyze(tokenizing)");

    token_t* tokensSequence = (token_t*) calloc(APPROXIMATE_WORD_SIZE, sizeof(token_t));
    assert(tokensSequence);
    LPRINTF("successfully allocated memory for tokensSequence");
    
    char* curBufferPos = code.buffer;
    size_t curTokenInd = 0;
    while(*curBufferPos != '\0'){
        LPRINTF("cycle iteration of reading token");
        char curTokenStr[MAX_VARIABLE_SIZE] = "";

        LPRINTF("string starting with curBufferPos: %s", curBufferPos);

        LPRINTF("Read word: %s", curTokenStr);

        token_t* curToken = isKeyword(curBufferPos);
        if(curToken){
            LPRINTF("is keyword");
            copyTokenContent(&tokensSequence[curTokenInd], curToken);

            curBufferPos += strlen(curToken->nameString);

            curTokenInd++;
        }
        else{
            LPRINTF("NOT KEYWORD");
            utf8Shift(1, &curBufferPos);
        }        
    }
    
    LPRINTF("ended tokenization cycle");

    free(code.strings);

    return tokensSequence;
}

static token_t* isKeyword(char* curBufferPos){
    assert(curBufferPos);

    for(size_t curTokenInd = 0; curTokenInd < sizeof(tokens) / sizeof(token_t); curTokenInd++){
        LPRINTF("tokens[curTokenInd].nameString = %s, myStrLen(tokens[curTokenInd].nameString) = %d", tokens[curTokenInd].nameString,  myStrLen(tokens[curTokenInd].nameString));
        if(!strncmp(curBufferPos, tokens[curTokenInd].nameString, myStrLen(tokens[curTokenInd].nameString))){
            LPRINTF("Needed token found tokens[curTokenInd].nameString = %s", tokens[curTokenInd].nameString);
            return &tokens[curTokenInd];
        }
    }

    return NULL;
}