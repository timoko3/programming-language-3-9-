#include "taleEmitters.h"

#include "../core/DSL.h"

#include <assert.h>

// void taleEmitAdd      (treeNode_t* node, FILE* taleFilePtr);
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

void taleEmitNumber   (treeNode_t* node, FILE* taleFilePtr);
void taleEmitName     (treeNode_t* node, FILE* taleFilePtr);
void taleEmitNodeValue(treeNode_t* node, FILE* taleFilePtr);
void taleEmitVariable (treeNode_t* node, FILE* taleFilePtr);
void taleEmitMain     (treeNode_t* node, FILE* taleFilePtr);
void taleEmitInOutRet    (treeNode_t* node, FILE* taleFilePtr);
void taleEmitComma    (treeNode_t* node, FILE* taleFilePtr);

void taleEmitCallFunc(treeNode_t* node, FILE* taleFilePtr);
void taleEmitInitFunc(treeNode_t* node, FILE* taleFilePtr);

struct taleEmitRule{
    ASTnodeType  type;
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

void taleEmitNode(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    if(_NODE_TYPE(node) == IN || _NODE_TYPE(node) == OUT || _NODE_TYPE(node) == RETURN){
        taleEmitInOutRet(node, taleFilePtr);
        return;
    }
    else if(_NODE_TYPE(node) == NAME){
        taleEmitName(node, taleFilePtr);
        if(_R(node)){
            taleEmitNode(_R(node), taleFilePtr);
        }
        return;
    }

    if(_L(node)){
        taleEmitNode(_L(node), taleFilePtr);
    }

    taleEmitter_t curTaleEmitter = getTaleEmitter(_NODE_TYPE(node));
    if(curTaleEmitter){
        curTaleEmitter(node, taleFilePtr);
    }

    if(_R(node)){
        taleEmitNode(_R(node), taleFilePtr);
    }

}

void taleEmitNumber(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "%d ", _NODE_VALUE_NUM(node));
}

void taleEmitNodeValue(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "%s ", _NODE_VALUE_STR(node));

    if(_NODE_TYPE(node) == END_STATEMENT){
        fprintf(taleFilePtr, "\n");
    }
}

void taleEmitName(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    if(!_L(node) && !_R(node)){
        taleEmitVariable(node, taleFilePtr);
    }
    else if(_L(node) && !_R(node)){
        taleEmitCallFunc(node, taleFilePtr);
    }
    else if(_L(node) && _R(node)){
        taleEmitInitFunc(node, taleFilePtr);
    }
}

void taleEmitVariable(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    treeNode_t* parent = _PAR(node);

    if((parent && _NODE_TYPE(parent) == ASSIGN && node == _L(parent))){
        fprintf(taleFilePtr, "Добрый молодец %s ", _NODE_VALUE_STR(node));
    }
    else{
        fprintf(taleFilePtr, "сила %s ", _NODE_VALUE_STR(node));
    }
}

void taleEmitCallFunc(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "Сивка-бурка вещая каурка стань передо мной как лист перед травой %s(", _NODE_VALUE_STR(node));

    if(_L(node)){
        taleEmitComma(_L(node), taleFilePtr);
    }

    fprintf(taleFilePtr, ")");
}

void taleEmitInitFunc(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "В некотором царстве, в некотором государстве %s(", _NODE_VALUE_STR(node));

    if(_L(node)){
        taleEmitComma(_L(node), taleFilePtr);
    }

    fprintf(taleFilePtr, ")\n");
}

void taleEmitMain(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "В некотором царстве, в некотором государстве %s() \n", _NODE_VALUE_STR(node));
}

void taleEmitInOutRet(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "%s ", _NODE_VALUE_STR(node));
    taleEmitVariable(_L(node), taleFilePtr);
}

void taleEmitComma(treeNode_t* node, FILE* taleFilePtr){
    assert(node);
    assert(taleFilePtr);

    if(_NODE_TYPE(node) == NAME){
        fprintf(taleFilePtr, "%s", _NODE_VALUE_STR(node));
        return;
    }

    while(_NODE_TYPE(node) == COMMA){
        fprintf(taleFilePtr, ", ");

        taleEmitVariable(_R(node), taleFilePtr);
    }
}





