#include "tokens.h"

#include "../general/strFunc.h"

#include <assert.h>
#include <malloc.h>

token_t* tokenSequenceCtor(token_t* tokenSequence){
    assert(tokenSequence);

    return NULL;
}

token_t* tokenSequenceDtor(token_t* tokenSequence){
    assert(tokenSequence);

    for(size_t curTokenInd = 0; tokenSequence[curTokenInd].nameString != NULL; curTokenInd++){
        free(tokenSequence[curTokenInd].nameString);
    }

    return NULL;
}

token_t* getCurrentToken(const char* curOpStringName){
    for(size_t curTokenInd = 0; curTokenInd < sizeof(tokens) / sizeof(token_t); curTokenInd++){
        if(isEqualStrings(curOpStringName, tokens[curTokenInd].nameString)){
            return &tokens[curTokenInd];
        }
    }

    return NULL;
}

bool copyTokenContent(token_t* token, token_t* reference){
    assert(token);
    assert(reference);

    token->nameString = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(token->nameString);

    myStrCpy(token->nameString,  reference->nameString);
    token->tClass       = reference->tClass;
    token->type         = reference->type;
    token->paramCount   = reference->paramCount;
    token->priorityRank = reference->priorityRank;

    return true;
}