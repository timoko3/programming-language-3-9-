
#include "syntaxAnalyze.h"
#include "../general/tokens/protectionTokens.h"
#include "../core/DSL.h"
#include "../general/nametable.h"
#include "../general/stack/stack.h"

#include "../general/debug.h"
#include "../general/strFunc.h"

#include <assert.h>
#include <malloc.h>

static treeNode_t* getG          (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getM          (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getE          (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getT          (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getP          (treeNode_t* node, token_t** curToken, stack* nameTables);
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
static treeNode_t* getSqrt       (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getPopm       (treeNode_t* node, token_t** curToken, stack* nameTables);
static treeNode_t* getDraw       (treeNode_t* node, token_t** curToken, stack* nameTables);

static treeNode_t* createTreeNodeFromToken(token_t* curToken, treeNode_t* left, treeNode_t* right);

static void SyntaxError(const char* file, int line, const char* func);
static void expect(token_t** curToken, ASTnodeType expected, const char* file, int line, const char* func);

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
    if(_TOKEN_TYPE(*curToken) != END_PROGRAM){
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

    while(_TOKEN_TYPE(*curToken) == END_BLOCK){
        token_t tempToken = **curToken;
        (*curToken)++;
        
        treeNode_t* val2 = getBlock(node, curToken, nameTables);

        if(val2){
            val1 = createTreeNodeFromToken(&tempToken, val1, val2);
            myStrCpy(_NODE_WRITE_FILE(val1), ";");
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

    while(_TOKEN_TYPE(*curToken) == INIT_FUNC ||
          _TOKEN_TYPE(*curToken) == WHILE ||
          _TOKEN_TYPE(*curToken) == IF ||
          _TOKEN_TYPE(*curToken) == INIT_VARIABLE ||
          _TOKEN_TYPE(*curToken) == CALL_VARIABLE ||
          _TOKEN_TYPE(*curToken) == CALL_FUNC ||
          _TOKEN_TYPE(*curToken) == RETURN ||
          _TOKEN_TYPE(*curToken) == OUT ||
          _TOKEN_TYPE(*curToken) == IN ||
          _TOKEN_TYPE(*curToken) == POPM ||
          _TOKEN_TYPE(*curToken) == DRAW ||
          _TOKEN_TYPE(*curToken) == MAIN ||
          _TOKEN_TYPE(*curToken) == HLT){
        treeNode_t* stmt = getES(node, curToken, nameTables);
        if (!stmt) break;

        if (!result) result = stmt;
        else _L(last) = stmt;

        last = stmt;

        if (_TOKEN_TYPE(*curToken) == END_STATEMENT)(*curToken)++;
    }

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

    while(_TOKEN_TYPE(*curToken) == END_STATEMENT){
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

    if (_TOKEN_TYPE(*curToken) == INIT_FUNC || 
        _TOKEN_TYPE(*curToken) == MAIN)            return getFunc(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == WHILE)           return getWhile(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == IF)              return getIf(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == RETURN)          return getRet(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == INIT_VARIABLE ||
        _TOKEN_TYPE(*curToken) == CALL_VARIABLE)   return getM(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == CALL_FUNC)       return getCallFunc(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == HLT)             return getHLT(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == OUT)             return getOut(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == IN)              return getIn(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == POPM)            return getPopm(node, curToken, nameTables);
    if (_TOKEN_TYPE(*curToken) == DRAW)            return getDraw(node, curToken, nameTables);

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

static treeNode_t* getPopm(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в PopM. Текущий токен: %p", *curToken);

    token_t tempToken = **curToken;
    EXPECT(curToken, POPM);

    treeNode_t* val1 = getCallFunc(node, curToken, nameTables);

    treeNode_t* result = createTreeNodeFromToken(&tempToken, val1, NULL);

    LPRINTF("Вышел из Draw. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getDraw(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в Draw. Текущий токен: %p", *curToken);

    token_t tempToken = **curToken;
    EXPECT(curToken, DRAW);

    treeNode_t* val1   = getCallFunc(node, curToken, nameTables);

    treeNode_t* result = createTreeNodeFromToken(&tempToken, val1, NULL);

    LPRINTF("Вышел из Draw. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getHLT(treeNode_t* /*node*/, token_t** curToken, stack* /*nameTables*/){
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
    if(_TOKEN_TYPE(*curToken) == INIT_FUNC){
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
    if(_TOKEN_TYPE(*curToken) == CALL_FUNC){
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
    if(_TOKEN_TYPE(*curToken) == IN){
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
    if(_TOKEN_TYPE(*curToken) == WHILE){
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
    if(_TOKEN_TYPE(*curToken) == IF){
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
    if(_TOKEN_TYPE(*curToken) == BRACKL){
        (*curToken)++;
        while(_TOKEN_TYPE(*curToken) != BRACKR){
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
    if(_TOKEN_TYPE(*curToken) == BRACKL){
        (*curToken)++;
        while(_TOKEN_TYPE(*curToken) != BRACKR){
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

    while(_TOKEN_TYPE(*curToken) == COMMA){
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
    if(_TOKEN_TYPE(*curToken) == INIT_VARIABLE) val1 = getInitVar(node, curToken, nameTables);
    if(_TOKEN_TYPE(*curToken) == CALL_VARIABLE) val1 = getCallVar(node, curToken, nameTables);

    if(_TOKEN_TYPE(*curToken) == ASSIGN){
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

    while(_TOKEN_TYPE(*curToken) == EQUAL || _TOKEN_TYPE(*curToken) == NOT_EQUAL){
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

    while(_TOKEN_TYPE(*curToken) == GT || 
          _TOKEN_TYPE(*curToken) == GE || 
          _TOKEN_TYPE(*curToken) == LT || 
          _TOKEN_TYPE(*curToken) == LE){
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

    while(_TOKEN_TYPE(*curToken) == ADD || _TOKEN_TYPE(*curToken) == SUB){
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

    treeNode_t* val1 = getSqrt(node, curToken, nameTables);

    while(_TOKEN_TYPE(*curToken) == MUL || _TOKEN_TYPE(*curToken) == DIVIDE){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getSqrt(node, curToken, nameTables);
        assert(val2);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из T. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getSqrt(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в SQRT. Текущий токен: %p", *curToken);

    treeNode_t* val1 = getP(node, curToken, nameTables);

    while(_TOKEN_TYPE(*curToken) == SQRT){
        token_t tempToken = **curToken;
        (*curToken)++;

        treeNode_t* val2 = getP(node, curToken, nameTables);
        assert(val2);

        val1 = createTreeNodeFromToken(&tempToken, val1, val2);
    }

    LPRINTF("Выхожу из SQRT. Текущий токен: %p", *curToken);

    return val1;
}

static treeNode_t* getP(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в P. Текущий токен: %p", *curToken);

    treeNode_t* result = NULL;
    if(_TOKEN_TYPE(*curToken) == BRACKL){
        EXPECT(curToken, BRACKL);

        result = getCallFunc(node, curToken, nameTables);

        EXPECT(curToken, BRACKR);

    }
    else{
        result = getOperand(node, curToken, nameTables);
    }

    LPRINTF("Выхожу из P. Текущий токен: %p", *curToken);

    return result;
}

static treeNode_t* getOperand(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    treeNode_t* result = NULL;
    if(_TOKEN_TYPE(*curToken) == NUMBER){
        result = getN(node, curToken, nameTables);
    }  
    else if(_TOKEN_TYPE(*curToken) == CALL_VARIABLE){
        EXPECT(curToken, CALL_VARIABLE);
        result = getName(node, curToken, nameTables);
    }
    else if(_TOKEN_TYPE(*curToken) == CALL_FUNC){
        result = getCallFunc(node, curToken, nameTables);
    }

    return result;
}

static treeNode_t* getInitVar(treeNode_t* node, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в initVar. Текущий токен: %p", *curToken);

    treeNode_t* val1 = NULL;
    if(_TOKEN_TYPE(*curToken) == INIT_VARIABLE){ //временно
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
    if(_TOKEN_TYPE(*curToken) == CALL_VARIABLE){
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

static treeNode_t* getName(treeNode_t* /*node*/, token_t** curToken, stack* nameTables){
    assert(curToken);

    LPRINTF("Зашел в NAME. Текущий токен: %p", *curToken);

    treeNode_t* name = NULL;
    if(_TOKEN_TYPE(*curToken) == NAME){
        nameTableAddElem(nameTables, *tokenStrData(*curToken), VARIABLE);
        name = createTreeNodeFromToken(*curToken, NULL, NULL);
        (*curToken)++;
    }

    LPRINTF("Выхожу из NAME. Текущий токен: %p", *curToken);
    
    return name;
}

static treeNode_t* getN(treeNode_t* /*node*/, token_t** curToken, stack* /*nameTables*/){
    assert(curToken);

    LPRINTF("Зашел в N. Текущий токен: %p", *curToken);

    treeNode_t* number = NULL;
    if(_TOKEN_TYPE(*curToken) == NUMBER){
        number = createTreeNodeFromToken(*curToken, NULL, NULL);
        (*curToken)++;
    }

    LPRINTF("Выхожу из N. Текущий токен: %p", *curToken);

    return number;
}

static treeNode_t* createTreeNodeFromToken(token_t* curToken, treeNode_t* left, treeNode_t* right){
    assert(curToken);

    treeNode_t* newNode = createNewNode(left, right);
    assert(newNode);

    _NODE_TYPE(newNode) = _TOKEN_TYPE(curToken);

    _NODE_WRITE_FILE(newNode) = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
    assert(_NODE_WRITE_FILE(newNode));

    myStrCpy(_NODE_WRITE_FILE(newNode), *tokenStrWriteFile(curToken));

    if(_TOKEN_TYPE(curToken) != NUMBER){
        _NODE_VALUE_STR(newNode) = (char*) calloc(MAX_VARIABLE_SIZE, sizeof(char));
        assert(_NODE_VALUE_STR(newNode));

        myStrCpy(_NODE_VALUE_STR(newNode), *tokenStrData(curToken));
    }
    else{
        _NODE_VALUE_NUM(newNode) = *tokenNumData(curToken);
    }

    return newNode;
}

static void expect(token_t** curToken, ASTnodeType expected, const char* file, int line, const char* func){
    assert(curToken);
    assert(file);
    assert(func);

    if (_TOKEN_TYPE(*curToken) != expected){
        SyntaxError(file, line, func);
    }
    (*curToken)++;
}

static void SyntaxError(const char* file, int line, const char* func){
    assert(file);
    assert(func);

    printf("Синтаксическая ошибка!! %s:%d:%s", file, line, func);
}