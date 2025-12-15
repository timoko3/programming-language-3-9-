
#ifndef TOKENS_H
#define TOKENS_H

#include <stddef.h>

#include "../core/core.h"

typedef ASTdata_t   token_t;
typedef ASTnodeType tokenType;

struct tokensSequence_t{
    token_t* data;
    size_t size;
    size_t capacity;
};

tokensSequence_t* tokenSequenceCtor(tokensSequence_t* tokenSequence);
tokensSequence_t* tokenSequenceDtor(tokensSequence_t* tokenSequence);
tokensSequence_t* reallocateTokensSequence(tokensSequence_t* tokenSequence);
void initTokensSequence(tokensSequence_t* tokensSequence, size_t startInd = 0);

token_t* getCurrentToken(const char* curOpStringName);

bool copyNumTokenContent(token_t* token, token_t* reference);
bool copyStrTokenContent(token_t* token, token_t* reference);
bool copyTokenContent(token_t* token, token_t* reference);

bool createNumberToken(token_t* token, int tokenValue);
bool createVariableToken(token_t* token, char* tokenValue);

char** tokenStrData(token_t* token);
int*   tokenNumData(token_t* token);
char** tokenStrWriteFile(token_t* token);

#endif /* TOKENS_H */