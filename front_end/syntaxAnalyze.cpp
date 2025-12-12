
#include "syntaxAnalyze.h"
#include "protection.h"


#include "../general/debug.h"
#include "../general/strFunc.h"

#include <assert.h>
#include <malloc.h>

static treeNode_t* getG(treeNode_t* node, token_t** curToken);
static treeNode_t* getES(treeNode_t* node, token_t** curToken);
static treeNode_t* getM(treeNode_t* node, token_t** curToken);
static treeNode_t* getE(treeNode_t* node, token_t** curToken);
static treeNode_t* getT(treeNode_t* node, token_t** curToken);
static treeNode_t* getVar(treeNode_t* node, token_t** curToken);
static treeNode_t* getName(treeNode_t* node, token_t** curToken);
static treeNode_t* getN(treeNode_t* node, token_t** curToken);
static treeNode_t* getFunc(treeNode_t* node, token_t** curToken);
static treeNode_t* getStart(treeNode_t* node, token_t** curToken);

static treeNode_t* createTreeNodeFromToken(token_t* curToken, treeNode_t* left, treeNode_t* right);

static void SyntaxError();

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
    node = getES(node, curToken);
    if((*curToken)->type != END_PROGRAM){
        SyntaxError();
    }
    (*curToken)++;

    LPRINTF("Завершаю чтение и возвращаю ноду: %p", node);

    return node;
}

static treeNode_t* getES(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в ES. Текущий токен: %p", *curToken);
    node = getStart(node, curToken);
    if((*curToken)->type != END_STATEMENT){
        SyntaxError();
    }
    (*curToken)++;

    LPRINTF("Выхожу из ES. Текущий токен: %p", *curToken);

    return node;
}

static treeNode_t* getM(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в M. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getVar(node, curToken);
    assert(val1);

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
    assert(val1);

    if((*curToken)->type == ADD || (*curToken)->type == SUB){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getT(node, curToken);
        assert(val2);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из E. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getT(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в T. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getN(node, curToken);
    assert(val1);

    if((*curToken)->type == MUL || (*curToken)->type == DIVIDE){
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
    else{
        SyntaxError();
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

static treeNode_t* getFunc(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в Func. Текущий токен: %p", *curToken);


    treeNode_t* funcNode = NULL;
    if((*curToken)->type == CALL_FUNC){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val1 = getName(node, curToken);

        treeNode_t* val2 = getM(node, curToken);

        funcNode = createTreeNodeFromToken(&tempToken, val1, val2);
    }
    else{
        SyntaxError();
    }

    LPRINTF("Выхожу из Func. Текущий токен: %p", *curToken);

    return funcNode;
}

static treeNode_t* getStart(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в START. Текущий токен: %p", *curToken);

    treeNode_t* startNode = NULL;
    if((*curToken)->type == START){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val1 = getFunc(node, curToken);

        startNode = createTreeNodeFromToken(&tempToken, val1, NULL);
    }
    else{
        SyntaxError();
    }

    LPRINTF("Выхожу из START. Текущий токен: %p", *curToken);

    return startNode;
}

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

static void SyntaxError(){
    printf("Синтаксическая ошибка!!");
}