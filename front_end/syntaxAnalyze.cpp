
#include "syntaxAnalyze.h"
#include "protection.h"


#include "../general/debug.h"
#include "../general/strFunc.h"

#include <assert.h>
#include <malloc.h>

static treeNode_t* getG(treeNode_t* node, token_t** curToken);
static treeNode_t* getM(treeNode_t* node, token_t** curToken);
static treeNode_t* getE(treeNode_t* node, token_t** curToken);
static treeNode_t* getT(treeNode_t* node, token_t** curToken);
static treeNode_t* getVar(treeNode_t* node, token_t** curToken);
static treeNode_t* getName(treeNode_t* node, token_t** curToken);
static treeNode_t* getN(treeNode_t* node, token_t** curToken);
static treeNode_t* getFunc(treeNode_t* node, token_t** curToken);
static treeNode_t* getFuncArg(treeNode_t* node, token_t** curToken);
static treeNode_t* getComma(treeNode_t* node, token_t** curToken);
// static treeNode_t* getStart(treeNode_t* node, token_t** curToken);
static treeNode_t* getEB(treeNode_t* node, token_t** curToken);
static treeNode_t* getBlock(treeNode_t* node, token_t** curToken);
static treeNode_t* getWhile(treeNode_t* node, token_t** curToken);
static treeNode_t* getWhileArg(treeNode_t* node, token_t** curToken);
static treeNode_t* getES(treeNode_t* node, token_t** curToken);
static treeNode_t* getStatement(treeNode_t* node, token_t** curToken);

static treeNode_t* createTreeNodeFromToken(token_t* curToken, treeNode_t* left, treeNode_t* right);

static void SyntaxError(const char* file, int line, const char* func);
static void expect(token_t** curToken, tokenType expected, const char* file, int line, const char* func);

#define SYNTAX_ERROR SyntaxError(__FILE__, __LINE__, __FUNCTION__)
#define EXPECT(curToken, expected) expect(curToken, expected, __FILE__, __LINE__, __FUNCTION__)

treeNode_t* syntaxAnalyze(tree_t* syntaxTree, tokensSequence_t* tokensSequence){
    assert(syntaxTree);
    assert(tokensSequence);

    LPRINTF("tokensSequence->data = %p", tokensSequence->data);
    dumpTokenSequence(tokensSequence);

    token_t* curToken = tokensSequence->data;

    syntaxTree->root = getG(syntaxTree->root, &curToken);

    return syntaxTree->root;
}

static treeNode_t* getG(treeNode_t* node, token_t** curToken){
    assert(curToken);
    
    LPRINTF("Зашел в G. Текущий токен: %p", *curToken);

    EXPECT(curToken, START);

    node = getEB(node, curToken);
    if((*curToken)->type != END_PROGRAM){
        SYNTAX_ERROR;
    }
    (*curToken)++;

    LPRINTF("Завершаю чтение и возвращаю ноду: %p", node);

    return node;
}

