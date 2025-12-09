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

struct token_t{
    char*    nameString; 
    tokenNames     type;
    tokenClass     tClass;
    size_t         paramCount;
    int            priorityRank;
};

static token_t tokens[]{
    {"собрать воедино",        ADD,           OPERATOR, 2, 2},
    {"убавить",                SUB,           OPERATOR, 2, 2},
    {"трижды",                 MUL3,          OPERATOR, 0, 1},
    {"повелеваю поделить",     DIVIDE,        OPERATOR, 2, 1},
    {"Жили-были",              START,         SPECIAL,  0, 0},
    {"Добрый молодец",         VARIABLE,      KEYWORD,  1, 0},
    {"с силушкой богатырской", ASSIGN,        OPERATOR, 2, 0},
    {"так и было",             END_STATEMENT, SPECIAL,  0, 0}
};

const size_t MAX_VARIABLE_SIZE    = 64;

token_t* tokenSequenceCtor(token_t* tokenSequence);
token_t* tokenSequenceDtor(token_t* tokenSequence);

token_t* getCurrentToken(const char* curOpStringName);

bool copyTokenContent(token_t* token, token_t* reference);