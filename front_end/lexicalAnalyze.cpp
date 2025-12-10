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

static token_t* getKeyword(char* curBufferPos);

tokensSequence_t* tokenize(char* curBufferPos, tokensSequence_t* tokensSequence){
    assert(curBufferPos);
    assert(tokensSequence);

    size_t curTokenInd = 0;
    while(*curBufferPos != '\0'){
        if(tokensSequence->size >= tokensSequence->capacity) tokensSequence = reallocateTokensSequence(tokensSequence);

        dumpTokenSequence(tokensSequence);
        LPRINTF("cycle iteration of reading token");
        LPRINTF("remaining string with curBufferPos: %s", curBufferPos);

        token_t* curToken = getKeyword(curBufferPos);
        if(curToken){
            LPRINTF("is keyword");
            copyTokenContent(&tokensSequence->data[curTokenInd], curToken);

            curBufferPos += strlen(*tokenStrData(curToken));
        }
        else{
            LPRINTF("NOT KEYWORD");
            if(isdigit(*curBufferPos)){
                LPRINTF("start creating number token lexical");
                char* endNumBufferPos = NULL;

                int number = (int) strtol(curBufferPos, &endNumBufferPos, 10);
                LPRINTF("get number token value: %d", number);
                createNumberToken(&tokensSequence->data[curTokenInd], number);

                curBufferPos = endNumBufferPos;
            }
            else if(!isspace(*curBufferPos)){
                LPRINTF("start creating variable token lexical");
                int curBufferShift = 0;

                char curTokenValue[MAX_VARIABLE_SIZE] = "";
                sscanf(curBufferPos, "%s%n", curTokenValue, &curBufferShift);

                createVariableToken(&tokensSequence->data[curTokenInd], curTokenValue);

                curBufferPos += curBufferShift;
            }
            else{
                utf8Shift(1, &curBufferPos);
                continue;
            }
        }        
        tokensSequence->size++;
        curTokenInd++;
    }
    
    LPRINTF("ended tokenization cycle");

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