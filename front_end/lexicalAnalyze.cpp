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

static bool parseKeyword(char* curBufferPos, token_t* token);
static bool parseNumber(char* curBufferPos, token_t* token, char** endPos);
static bool parseVariable(char* curBufferPos, token_t* token, char** endPos);

static tokensSequence_t* appendToken(tokensSequence_t* tokensSequence, token_t* newToken);

tokensSequence_t* tokenize(char* curBufferPos, tokensSequence_t* tokensSequence){
    assert(curBufferPos);
    assert(tokensSequence);

    while(*curBufferPos != '\0'){
        LPRINTF("cycle iteration of reading token");
        LPRINTF("remaining string with curBufferPos: %s", curBufferPos);
        dumpTokenSequence(tokensSequence);
        skipSpaces(&curBufferPos);
        if (*curBufferPos == '\0') break;

        token_t tempToken = {0}; 
        bool tokenFound = false;

        if(parseKeyword(curBufferPos, &tempToken)){
            LPRINTF("keyword case");
            curBufferPos += strlen(*tokenStrData(&tempToken));   
            tokenFound = true;
        }
        else if(parseNumber(curBufferPos, &tempToken, &curBufferPos)){
            LPRINTF("number case");
            tokenFound = true;
        }
        else if(parseVariable(curBufferPos, &tempToken, &curBufferPos)){
            LPRINTF("variable case");
            tokenFound = true;
        }

        if(tokenFound){
            tokensSequence_t* tempSeqPtr = appendToken(tokensSequence, &tempToken);
            assert(tempSeqPtr);

            tokensSequence = tempSeqPtr;
            if(*tokenStrData(&tempToken)) free(*tokenStrData(&tempToken));
            if(*tokenStrWriteFile(&tempToken)) free(*tokenStrWriteFile(&tempToken));
        }
    }

    LPRINTF("ended tokenization cycle");

    return tokensSequence;
}

static bool parseKeyword(char* curBufferPos, token_t* token){
    assert(curBufferPos);
    assert(token);

    for(size_t curTokenInd = 0; curTokenInd < TOKENS_COUNT; curTokenInd++){
        if(!strncmp(curBufferPos, *tokenStrData(&tokens[curTokenInd]), myStrLen(*tokenStrData(&tokens[curTokenInd])))){
            LPRINTF("Needed token found tokens[curTokenInd].nameString = %s", tokens[curTokenInd].nameString);

            copyStrTokenContent(token, &tokens[curTokenInd]); 
            return true;
        }
    }
    return false;
}

static bool parseNumber(char* curBufferPos, token_t* token, char** endPos){
    assert(curBufferPos);
    assert(token);

    if (isdigit(*curBufferPos)) {
        int number = (int) strtol(curBufferPos, endPos, 10);
        LPRINTF("Found number: %d", number);
        createNumberToken(token, number);

        return true;
    }
    return false;
}

static bool parseVariable(char* curBufferPos, token_t* token, char** endPos){
    assert(curBufferPos);
    assert(token);
    assert(endPos);

    if(!isspace(*curBufferPos)) {
        char curTokenValue[MAX_VARIABLE_SIZE] = "";
        int curBufferShift = 0;
        
        LPRINTF("start getting varible name");
        while(isApprovedVariableSym(curBufferPos)){
            int symAmountBytes = getUtf8CharLength(*curBufferPos);
            for(int curByte = 0; curByte < symAmountBytes; curByte++){
                LPRINTF("getUtf8CharLength(*curBufferPos) = %d, curBufferShift = %d", getUtf8CharLength(*curBufferPos), curBufferShift);
                curTokenValue[curBufferShift] = *curBufferPos;
                curBufferPos++;
                curBufferShift++;
            }
        }
        
        createVariableToken(token, curTokenValue);
        
        *endPos = curBufferPos;
        return true;
    }
    return false;
}


static tokensSequence_t* appendToken(tokensSequence_t* tokensSequence, token_t* newToken){
    assert(tokensSequence);
    assert(newToken);

    if(tokensSequence->size >= tokensSequence->capacity){
        reallocateTokensSequence(tokensSequence);
    }

    if(newToken->type == NUMBER){
        copyNumTokenContent(&tokensSequence->data[tokensSequence->size], newToken);
    }
    else{
        copyStrTokenContent(&tokensSequence->data[tokensSequence->size], newToken);
    }

    tokensSequence->size++;

    return tokensSequence;
}
