
#include "tokens.h"
#include "protection.h"

#include "../general/debug.h"
#include "../general/strFunc.h"

#include <assert.h>
#include <malloc.h>

token_t tokens[]{
    {"собрать воедино",        ADD,           OPERATOR, 2, 2},
    {"убавить",                SUB,           OPERATOR, 2, 2},
    {"трижды",                 MUL3,          OPERATOR, 1, 1},
    {"преумножить",            MUL,           OPERATOR, 2, 0},
    {"повелеваю поделить",     DIVIDE,        OPERATOR, 2, 1},
    {"Жили-были",              START,         SPECIAL,  0, 0},
    {"Добрый молодец",         INIT_VARIABLE, KEYWORD,  1, 0},
    {"сила",                   CALL_VARIABLE, KEYWORD,  1, 0},
    {"с силушкой богатырской", ASSIGN,        OPERATOR, 2, 0},
    {"так и было",             END_STATEMENT, SPECIAL,  0, 0},
    {"и я там был мед пиво "
        "пил по усам текло, " 
        "а в рот не попало",   END_PROGRAM,   SPECIAL,  0, 0},
    {"Скоро сказка "
     "сказывается, да не "
     "скоро дело делается",    END_BLOCK,      SPECIAL,  0, 0},
    {"Сивка-бурка вещая каурка "
     "стань передо мной "
      "как лист перед травой", CALL_FUNC,     SPECIAL,  1, 0},
    {"Покуда",                 WHILE,         KEYWORD,  2, 0},
    {"ежели случилось, что",   IF,            KEYWORD,  2, 0},
    
    {"А коли не "
     "случилось, то",          ELSE,          KEYWORD,  1, 0},
    {"А ежели случилось, что", ELSE_IF,       KEYWORD,  2, 0},
    {"(",                      BRACKL,        SPECIAL,  0, 0},
    {")",                      BRACKR,        SPECIAL,  0, 0},
    {",",                      COMMA,         SPECIAL,  0, 0},
    {"В некотором царстве, " 
     "в некотором " 
     "государстве",            INIT_FUNC,     KEYWORD,  2, 0},
    {"двое из ларца "
     "одинаковых с лица",      EQUAL,         OPERATOR, 2, 0},
    {"ни чета",                NOT_EQUAL,     OPERATOR, 2, 0},         // не одного поля ягода
    {"и в подметки " 
     "не годится",             LT,            OPERATOR, 2, 0},
    {"по плечу",               LE,            OPERATOR, 2, 0},
    {"не по плечу",            GT,            OPERATOR, 2, 0},
    {"больше али равна",       GE,            OPERATOR, 2, 0},
    {"воротить",               RETURN,        KEYWORD,  1, 0},
    {"жар-птица выжгла "
     "на земле русской",       OUT,           KEYWORD,  1, 0},
    {"пойди туда — не знаю куда, "
     "принеси то — не знаю что", IN,          KEYWORD,  1, 0},
    {"вот и сказочке конец",   HLT,           KEYWORD,  0, 0}
};

const size_t TOKENS_COUNT = sizeof(tokens) / sizeof(tokens[0]);

tokensSequence_t* tokenSequenceCtor(tokensSequence_t* tokensSequence){
    assert(tokensSequence);

    tokensSequence->data = (token_t*) calloc(1, sizeof(token_t));
    assert(tokensSequence->data);

    tokensSequence->data[0].tClass = EMPTY_TOKEN;

    tokensSequence->capacity = 1;
    tokensSequence->size = 0;

    LPRINTF("successfully allocated memory for tokensSequence");

    return tokensSequence;
}

tokensSequence_t* tokenSequenceDtor(tokensSequence_t* tokenSequence){
    assert(tokenSequence);

    for(size_t curTokenInd = 0; curTokenInd < tokenSequence->size; curTokenInd++){
        if(tokenSequence->data[curTokenInd].type != NUMBER){
            LPRINTF("free[%p]", *tokenStrData(&tokenSequence->data[curTokenInd]));
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
    token->type         = NAME;
    token->paramCount   = 0;
    token->priorityRank = 0;

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