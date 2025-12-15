
#include "tokens.h"
#include "protection.h"

#include "../general/debug.h"
#include "../general/strFunc.h"

#include <assert.h>
#include <malloc.h>

token_t tokens[]{
    {"собрать воедино",        ADD,           OPERATOR, "+"},
    {"убавить",                SUB,           OPERATOR, "-"},
    {"трижды",                 MUL3,          OPERATOR, "no"},
    {"преумножить",            MUL,           OPERATOR, "*"},
    {"повелеваю поделить",     DIVIDE,        OPERATOR, "/"},
    {"Жили-были",              START,         SPECIAL,  "no"},
    {"Добрый молодец",         INIT_VARIABLE, KEYWORD,  "no"},
    {"сила",                   CALL_VARIABLE, KEYWORD,  "no"},
    {"с силушкой богатырской", ASSIGN,        OPERATOR, "="},
    {"так и было",             END_STATEMENT, SPECIAL,  ";"},
    {"и я там был мед пиво "
        "пил по усам текло, " 
        "а в рот не попало",   END_PROGRAM,   SPECIAL,  ";"},
    {"Скоро сказка "
     "сказывается, да не "
     "скоро дело делается",    END_BLOCK,      SPECIAL, "no"},
    {"Сивка-бурка вещая каурка "
     "стань передо мной "
      "как лист перед травой", CALL_FUNC,     SPECIAL,  "no"},
    {"Покуда",                 WHILE,         KEYWORD,  "while"},
    {"ежели случилось, что",   IF,            KEYWORD,  "if"},
    
    {"А коли не "
     "случилось, то",          ELSE,          KEYWORD,  "no"},
    {"А ежели случилось, что", ELSE_IF,       KEYWORD,  "no"},
    {"(",                      BRACKL,        SPECIAL,  "no"},
    {")",                      BRACKR,        SPECIAL,  "no"},
    {",",                      COMMA,         SPECIAL,  ","},
    {"В некотором царстве, " 
     "в некотором " 
     "государстве",            INIT_FUNC,     KEYWORD,  "no"},
    {"двое из ларца "
     "одинаковых с лица",      EQUAL,         OPERATOR, "=="},
    {"ни чета",                NOT_EQUAL,     OPERATOR, "!="},         // не одного поля ягода
    {"и в подметки " 
     "не годится",             LT,            OPERATOR, "<"},
    {"по плечу",               LE,            OPERATOR, "<="},
    {"не по плечу",            GT,            OPERATOR, ">"},
    {"больше али равна",       GE,            OPERATOR, ">="},
    {"воротить",               RETURN,        KEYWORD,  "ret"},
    {"жар-птица выжгла "
     "на земле русской",       OUT,           KEYWORD,  "out"},
    {"пойди туда — не знаю куда, "
     "принеси то — не знаю что", IN,          KEYWORD,  "in"},
    {"вот и сказочке конец",   HLT,           KEYWORD,  "hlt"}
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
        *tokenStrData(&tokensSequence->data[curTokenInd]) = NULL;
        *tokenStrWriteFile(&tokensSequence->data[curTokenInd]) = NULL;
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
    *tokenStrData(token) = NULL;
    token->tClass        = LITERAL;
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
    token->tClass       = IDENTIFIER;
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

    token->tClass       = reference->tClass;
    token->type         = reference->type;

    *tokenStrWriteFile(token) = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(*tokenStrWriteFile(token));

    myStrCpy(*tokenStrWriteFile(token),  reference->fileWrite);

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

char** tokenStrWriteFile(token_t* token){
    assert(token);

    return &token->fileWrite;
}