static treeNode_t* getEB(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в EB. Текущий токен: %p", *curToken);
    
    treeNode_t* val1 = getES(node, curToken);

    while((*curToken)->type == END_BLOCK){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getBlock(node, curToken);

        if(val2){
            val1 = createTreeNodeFromToken(&tempToken, val1, val2);
        }
    }

    LPRINTF("Вышел из EB. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getES(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в ES. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getBlock(node, curToken);

    while((*curToken)->type == END_STATEMENT){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getBlock(node, curToken);

        if(val2){
            val1 = createTreeNodeFromToken(&tempToken, val1, val2);
        }
    }

    LPRINTF("Вышел из ES. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getBlock(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в BLOCK. Текущий токен: %p", *curToken);

    treeNode_t* result = NULL;
    treeNode_t* last   = NULL;

    if((*curToken)->type == INIT_FUNC){
        treeNode_t* stmt = getFunc(node, curToken);

        if(!result){
            result = stmt;
        }
        else{
            last->left = stmt;
        }

        last = stmt;
    }
    else if((*curToken)->type == WHILE){
        treeNode_t* stmt = getWhile(node, curToken);

        if(!result){
            result = stmt;
        }
        else{
            last->left = stmt;
        }

        last = stmt;
    }
    else{
        treeNode_t* stmt = getStatement(node, curToken);

        if(!result){
            result = stmt;
        }
        else{
            last->left = stmt;
        }

        last = stmt;
    }

    LPRINTF("Вышел из BLOCK. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getStatement(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в STATEMENT. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getM(node, curToken);

    while((*curToken)->type == END_STATEMENT){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getM(node, curToken);
        
        if(val2){
            val1 = createTreeNodeFromToken(&tempToken, val1, val2);
        }
    }

    LPRINTF("Вышел из STATEMENT. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getFunc(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в Func. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;
    if((*curToken)->type == INIT_FUNC){
        (*curToken)++;
        token_t tempToken = **curToken;
        (*curToken)++;        

        val1 = getFuncArg(node, curToken);

        treeNode_t* val2 = getBlock(node, curToken);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из Func. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getWhile(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в WHILE. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;
    if((*curToken)->type == WHILE){
        token_t tempToken = **curToken;
        (*curToken)++;        

        val1 = getWhileArg(node, curToken);

        treeNode_t* val2 = getBlock(node, curToken);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Вешел из WHILE. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getWhileArg(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("ЗАшел в WHILE_ARG. Текущий токен: %p", *curToken);

    treeNode_t* result = NULL;
    if((*curToken)->type == BRACKL){
        (*curToken)++;
        while((*curToken)->type != BRACKR){
            result = getE(node, curToken);
        }
        (*curToken)++;
    }
    else{
        SYNTAX_ERROR;
    }

    LPRINTF("Вешел из WHILE_ARG. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getFuncArg(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в FuncArg. Текущий токен: %p", *curToken);

    treeNode_t* result = NULL;
    if((*curToken)->type == BRACKL){
        (*curToken)++;
        while((*curToken)->type != BRACKR){
            result = getComma(node, curToken);
        }
        (*curToken)++;
    }
    else{
        SYNTAX_ERROR;
    }

    LPRINTF("Вышел из FuncArg. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getComma(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в Comma. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getName(node, curToken);

    while((*curToken)->type == COMMA){
            token_t tempToken = **curToken;
            (*curToken)++;   

            treeNode_t* val2 = getName(node, curToken);

            val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }
    if(!val1){
        SYNTAX_ERROR;
    }

    LPRINTF("Вышел из Comma. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getM(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в M. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getVar(node, curToken);

    if((*curToken)->type == ASSIGN){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getE(node, curToken);
        assert(val2);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из M. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getE(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в E. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getT(node, curToken);

    while((*curToken)->type == ADD || (*curToken)->type == SUB){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getT(node, curToken);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из E. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getT(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в T. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getN(node, curToken);

    while((*curToken)->type == MUL || (*curToken)->type == DIVIDE){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getN(node, curToken);
        assert(val2);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из T. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getVar(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в VAR. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;
    if((*curToken)->type == VARIABLE){
        token_t tempToken = **curToken;
        (*curToken)++;

        val1 = getName(node, curToken);

        val1 = createTreeNodeFromToken(&tempToken, val1, NULL);
    }   

    LPRINTF("Выхожу из VAR. Текущий токен: %p", *curToken);

    return val1; 
}

static treeNode_t* getName(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в NAME. Текущий токен: %p", *curToken);

    treeNode_t* name = NULL;
    if((*curToken)->type == NAME){
        name = createTreeNodeFromToken(*curToken, NULL, NULL);
        (*curToken)++;
    }

    LPRINTF("Выхожу из NAME. Текущий токен: %p", *curToken);
    
    return name;
}

static treeNode_t* getN(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в N. Текущий токен: %p", *curToken);

    treeNode_t* number = NULL;
    if((*curToken)->type == NUMBER){
        number = createTreeNodeFromToken(*curToken, NULL, NULL);
        (*curToken)++;
    }

    LPRINTF("Выхожу из N. Текущий токен: %p", *curToken);

    return number;
}

// static treeNode_t* getStart(treeNode_t* node, token_t** curToken){
//     assert(curToken);

//     LPRINTF("Зашел в START. Текущий токен: %p", *curToken);

//     static size_t amountStartCalls = 0;
//     amountStartCalls++;

//     treeNode_t* val1 = NULL;
//     if((*curToken)->type == START && amountStartCalls == 1){
//         (*curToken)++;
//     }
//     else if(amountStartCalls > 0) return NULL;
//     else{
//         SYNTAX_ERROR;
//     }

//     LPRINTF("Выхожу из START. Текущий токен: %p", *curToken);

//     return val1;
// }

static treeNode_t* createTreeNodeFromToken(token_t* curToken, treeNode_t* left, treeNode_t* right){
    assert(curToken);

    treeNode_t* newNode = createNewNode(left, right);
    assert(newNode);

    newNode->nClass = curToken->tClass;
    newNode->type   = curToken->type;
    if(curToken->type != NUMBER){
        newNode->data.str = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
        assert(newNode->data.str);

        myStrCpy(newNode->data.str, *tokenStrData(curToken));
    }
    else{
        newNode->data.num = *tokenNumData(curToken);
    }

    return newNode;
}

static void expect(token_t** curToken, tokenType expected, const char* file, int line, const char* func){
    assert(curToken);
    assert(file);
    assert(func);

    if ((*curToken)->type != expected){
        SYNTAX_ERROR;
    }
    (*curToken)++;
}

static void SyntaxError(const char* file, int line, const char* func){
    assert(file);
    assert(func);

    printf("Синтаксическая ошибка!! %s:%d:%s", file, line, func);
}