
#include "tokens.h"
#include "protectionTokens.h"

#include "../general/debug.h"
#include "../general/strFunc.h"

#include <assert.h>
#include <malloc.h>

#define TOKENS_COUNT AST_DATA_COUNT
#define tokens       ASTdata

tokensSequence_t* tokenSequenceCtor(tokensSequence_t* tokensSequence){
    assert(tokensSequence);

    tokensSequence->data = (token_t*) calloc(1, sizeof(token_t));
    assert(tokensSequence->data);

    tokensSequence->capacity = 1;
    tokensSequence->size = 0;

    LPRINTF("successfully allocated memory for tokensSequence");

    return tokensSequence;
}

tokensSequence_t* tokenSequenceDtor(tokensSequence_t* tokenSequence){
    assert(tokenSequence);

    for(size_t curTokenInd = 0; curTokenInd < tokenSequence->size; curTokenInd++){
        if(*tokenStrData(&tokenSequence->data[curTokenInd])){
            LPRINTF("free[%p]", *tokenStrData(&tokenSequence->data[curTokenInd]));
            free(*tokenStrData(&tokenSequence->data[curTokenInd]));
        }
        if(*tokenStrWriteFile(&tokenSequence->data[curTokenInd])){
            free(*tokenStrWriteFile(&tokenSequence->data[curTokenInd]));
        }
    }

    free(tokenSequence->data);

    return NULL;
}

tokensSequence_t* reallocateTokensSequence(tokensSequence_t* tokenSequence){
    assert(tokenSequence);

    LPRINTF("start reallocation");

    size_t startSize = tokenSequence->capacity;

    token_t* temp = (token_t*) realloc(tokenSequence->data, tokenSequence->capacity * 2 * sizeof(token_t));
    assert(temp);

    LPRINTF("temp->data[0].type = %d", temp[0].type);

    tokenSequence->data = temp;
    tokenSequence->capacity *= 2;

    initTokensSequence(tokenSequence, startSize);

    LPRINTF("ended reallocation");

    return tokenSequence;
}

void initTokensSequence(tokensSequence_t* tokensSequence, size_t startInd){
    assert(tokensSequence);

    for(size_t curTokenInd = startInd; curTokenInd < tokensSequence->capacity; curTokenInd++){
        *tokenStrData(&tokensSequence->data[curTokenInd]) = NULL;
        *tokenStrWriteFile(&tokensSequence->data[curTokenInd]) = NULL;
    }
}

token_t* getCurrentToken(const char* curOpStringName){
    for(size_t curTokenInd = 0; curTokenInd < TOKENS_COUNT; curTokenInd++){
        if(isEqualStrings(curOpStringName, *tokenStrData(&tokens[curTokenInd]))){
            return &tokens[curTokenInd];
        }
    }

    return NULL;
}

bool createNumberToken(token_t* token, int tokenValue){
    assert(token);

    *tokenNumData(token) = tokenValue;
    *tokenStrData(token) = NULL;
    token->type          = NUMBER;
    
    *tokenStrWriteFile(token) = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(*tokenStrWriteFile(token));

    sprintf(*tokenStrWriteFile(token), "%d", tokenValue);

    LPRINTF("created number node");
    return true;
}

bool createVariableToken(token_t* token, char* tokenValue){
    assert(token);
    assert(tokenValue);

    *tokenStrData(token) = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(*tokenStrData(token));
    LPRINTF("create variable token memory allocated");

    myStrCpy(*tokenStrData(token),  tokenValue);    
    token->type         = NAME;

    *tokenStrWriteFile(token) = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(*tokenStrWriteFile(token));

    myStrCpy(*tokenStrWriteFile(token),  tokenValue);  

    return true;
}

bool copyStrTokenContent(token_t* token, token_t* reference){
    assert(token);
    assert(reference);

    *tokenStrData(token) = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(*tokenStrData(token));

    myStrCpy(*tokenStrData(token),  *tokenStrData(reference));

    return copyTokenContent(token, reference);
}

bool copyNumTokenContent(token_t* token, token_t* reference){
    assert(token);
    assert(reference);

    *tokenNumData(token) = *tokenNumData(reference);
    
    // LPRINTF("before token.type = %d, reference.type = %d", token->type, reference->type);
    copyTokenContent(token, reference);
    // LPRINTF("after token.type = %d, reference.type = %d", token->type, reference->type);

    return true;
}

bool copyTokenContent(token_t* token, token_t* reference){
    assert(token);
    assert(reference);

    token->type         = reference->type;

    *tokenStrWriteFile(token) = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(*tokenStrWriteFile(token));

    myStrCpy(*tokenStrWriteFile(token),  reference->writeFile);

    return true;
}

char** tokenStrData(token_t* token){
    assert(token);

    return &token->value.str;
}

int* tokenNumData(token_t* token){
    assert(token);

    return &token->value.num;
}

char** tokenStrWriteFile(token_t* token){
    assert(token);

    return &token->writeFile;
}