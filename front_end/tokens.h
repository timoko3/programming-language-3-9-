
#ifndef TOKENS_H
#define TOKENS_H

#include <stddef.h>

enum tokenType{
    START,
    CALL_FUNC,
    VARIABLE,
    NUMBER,
    NAME,
    ASSIGN,
    EXPRESS,
    BRACKL,
    BRACKR,
    ADD,
    SUB,
    MUL3,
    MUL,
    DIVIDE,
    END_STATEMENT,
    END_PROGRAM
};

enum tokenClass{
    KEYWORD,
    OPERATOR,
    LITERAL,
    IDENTIFIER,
    SPECIAL,
    EMPTY_TOKEN
};

union tokenVal_t{
    char* str;
    int num;
};

struct token_t{
    tokenVal_t   nameString; 
    tokenType    type;
    tokenClass   tClass;
    size_t       paramCount;
    int          priorityRank;
};

struct tokensSequence_t{
    token_t* data;
    size_t size;
    size_t capacity;
};

extern token_t tokens[];
extern const size_t TOKENS_COUNT;

const size_t MAX_VARIABLE_SIZE    = 512;

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

#endif /* TOKENS_H */