#include "syntaxAnalyze.h"

#include "general/debug.h"
#include "general/tree.h"
#include "general/strFunc.h"

#include <assert.h>
#include <malloc.h>

static treeNode_t* getG(treeNode_t* node, token_t** curToken);
static treeNode_t* getM(treeNode_t* node, token_t** curToken);

static treeNode_t* createTreeNodeFromToken(token_t* curToken, treeNode_t* left, treeNode_t* right);

static void SyntaxError();

treeNode_t* syntaxAnalyze(tree_t* syntaxTree, tokensSequence_t* tokensSequence){
    assert(syntaxTree);
    assert(tokensSequence);

    token_t* curToken = tokensSequence->data;

    syntaxTree->root = getG(syntaxTree->root, &curToken);

    return syntaxTree->root;
}

static treeNode_t* getG(treeNode_t* node, token_t** curToken){
    assert(curToken);
    
    LPRINTF("Зашел в G. Текущий токен: %p", *curToken);
    node = getM(node, curToken);
    if((*curToken)->type != END_PROGRAM){
        SyntaxError();
    }
    (*curToken)++;

    LPRINTF("Завершаю чтение и возвращаю ноду: %p", node);

    return node;
}

static treeNode_t* getM(treeNode_t* node, token_t** curToken){
    assert(curToken);

    LPRINTF("Зашел в M. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getE(node, curToken);
    assert(val1);

    if((*curToken)->type == ASSIGN){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getE(node, curToken);
        assert(val2);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    return val1;
}

static treeNode_t* createTreeNodeFromToken(token_t* curToken, treeNode_t* left, treeNode_t* right){
    assert(curToken);
    assert(left);
    assert(right);

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
}

static void SyntaxError(){
    printf("Синтаксическая ошибка!!");
}