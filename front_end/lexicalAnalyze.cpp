#include "lexicalAnalyze.h"
#include "protection.h"

#include "../general/file.h"
#include "../general/strFunc.h"
#include "../general/debug.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const size_t APPROXIMATE_WORD_SIZE = 10;

static token_t* getKeyword(char* curBufferPos);

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
        dumpTokenSequence(tokensSequence);
        LPRINTF("cycle iteration of reading token");

        LPRINTF("string starting with curBufferPos: %s", curBufferPos);

        token_t* curToken = getKeyword(curBufferPos);
        if(curToken){
            LPRINTF("is keyword");
            copyTokenContent(&tokensSequence[curTokenInd], curToken);

            curBufferPos += strlen(*tokenStrData(curToken));

        }
        else{
            LPRINTF("NOT KEYWORD");
            if(isdigit(*curBufferPos)){
                LPRINTF("start creating number token lexical");
                char* endNumBufferPos = NULL;

                int number = (int) strtol(curBufferPos, &endNumBufferPos, 10);
                LPRINTF("get number token value: %d", number);
                createNumberToken(&tokensSequence[curTokenInd], number);

                curBufferPos = endNumBufferPos;
            }
            else if(!isspace(*curBufferPos)){
                LPRINTF("start creating variable token lexical");
                int curBufferShift = 0;

                char curTokenValue[MAX_VARIABLE_SIZE] = "";
                sscanf(curBufferPos, "%s%n", curTokenValue, &curBufferShift);

                createVariableToken(&tokensSequence[curTokenInd], curTokenValue);

                curBufferPos += curBufferShift;
            }
            else{
                utf8Shift(1, &curBufferPos);
                continue;
            }
        }        
        curTokenInd++;
    }
    
    LPRINTF("ended tokenization cycle");

    free(code.strings);
    free(code.buffer);

    return tokensSequence;
}

static token_t* getKeyword(char* curBufferPos){
    assert(curBufferPos);

    for(size_t curTokenInd = 0; curTokenInd < TOKENS_COUNT; curTokenInd++){
        LPRINTF("tokens[curTokenInd].nameString = %s, myStrLen(tokens[curTokenInd].nameString) = %d", *tokenStrData(&tokens[curTokenInd]),  myStrLen(*tokenStrData(&tokens[curTokenInd])));
        if(!strncmp(curBufferPos, *tokenStrData(&tokens[curTokenInd]), myStrLen(*tokenStrData(&tokens[curTokenInd])))){
            LPRINTF("Needed token found tokens[curTokenInd].nameString = %s", tokens[curTokenInd].nameString);
            return &tokens[curTokenInd];
        }
    }

    return NULL;
}