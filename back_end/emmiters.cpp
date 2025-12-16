#include "emitters.h"

#include "../core/core.h"
#include "../core/DSL.h"

#include <stdio.h>

void emitNumber(treeNode_t* node, codeGenContext* context);
void emitAdd(treeNode_t* node, codeGenContext* context);
void emitSub(treeNode_t* node, codeGenContext* context);
void emitMul(treeNode_t* node, codeGenContext* context);
void emitDiv(treeNode_t* node, codeGenContext* context);
void emitIn (treeNode_t* node, codeGenContext* context);
void emitOut(treeNode_t* node, codeGenContext* context);
void emitHlt(treeNode_t* node, codeGenContext* context);
void emitVar(treeNode_t* node, codeGenContext* context);
void emitPlug(treeNode_t* node, codeGenContext* context);


typedef void (*emitter_t) (treeNode_t*, codeGenContext*);

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
};

static emitRule emittersTable[] = {
    {NUMBER, emitNumber},
    {ADD,    emitAdd},
    {SUB,    emitSub},
    {MUL,    emitMul},
    {DIVIDE, emitDiv},
    {IN,     emitIn },
    {OUT,    emitOut},
    {HLT,    emitHlt},
    {IF,            emitPlug},
    {WHILE,         emitPlug},
    {RETURN,        emitPlug},
    {ASSIGN,        emitPlug},
    {END_STATEMENT, emitPlug},
    {MAIN,          emitPlug}
};

const size_t EMIT_TABLE_SIZE = sizeof(emittersTable) / sizeof(emitRule);

emitter_t getEmitter(ASTnodeType type){
    for(size_t curEmitRuleInd = 0; curEmitRuleInd < EMIT_TABLE_SIZE; curEmitRuleInd++){
        if(type == _EMIT_RULE_TYPE(&emittersTable[curEmitRuleInd])){
            return _EMIT_RULE_EMITTER(&emittersTable[curEmitRuleInd]);
        }
    }

    return NULL;
}   

void emitNode(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    if(_L(node)){
        emitNode(_L(node), context);
    }

    if(_R(node)){
        emitNode(_R(node), context);
    }

    emitter_t curEmitter = getEmitter(_NODE_TYPE(node));
    if(curEmitter){
        curEmitter(node, context);
    }
    else if(_NODE_TYPE(node) == NUMBER){
        emitNumber(node, context);
    }
    else if(_NODE_TYPE(node) == NAME){
        if(_L(node) || _R(node)){
            // emitFunc(node, context);
        }
        else{
            emitVar(node, context);
        }
    }
}

void emitNumber(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "PUSH %d\n", _NODE_VALUE_NUM(node));
}

void emitAdd(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "ADD\n");
}

void emitSub(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "SUB\n");
}

void emitMul(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "MUL\n");
}

void emitDiv(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "DIV\n");
}

void emitIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "IN\n");
}

void emitOut(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "OUT\n");
}
void emitHlt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "HLT\n");
}

void emitFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    if(node->right){
        fprintf(_CONTEXT_FILE_PTR(context), "HLT\n");
    }
    else{
        fprintf(_CONTEXT_FILE_PTR(context), "")
    }
}

void emitVar(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);   

    int addr = getVarAddr(context->names, _NODE_VALUE_STR(node));
    if(_NODE_TYPE(_PAR(node)) == ASSIGN){
        fprintf(_CONTEXT_FILE_PTR(context), "POPMA %d\n", addr);
    }
    else{
        fprintf(_CONTEXT_FILE_PTR(context), "PUSHMA %d\n", addr);
    }
}

void emitPlug(treeNode_t* node, codeGenContext* context){
    return;
}