#include "lexicalAnalyze.h"
#include "general/tokens/protectionTokens.h"

#include "general/file.h"
#include "general/strFunc.h"
#include "general/debug.h"

#include "core/DSL.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define TOKENS_COUNT AST_DATA_COUNT
#define tokens       ASTdata

const size_t MAX_FUNCS_AMOUNT     = 4000;
const size_t MAX_LOCAL_VAR_AMOUNT = 4000;


static bool parseKeyword(char* curBufferPos, token_t* token);
static bool parseNumber(char* curBufferPos, token_t* token, char** endPos);
static bool parseName(char* curBufferPos, token_t* token, char** endPos, ASTnodeType type, hashTable_t* nameTable);

static tokensSequence_t* appendToken(tokensSequence_t* tokensSequence, token_t* newToken);

tokensSequence_t* tokenize(char* curBufferPos, tokensSequence_t* tokensSequence){
    assert(curBufferPos);
    assert(tokensSequence);
    
    stack nameTablesStack;
    stackCtor(&nameTablesStack, 1);

    hashTable_t* nameTable = (hashTable_t*) calloc(1, sizeof(hashTable_t));
    assert(newNameTable);

    stackPush(&nameTablesStack, nameTable);

    while(*curBufferPos != '\0'){
        LPRINTF("cycle iteration of reading token");
        LPRINTF("remaining string with curBufferPos: %s", curBufferPos);

        #ifdef DEBUG
        dumpTokenSequence(tokensSequence);
        #endif /* DEBUG */
        
        skipSpaces(&curBufferPos);
        if (*curBufferPos == '\0') break;

        token_t tempToken = {0}; 
        bool tokenFound = false;

        hashTable_t* topNameTable = NULL;
        stackGetTop(&nameTablesStack, (void**) &topNameTable);

        if(parseKeyword(curBufferPos, &tempToken)){
            if(_TOKEN_TYPE((&tempToken)) == INIT_FUNC ||
               _TOKEN_TYPE((&tempToken)) == WHILE     ||
               _TOKEN_TYPE((&tempToken)) == IF  ){
                hashTable_t* newNameTable = (hashTable_t*) calloc(1, sizeof(hashTable_t));
                assert(newNameTable);

                hashTableCtor(newNameTable, MAX_LOCAL_VAR_AMOUNT);
                stackPush(&nameTablesStack, newNameTable);
            }
            else if(_TOKEN_TYPE((&tempToken)) == END_BLOCK){
                hashTable_t* rubbishHashTable = NULL;
                stackPop(&nameTablesStack, (void**) &rubbishHashTable);
                hashTableDtor(rubbishHashTable);
            }


            LPRINTF("keyword case");
            curBufferPos += strlen(*tokenStrData(&tempToken));   
            tokenFound = true;
        }
        else if(parseNumber(curBufferPos, &tempToken, &curBufferPos)){
            LPRINTF("number case");
            tokenFound = true;
        }
        else if((_TOKEN_TYPE(tokenSequenceTail(tokensSequence)) == INIT_VARIABLE ||
                _TOKEN_TYPE(tokenSequenceTail(tokensSequence)) == INIT_VARIABLE) && 
                parseName(curBufferPos, &tempToken, &curBufferPos, VARIABLE, topNameTable)){
            LPRINTF("variable case");
            tokenFound = true;
        }
        else if((_TOKEN_TYPE(tokenSequenceTail(tokensSequence)) == INIT_FUNC ||
                _TOKEN_TYPE(tokenSequenceTail(tokensSequence)) == CALL_FUNC) && 
                parseName(curBufferPos, &tempToken, &curBufferPos, FUNCTION, nameTable)){
            LPRINTF("function case");
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
    
    hashTableDtor(nameTable);

    stackDtor(&nameTablesStack);

    LPRINTF("ended tokenization cycle");

    return tokensSequence;
}

static bool parseKeyword(char* curBufferPos, token_t* token){
    assert(curBufferPos);
    assert(token);

    LPRINTF("*tokenStrData(&tokens[0]) = %s", *tokenStrData(&tokens[0]));

    for(size_t curTokenInd = 0; curTokenInd < TOKENS_COUNT; curTokenInd++){
        if(!strncmp(curBufferPos, *tokenStrData(&tokens[curTokenInd]), myStrLen(*tokenStrData(&tokens[curTokenInd])))){
            LPRINTF("Needed token found tokens[curTokenInd].nameString = %s", *tokenStrData(&tokens[curTokenInd]));

            copyStrTokenContent(token, &tokens[curTokenInd]); 
            return true;
        }
    }
    return false;
}

static bool parseNumber(char* curBufferPos, token_t* token, char** endPos){
    assert(curBufferPos);
    assert(token);

    if (*curBufferPos == '-' || *curBufferPos == '+' || isdigit(*curBufferPos)) {
        int number = (int) strtol(curBufferPos, endPos, 10);
        LPRINTF("Found number: %d", number);
        createNumberToken(token, number);

        return true;
    }
    return false;
}

static bool parseName(char* curBufferPos, token_t* token, char** endPos, ASTnodeType type, hashTable_t* nameTable){
    assert(curBufferPos);
    assert(token);
    assert(endPos);
    assert(nameTable);

    if(!isspace(*curBufferPos)) {
        char curTokenValue[MAX_VARIABLE_SIZE] = "";
        int curBufferShift = 0;
        
        LPRINTF("start getting varible name");
        while(isApprovedVariableSym(curBufferPos)){
            size_t symAmountBytes = getUtf8CharLength(*curBufferPos);
            for(size_t curByte = 0; curByte < symAmountBytes; curByte++){
                LPRINTF("getUtf8CharLength(*curBufferPos) = %lu, curBufferShift = %d", getUtf8CharLength(*curBufferPos), curBufferShift);
                curTokenValue[curBufferShift] = *curBufferPos;
                curBufferPos++;
                curBufferShift++;
            }
        }

        int curCellNum = 0;
        hashTableFind(nameTable, curTokenValue, &curCellNum);
        if(curCellNum == SEARCH_NOT_FOUND_VALUE) return false;
        else{
            hashTableInsert(nameTable, curTokenValue, &curCellNum);
        }
        char writeFileName[MAX_VARIABLE_SIZE] = "";
        sprintf(writeFileName, "%d", curCellNum);
        createNameTokenNdbg(token, curTokenValue, writeFileName, type);
        
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
