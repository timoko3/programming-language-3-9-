
#include "syntaxAnalyze.h"
#include "protection.h"
#include "nametable.h"
#include "../general/stack/stack.h"

#include "../general/debug.h"
#include "../general/strFunc.h"

#include <assert.h>
#include <malloc.h>

static treeNode_t* getG          (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getM          (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getE          (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getT          (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getInitVar    (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getName       (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getN          (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getFunc       (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getCallFunc   (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getFuncArg    (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getComma      (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getEB         (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getBlock      (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getWhile      (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getIfWhileArg (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getES         (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getIf         (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getEqual      (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getComparison (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getStatement  (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getOperand    (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getCallVar    (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getRet        (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getHLT        (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getOut        (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getIn         (treeNode_t* node, token_t** curToken, stack* nameTables);

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

    stack nameTablesStack;
    stackCtor(&nameTablesStack, 1);

    syntaxTree->root = getG(syntaxTree->root, &curToken, &nameTablesStack);

    stackDtor(&nameTablesStack);

    return syntaxTree->root;
}

static treeNode_t* getG(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);
    
    LPRINTF("Зашел в G. Текущий токен: %p", *curToken);

    EXPECT(curToken, START);

    node = getEB(node, curToken, nameTables);
    if((*curToken)->type != END_PROGRAM){
        SYNTAX_ERROR;
    }
    (*curToken)++;

    LPRINTF("Завершаю чтение и возвращаю ноду: %p", node);

    return node;
}

static treeNode_t* getEB(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в EB. Текущий токен: %p", *curToken);
    
    treeNode_t* val1 = getBlock(node, curToken, nameTables);

    while((*curToken)->type == END_BLOCK){
        token_t tempToken = **curToken;
        (*curToken)++;
        
        treeNode_t* val2 = getBlock(node, curToken, nameTables);

        if(val2){
            val1 = createTreeNodeFromToken(&tempToken, val1, val2);
            myStrCpy(val1->writeFile, ";");
        }
    }

    LPRINTF("Вышел из EB. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getBlock(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в BLOCK. Текущий токен: %p", *curToken);

    treeNode_t* result = NULL;
    treeNode_t* last   = NULL;

    nametable_t* newNameTable = (nametable_t*) calloc(1, sizeof(nametable_t));
    assert(newNameTable);

    nameTableCtor(newNameTable);
    stackPush(nameTables, newNameTable);

    while((*curToken)->type == INIT_FUNC ||
          (*curToken)->type == WHILE ||
          (*curToken)->type == IF ||
          (*curToken)->type == INIT_VARIABLE ||
          (*curToken)->type == CALL_VARIABLE ||
          (*curToken)->type == CALL_FUNC ||
          (*curToken)->type == RETURN ||
          (*curToken)->type == OUT ||
          (*curToken)->type == IN ||
          (*curToken)->type == HLT){
        treeNode_t* stmt = getES(node, curToken, nameTables);
        if (!stmt) break;

        if (!result) result = stmt;
        else last->left = stmt;

        last = stmt;

        if ((*curToken)->type == END_STATEMENT)(*curToken)++;
    }
    
    // if((*curToken)->type == INIT_FUNC){
    //     treeNode_t* stmt = getFunc(node, curToken, nameTables);

    //     if(!result){
    //         result = stmt;
    //     }
    //     else{
    //         last->left = stmt;
    //     }

    //     last = stmt;

    // }
    // else if((*curToken)->type == WHILE){
    //     treeNode_t* stmt = getWhile(node, curToken, nameTables);

    //     if(!result){
    //         result = stmt;
    //     }
    //     else{
    //         last->left = stmt;
    //     }

    //     last = stmt;
    // }
    // else if((*curToken)->type == IF){
    //     treeNode_t* stmt = getIf(node, curToken, nameTables);

    //     if(!result){
    //         result = stmt;
    //     }
    //     else{
    //         last->left = stmt;
    //     }

    //     last = stmt;
    // }
    // else{
    //     treeNode_t* stmt = getES(node, curToken, nameTables);

    //     if(!result){
    //         result = stmt;
    //     }
    //     else{
    //         last->left = stmt;
    //     }

    //     last = stmt;
    // }

    nametable_t* nameTableToDelete = NULL;
    stackPop(nameTables, (void**)&nameTableToDelete);

    nameTableDtor(nameTableToDelete);
    free(nameTableToDelete);

    LPRINTF("Вышел из BLOCK. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getES(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в STATEMENT. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getStatement(node, curToken, nameTables);

    while((*curToken)->type == END_STATEMENT){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getBlock(node, curToken, nameTables);
        
        if(val2){
            val1 = createTreeNodeFromToken(&tempToken, val1, val2);
        }
    }

    LPRINTF("Вышел из STATEMENT. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getStatement(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    if ((*curToken)->type == INIT_FUNC)       return getFunc(node, curToken, nameTables);
    if ((*curToken)->type == WHILE)           return getWhile(node, curToken, nameTables);
    if ((*curToken)->type == IF)              return getIf(node, curToken, nameTables);
    if ((*curToken)->type == RETURN)          return getRet(node, curToken, nameTables);
    if ((*curToken)->type == INIT_VARIABLE ||
        (*curToken)->type == CALL_VARIABLE)   return getM(node, curToken, nameTables);
    if ((*curToken)->type == CALL_FUNC)       return getCallFunc(node, curToken, nameTables);
    if ((*curToken)->type == HLT)             return getHLT(node, curToken, nameTables);
    if ((*curToken)->type == OUT)             return getOut(node, curToken, nameTables);
    if ((*curToken)->type == IN)              return getIn(node, curToken, nameTables);

    return NULL;
}

static treeNode_t* getOut(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в OUT. Текущий токен: %p", *curToken);

    token_t tempToken = **curToken;
    EXPECT(curToken, OUT);

    treeNode_t* val1 = getCallFunc(node, curToken, nameTables);

    treeNode_t* result = createTreeNodeFromToken(&tempToken, val1, NULL);

    LPRINTF("Вышел из OUT. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getHLT(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в HLT. Текущий токен: %p", *curToken);

    treeNode_t* result = createTreeNodeFromToken(*curToken, NULL, NULL);
    EXPECT(curToken, HLT);

    LPRINTF("Вышел из HLT. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getRet(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в RET. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;

    token_t tempToken = **curToken;
    (*curToken)++;     

    val1 = getCallFunc(node, curToken, nameTables);

    val1 = createTreeNodeFromToken(&tempToken, val1, NULL);

    LPRINTF("Вышел из RET. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getFunc(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в Func. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;
    if((*curToken)->type == INIT_FUNC){
        (*curToken)++;
        token_t tempToken = **curToken;
        (*curToken)++;        

        val1 = getFuncArg(node, curToken, nameTables);

        treeNode_t* val2 = getBlock(node, curToken, nameTables);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из Func. Текущий токен: %p", *curToken);

    return val1;
}

// можно заобертить
static treeNode_t* getCallFunc(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в CallFunc. Текущий токен: %p", *curToken);

    // if(!checkExistsName(nameTables, *tokenStrData(*curToken))){
    //     SYNTAX_ERROR;
    //     return NULL;
    // }

    treeNode_t* val1 = NULL;
    if((*curToken)->type == CALL_FUNC){
        EXPECT(curToken, CALL_FUNC);
        token_t tempToken = **curToken;
        (*curToken)++;        

        val1 = getFuncArg(node, curToken, nameTables);

        val1 = createTreeNodeFromToken(&tempToken, val1, NULL);
    }
    else{
        val1 = getE(node, curToken, nameTables);
    }

    LPRINTF("Выхожу из CallFunc. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getIn(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    treeNode_t* result = NULL;
    if((*curToken)->type == IN){
        token_t tempToken = **curToken;
        (*curToken)++;  

        treeNode_t* val1 = getCallVar(node, curToken, nameTables);

        result = createTreeNodeFromToken(&tempToken, val1, NULL);
    }
    else{
        SYNTAX_ERROR;
    }

    return result;
}

static treeNode_t* getWhile(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в WHILE. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;
    if((*curToken)->type == WHILE){
        token_t tempToken = **curToken;
        (*curToken)++;        

        val1 = getIfWhileArg(node, curToken, nameTables);

        treeNode_t* val2 = getBlock(node, curToken, nameTables);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    EXPECT(curToken, END_BLOCK);

    LPRINTF("Вешел из WHILE. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getIf(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в IF. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;
    if((*curToken)->type == IF){
        token_t tempToken = **curToken;
        (*curToken)++;        

        val1 = getIfWhileArg(node, curToken, nameTables);

        treeNode_t* val2 = getBlock(node, curToken, nameTables);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    EXPECT(curToken, END_BLOCK);

    LPRINTF("Вешел из IF. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getIfWhileArg(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в WHILE_ARG. Текущий токен: %p", *curToken);

    treeNode_t* result = NULL;
    if((*curToken)->type == BRACKL){
        (*curToken)++;
        while((*curToken)->type != BRACKR){
            result = getEqual(node, curToken, nameTables);
        }
        (*curToken)++;
    }
    else{
        SYNTAX_ERROR;
    }

    LPRINTF("Вешел из WHILE_ARG. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getFuncArg(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в FuncArg. Текущий токен: %p", *curToken);

    treeNode_t* result = NULL;
    if((*curToken)->type == BRACKL){
        (*curToken)++;
        while((*curToken)->type != BRACKR){
            result = getComma(node, curToken, nameTables);
        }
        (*curToken)++;
    }
    else{
        SYNTAX_ERROR;
    }

    LPRINTF("Вышел из FuncArg. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getComma(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в Comma. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getName(node, curToken, nameTables);

    while((*curToken)->type == COMMA){
            token_t tempToken = **curToken;
            (*curToken)++;   

            treeNode_t* val2 = getName(node, curToken, nameTables);

            val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }
    if(!val1){
        SYNTAX_ERROR;
    }

    LPRINTF("Вышел из Comma. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getM(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в M. Текущий токен: %p", *curToken);

     treeNode_t* val1 = NULL;
    if((*curToken)->type == INIT_VARIABLE) val1 = getInitVar(node, curToken, nameTables);
    if((*curToken)->type == CALL_VARIABLE) val1 = getCallVar(node, curToken, nameTables);

    if((*curToken)->type == ASSIGN){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getCallFunc(node, curToken, nameTables);
        assert(val2);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из M. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getEqual(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    treeNode_t* val1 = getComparison(node, curToken, nameTables);

    while((*curToken)->type == EQUAL || (*curToken)->type == NOT_EQUAL){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getComparison(node, curToken, nameTables);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }


    return val1; 
}

static treeNode_t* getComparison(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    treeNode_t* val1 = getE(node, curToken, nameTables);

    while((*curToken)->type == GT || 
          (*curToken)->type == GE || 
          (*curToken)->type == LT || 
          (*curToken)->type == LE){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getE(node, curToken, nameTables);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    return val1; 
}

static treeNode_t* getE(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в E. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getT(node, curToken, nameTables);

    while((*curToken)->type == ADD || (*curToken)->type == SUB){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getT(node, curToken, nameTables);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из E. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getT(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в T. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getOperand(node, curToken, nameTables);

    while((*curToken)->type == MUL || (*curToken)->type == DIVIDE){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getOperand(node, curToken, nameTables);
        assert(val2);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из T. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getOperand(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    treeNode_t* result = NULL;
    if((*curToken)->type == NUMBER){
        result = getN(node, curToken, nameTables);
    }  
    else if((*curToken)->type == CALL_VARIABLE){
        EXPECT(curToken, CALL_VARIABLE);
        result = getName(node, curToken, nameTables);
    }

    return result;
}

static treeNode_t* getInitVar(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в initVar. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;
    if((*curToken)->type == INIT_VARIABLE){ //временно
        EXPECT(curToken, INIT_VARIABLE);

        if(checkExistsName(nameTables, *tokenStrData(*curToken))){
            SYNTAX_ERROR;
            return NULL;
        }

        val1 = getName(node, curToken, nameTables);
    }   

    LPRINTF("Выхожу из initVar. Текущий токен: %p", *curToken);

    return val1; 
}

static treeNode_t* getCallVar(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в callVar. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;
    if((*curToken)->type == CALL_VARIABLE){
        EXPECT(curToken, CALL_VARIABLE);
        
        if(!checkExistsName(nameTables, *tokenStrData(*curToken))){
            SYNTAX_ERROR;
            return NULL;
        }

        val1 = getName(node, curToken, nameTables);
    }   

    LPRINTF("Выхожу из callVar. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getName(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в NAME. Текущий токен: %p", *curToken);

    treeNode_t* name = NULL;
    if((*curToken)->type == NAME){
        nameTableAddElem(nameTables, *tokenStrData(*curToken), VARIABLE);
        name = createTreeNodeFromToken(*curToken, NULL, NULL);
        (*curToken)++;
    }

    LPRINTF("Выхожу из NAME. Текущий токен: %p", *curToken);
    
    return name;
}

static treeNode_t* getN(treeNode_t* node, token_t** curToken, stack* nameTables){
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

// static treeNode_t* getStart(treeNode_t* node, token_t** curToken, stack* nameTables){
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

    newNode->writeFile = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(newNode->writeFile);

    myStrCpy(newNode->writeFile, *tokenStrWriteFile(curToken));

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