#ifndef CORE_H
#define CORE_H

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
    OUT
};

union ASTVal_t{
    char* str;
    int num;
};

struct ASTdata_t{
    char*       writeFile;
    ASTnodeType type;
    ASTVal_t    value;
};

#endif /* CORE_H */