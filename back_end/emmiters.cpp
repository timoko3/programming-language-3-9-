#include "emitters.h"
#include "labels.h"

#include "../core/core.h"
#include "../core/DSL.h"

#include <stdio.h>

void emitNumber   (treeNode_t* node, codeGenContext* context);
void emitAdd      (treeNode_t* node, codeGenContext* context);
void emitSub      (treeNode_t* node, codeGenContext* context);
void emitMul      (treeNode_t* node, codeGenContext* context);
void emitDiv      (treeNode_t* node, codeGenContext* context);
void emitIn       (treeNode_t* node, codeGenContext* context);
void emitOut      (treeNode_t* node, codeGenContext* context);
void emitHlt      (treeNode_t* node, codeGenContext* context);
void emitVar      (treeNode_t* node, codeGenContext* context);
void emitRet      (treeNode_t* node, codeGenContext* context);
void emitInitFunc (treeNode_t* node, codeGenContext* context);
void emitCallFunc (treeNode_t* node, codeGenContext* context);
void emitPlug     (treeNode_t* node, codeGenContext* context);
void emitGt       (treeNode_t* node, codeGenContext* context);
void emitLt       (treeNode_t* node, codeGenContext* context);
void emitGe       (treeNode_t* node, codeGenContext* context);
void emitLe       (treeNode_t* node, codeGenContext* context);
void emitWhile    (treeNode_t* node, codeGenContext* context);
void emitIf       (treeNode_t* node, codeGenContext* context);
void emitSqrt     (treeNode_t* node, codeGenContext* context);

typedef void (*emitter_t) (treeNode_t*, codeGenContext*);

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
};

static emitRule emittersTable[] = {
    {NUMBER,        emitNumber},
    {ADD,           emitAdd   },
    {SUB,           emitSub   },
    {MUL,           emitMul   },
    {DIVIDE,        emitDiv   },
    {HLT,           emitHlt   },
    {RETURN,        emitRet   },
    {GT,            emitGt    },           
    {LT,            emitLt    },
    {GE,            emitGe    },
    {LE,            emitLe    },
    {SQRT,          emitSqrt  },
    {IF,            emitPlug  },
    {WHILE,         emitPlug  },
    {IN,            emitPlug  },
    {OUT,           emitPlug  },
    {ASSIGN,        emitPlug  },
    {END_STATEMENT, emitPlug  },
    {MAIN,          emitPlug  }
};

static int cycleLabelNumber = 0;

const size_t EMIT_TABLE_SIZE = sizeof(emittersTable) / sizeof(emitRule);

emitter_t getEmitter(ASTnodeType type){
    for(size_t curEmitRuleInd = 0; curEmitRuleInd < EMIT_TABLE_SIZE; curEmitRuleInd++){
        if(type == _EMIT_RULE_TYPE(&emittersTable[curEmitRuleInd])){
            return _EMIT_RULE_EMITTER(&emittersTable[curEmitRuleInd]);
        }
    }

    return NULL;
}   

void emitNode(treeNode_t* node, codeGenContext* context){   ///ASSIGN закостылена из-за формата дерева
    assert(node);
    assert(context);

    if(_NODE_TYPE(node) == NAME && _L(node) && _R(node)){
        emitInitFunc(node, context);
    }

    if(_NODE_TYPE(node) == WHILE){
        emitWhile(node, context);
        return;
    }
    else if(_NODE_TYPE(node) == IN){
        emitIn(node, context);
        emitVar(node->left, context);
        return;
    }
    else if(_NODE_TYPE(node) == OUT){
        emitVar(node->left, context);
        emitOut(node, context);
        return;
    }
    else if(_NODE_TYPE(node) == IF){

    }

    if(_NODE_TYPE(node) == ASSIGN){
        if(_R(node)){
            emitNode(_R(node), context);
        }
        if(_L(node)){
            emitNode(_L(node), context);
        }
    }
    else{
        if(_L(node)){
            emitNode(_L(node), context);
        }
        if(_R(node)){
            emitNode(_R(node), context);
        }
    }

    emitter_t curEmitter = getEmitter(_NODE_TYPE(node));
    if(curEmitter){
        curEmitter(node, context);
    }
    else if(_NODE_TYPE(node) == NUMBER){
        emitNumber(node, context);
    }
    else if(_NODE_TYPE(node) == NAME){
        if(!_R(node) && !_L(node)){
            emitVar(node, context);
        }
        else if(!_R(node)){
            emitCallFunc(node, context);
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

void emitSqrt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "SQRT\n");
}

void emitRet(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "RET\n");
}

void emitGt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "GT\n");
}

void emitLt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "LT\n");
}

void emitGe(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "GE\n");
}

void emitLe(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "LE\n");
}

void emitInitFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "\n:%s\n", "factorial");
}

void emitCallFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "CALL :%s\n", "factorial");
}

void emitVar(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);   

    int addr = getVarAddr(context->names, _NODE_VALUE_STR(node));
    if(_NODE_TYPE(_PAR(node)) == ASSIGN || _NODE_TYPE(_PAR(node)) == IN){
        fprintf(_CONTEXT_FILE_PTR(context), "POPMA %d\n", addr);
    }
    else if(!(_NODE_TYPE(_PAR(node)) == NAME) && !(_NODE_TYPE(_PAR(node)) == OUT)){
        fprintf(_CONTEXT_FILE_PTR(context), "PUSHMA %d\n", addr);
    }
    else if((_NODE_TYPE(_PAR(node)) == OUT)){
        fprintf(_CONTEXT_FILE_PTR(context), "PUSHMA %d\n", addr);
    }
}

void emitPlug(treeNode_t* node, codeGenContext* context){
    return;
}

void emitWhile(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    int label1 = getLabelName(context->labels, _NODE_VALUE_STR(node));
    fprintf(_CONTEXT_FILE_PTR(context), "\n:%d\n", label1);

    int label2 = getLabelName(context->labels, _NODE_VALUE_STR(node));

    if(_L(node)){
        emitNode(_L(node), context);
    }

    int label3 = getLabelName(context->labels, _NODE_VALUE_STR(node));

    fprintf(_CONTEXT_FILE_PTR(context), "PUSH 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "JBE :%d\n", cycleLabelNumber);

    if(_R(node)){
        emitNode(_R(node), context);
    }    

    fprintf(_CONTEXT_FILE_PTR(context), "JMP :%d\n", label1);

    fprintf(_CONTEXT_FILE_PTR(context), ":%d\n\n", label2);

    cycleLabelNumber++;
}

void emitIf(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    int label1 = cycleLabelNumber++;

    if(_L(node)){
        emitNode(_L(node), context);
    }

    fprintf(_CONTEXT_FILE_PTR(context), "PUSH 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "JAE :%d\n", label1);

    if(_R(node)){
        emitNode(_R(node), context);
    }    

    fprintf(_CONTEXT_FILE_PTR(context), ":%d\n\n", label1);
}