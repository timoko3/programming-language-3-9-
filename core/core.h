#ifndef CORE_H
#define CORE_H

#include <stddef.h>

extern const char* TREE_FILE_NAME;

enum ASTnodeType{
    START,
    INIT_VARIABLE,
    CALL_VARIABLE,
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
    COMMA,
    END_STATEMENT,
    END_PROGRAM,
    END_BLOCK,
    WHILE,
    IF,
    ELSE,
    ELSE_IF,
    INIT_FUNC,
    CALL_FUNC,
    EQUAL,
    NOT_EQUAL,
    GT,
    LT,
    GE,
    LE,
    RETURN,
    HLT,
    IN,
    OUT,
    MAIN,
    SQRT,
    POPM,
    DRAW
};

union ASTVal_t{
    char* str;
    int num;
};

struct ASTnodeData_t{
    ASTVal_t    value;
    ASTnodeType type;
    char*       writeFile;
};

const size_t MAX_VARIABLE_SIZE = 512;

extern ASTnodeData_t ASTdata[];
extern const size_t AST_DATA_COUNT;

void initDescriptors();
ASTnodeData_t* findAstData(char* referenceStr);

#endif /* CORE_H */