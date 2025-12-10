#ifndef TOKENS_H
#define TOKENS_H

#include <stddef.h>

enum tokenNames{
    START,
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
    DIVIDE,
    END_STATEMENT
};

enum tokenClass{
    KEYWORD,
    OPERATOR,
    LITERAL,
    IDENTIFIER,
    SPECIAL
};

union tokenVal_t{
    char* str;
    int num;
};

struct token_t{
    tokenVal_t   nameString; 
    tokenNames   type;
    tokenClass   tClass;
    size_t       paramCount;
    int          priorityRank;
};

extern token_t tokens[];
extern const size_t TOKENS_COUNT;

const size_t MAX_VARIABLE_SIZE    = 64;

token_t* tokenSequenceCtor(token_t* tokenSequence);
token_t* tokenSequenceDtor(token_t* tokenSequence);

token_t* getCurrentToken(const char* curOpStringName);

bool copyTokenContent(token_t* token, token_t* reference);

bool createNumberToken(token_t* token, int tokenValue);
bool createVariableToken(token_t* token, char* tokenValue);

char** tokenStrData(token_t* token);
int*   tokenNumData(token_t* token);

#endif /* TOKENS_H */