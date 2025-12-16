#include "emitters.h"

#include "../core/core.h"
#include "../core/DSL.h"

#include <stdio.h>

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
            emitFunc(node, context);
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

void emitFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);


}

void emitVar(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);   
}

void emitPlug(treeNode_t* node, codeGenContext* context){
    return;
}