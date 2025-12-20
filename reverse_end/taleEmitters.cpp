#include "taleEmitters.h"

#include "../core/DSL.h"

#include <assert.h>

// void taleEmitAdd      (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
// void taleEmitSub      (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitMul      (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitDiv      (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitIn       (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitOut      (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitHlt      (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitRet      (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitGt       (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitLt       (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitGe       (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitLe       (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitSqrt     (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitIf       (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitWhile    (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitVar      (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitInitFunc (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitCallFunc (treeNode_t* node, FILE* taleFilePtr);
// void taleEmitPlug     (treeNode_t* node, FILE* taleFilePtr);

void taleEmitNumber   (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitName     (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitNodeValue(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitVariable (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitMain     (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitInOutRet (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitComma    (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitWhile    (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitIf       (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitRet      (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);

void taleEmitCallFunc (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);
void taleEmitInitFunc (treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables);

struct taleEmitRule{
    ASTnodeType   type;
    taleEmitter_t emitter;
};

static taleEmitRule taleEmittersTable[] = {
    {NUMBER,        taleEmitNumber    },
    {ADD,           taleEmitNodeValue },
    {SUB,           taleEmitNodeValue },
    {MUL,           taleEmitNodeValue },
    {DIVIDE,        taleEmitNodeValue },
    {HLT,           taleEmitNodeValue },
    {GT,            taleEmitNodeValue },           
    {LT,            taleEmitNodeValue },
    {GE,            taleEmitNodeValue },
    {LE,            taleEmitNodeValue },
    {SQRT,          taleEmitNodeValue },
    {IF,            taleEmitNodeValue },
    {WHILE,         taleEmitNodeValue },
    {ASSIGN,        taleEmitNodeValue },
    {END_STATEMENT, taleEmitNodeValue },
    {MAIN,          taleEmitMain      },
    {COMMA,         taleEmitNodeValue }
};

const size_t TALE_EMIT_TABLE_SIZE = sizeof(taleEmittersTable) / sizeof(taleEmitRule);

taleEmitter_t getTaleEmitter(ASTnodeType type){
    for(size_t curEmitRuleInd = 0; curEmitRuleInd < TALE_EMIT_TABLE_SIZE; curEmitRuleInd++){
        if(type == _EMIT_RULE_TYPE(&taleEmittersTable[curEmitRuleInd])){
            return _EMIT_RULE_EMITTER(&taleEmittersTable[curEmitRuleInd]);
        }
    }

    return NULL;
}   

void taleEmitNode(treeNode_t* node, FILE* taleFilePtr, variableScope scope, stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    if(_NODE_TYPE(node) == IN || _NODE_TYPE(node) == OUT){
        taleEmitInOutRet(node, taleFilePtr, scope, nameTables);
        return;
    }
    else if(_NODE_TYPE(node) == NAME){
        taleEmitName(node, taleFilePtr, scope, nameTables);
        if(_R(node)){
            taleEmitNode(_R(node), taleFilePtr, scope, nameTables);
        }

        // if(_R(node) && _L(node)){
        //     nametable_t* nameTableToDelete = NULL;
        //     stackPop(nameTables, (void**)&nameTableToDelete);

        //     nameTableDtor(nameTableToDelete);
        //     free(nameTableToDelete);
        // }

        return;
    }
    else if(_NODE_TYPE(node) == WHILE){
        taleEmitWhile(node, taleFilePtr, scope, nameTables);
        return;
    }
    else if(_NODE_TYPE(node) == IF){
        taleEmitIf(node, taleFilePtr, scope, nameTables);
        return;
    }
    else if(_NODE_TYPE(node) == RETURN){
        taleEmitRet(node, taleFilePtr, scope, nameTables);
        return;
    }

    if(_L(node)){
        taleEmitNode(_L(node), taleFilePtr, scope, nameTables);
    }

    taleEmitter_t curTaleEmitter = getTaleEmitter(_NODE_TYPE(node));
    if(curTaleEmitter){
        curTaleEmitter(node, taleFilePtr, scope, nameTables);
    }

    if(_R(node)){
        taleEmitNode(_R(node), taleFilePtr, scope, nameTables);
    }

}

void taleEmitNumber(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "%d ", _NODE_VALUE_NUM(node));
}

void taleEmitNodeValue(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "%s ", _NODE_VALUE_STR(node));

    if(_NODE_TYPE(node) == END_STATEMENT){
        fprintf(taleFilePtr, "\n");
    }
}

void taleEmitName(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    if(!_L(node) && !_R(node)){
        taleEmitVariable(node, taleFilePtr, scope, nameTables);
    }
    else if(_L(node) && !_R(node)){
        taleEmitCallFunc(node, taleFilePtr, scope, nameTables);
    }
    else if(_L(node) && _R(node)){
        taleEmitInitFunc(node, taleFilePtr, scope, nameTables);
    }
}

void taleEmitVariable(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    // bool variableExists = false;

    // if(checkExistsName(nameTables, _NODE_VALUE_STR(node))){
    //     variableExists = true;
    // }
    // else{
    //     nameTableAddElem(nameTables, _NODE_VALUE_STR(node), VARIABLE);
    // }

    treeNode_t* parent = _PAR(node);

    if((parent && _NODE_TYPE(parent) == ASSIGN && node == _L(parent))){
        fprintf(taleFilePtr, "Добрый молодец %s ", _NODE_VALUE_STR(node));
    }
    // else if(variableExists){
        fprintf(taleFilePtr, "сила %s ", _NODE_VALUE_STR(node));
    // }
}

void taleEmitCallFunc(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "Сивка-бурка вещая каурка стань передо мной как лист перед травой %s(", _NODE_VALUE_STR(node));

    if(_L(node)){
        taleEmitComma(_L(node), taleFilePtr, scope, nameTables);
    }

    fprintf(taleFilePtr, ")");
}

void taleEmitInitFunc(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    nametable_t* newNameTable = (nametable_t*) calloc(1, sizeof(nametable_t));
    assert(newNameTable);

    nameTableCtor(newNameTable);
    stackPush(nameTables, newNameTable);

    fprintf(taleFilePtr, "В некотором царстве, в некотором государстве %s(", _NODE_VALUE_STR(node));

    if(_L(node)){
        taleEmitComma(_L(node), taleFilePtr, scope, nameTables);
    }

    fprintf(taleFilePtr, ")\n");
}

void taleEmitMain(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "В некотором царстве, в некотором государстве %s() \n", _NODE_VALUE_STR(node));
}

void taleEmitInOutRet(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "%s ", _NODE_VALUE_STR(node));
    taleEmitVariable(_L(node), taleFilePtr, scope, nameTables);
}

void taleEmitComma(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    if(_NODE_TYPE(node) == NAME){
        fprintf(taleFilePtr, "%s", _NODE_VALUE_STR(node));
        return;
    }

    while(_NODE_TYPE(node) == COMMA){
        fprintf(taleFilePtr, ", ");

        taleEmitVariable(_R(node), taleFilePtr, scope, nameTables);
    }
}

void taleEmitWhile(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "Покуда(");

    if(_L(node)){
        taleEmitNode(_L(node), taleFilePtr, scope, nameTables);
    }

    fprintf(taleFilePtr, ")\n");

    if(_R(node)){
        taleEmitNode(_R(node), taleFilePtr, scope, nameTables);
    }
}

void taleEmitIf(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "ежели случилось, что(");

    if(_L(node)){
        taleEmitNode(_L(node), taleFilePtr, scope, nameTables);
    }

    fprintf(taleFilePtr, ")\n");

    if(_R(node)){
        taleEmitNode(_R(node), taleFilePtr, scope, nameTables);
    }
}

void taleEmitRet(treeNode_t* node, FILE* taleFilePtr, variableScope scope,  stack* nameTables){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "воротить");

    if(_L(node)){
        taleEmitNode(_L(node), taleFilePtr, scope, nameTables);
    }

}




