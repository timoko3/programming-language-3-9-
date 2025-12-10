#include "tokens.h"
#include "protection.h"

#include "../general/debug.h"
#include "../general/strFunc.h"

#include <assert.h>
#include <malloc.h>

token_t tokens[]{
    {"собрать воедино",        ADD,           OPERATOR, 2, 2},
    {"убавить",                SUB,           OPERATOR, 2, 2},
    {"трижды",                 MUL3,          OPERATOR, 0, 1},
    {"повелеваю поделить",     DIVIDE,        OPERATOR, 2, 1},
    {"Жили-были",              START,         SPECIAL,  0, 0},
    {"Добрый молодец",         VARIABLE,      KEYWORD,  1, 0},
    {"с силушкой богатырской", ASSIGN,        OPERATOR, 2, 0},
    {"так и было",             END_STATEMENT, SPECIAL,  0, 0}
};

const size_t TOKENS_COUNT = sizeof(tokens) / sizeof(tokens[0]);

tokensSequence_t* tokenSequenceCtor(tokensSequence_t* tokensSequence){
    assert(tokensSequence);

    tokensSequence->data = (token_t*) calloc(1, sizeof(token_t));
    assert(tokensSequence->data);

    tokensSequence->data[0].tClass = EMPTY_TOKEN;

    tokensSequence->capacity = 1;

    LPRINTF("successfully allocated memory for tokensSequence");

    return tokensSequence;
}

tokensSequence_t* tokenSequenceDtor(tokensSequence_t* tokenSequence){
    assert(tokenSequence);

    for(size_t curTokenInd = 0; curTokenInd < tokenSequence->size; curTokenInd++){
        if(tokenSequence->data[curTokenInd].type != NUMBER){
            free(*tokenStrData(&tokenSequence->data[curTokenInd]));
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

    LPRINTF("temp->data[0].tClass= %d, temp->data[0].type = %d", temp[0].tClass, temp[0].type);

    tokenSequence->data = temp;
    tokenSequence->capacity *= 2;

    initTokensSequence(tokenSequence, startSize);

    LPRINTF("ended reallocation");

    return tokenSequence;
}

void initTokensSequence(tokensSequence_t* tokensSequence, size_t startInd){
    assert(tokensSequence);

    for(size_t curTokenInd = startInd; curTokenInd < tokensSequence->capacity; curTokenInd++){
        tokensSequence->data[curTokenInd].tClass = EMPTY_TOKEN;
    }
}

token_t* getCurrentToken(const char* curOpStringName){
    for(size_t curTokenInd = 0; curTokenInd < sizeof(tokens) / sizeof(token_t); curTokenInd++){
        if(isEqualStrings(curOpStringName, *tokenStrData(&tokens[curTokenInd]))){
            return &tokens[curTokenInd];
        }
    }

    return NULL;
}

bool createNumberToken(token_t* token, int tokenValue){
    assert(token);

    *tokenNumData(token) = tokenValue;
    token->tClass        = LITERAL;
    token->type          = NUMBER;
    token->paramCount    = 0;
    token->priorityRank  = 0;

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
    token->tClass       = IDENTIFIER;
    token->type         = VARIABLE;
    token->paramCount   = 0;
    token->priorityRank = 0;

    return true;
}

bool copyTokenContent(token_t* token, token_t* reference){
    assert(token);
    assert(reference);

    *tokenStrData(token) = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(*tokenStrData(token));

    myStrCpy(*tokenStrData(token),  *tokenStrData(reference));
    token->tClass       = reference->tClass;
    token->type         = reference->type;
    token->paramCount   = reference->paramCount;
    token->priorityRank = reference->priorityRank;

    return true;
}

char** tokenStrData(token_t* token){
    assert(token);

    return &token->nameString.str;
}

int* tokenNumData(token_t* token){
    assert(token);

    return &token->nameString.num;
}