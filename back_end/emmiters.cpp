#include "emitters.h"

#include "../core/core.h"
#include "../core/DSL.h"

#include "../general/debug.h"
#include "../general/strFunc.h"

#include <stdio.h>

void emitName     (treeNode_t* node, codeGenContext* context, variableScope scope);
void emitNumber   (treeNode_t* node, codeGenContext* context);
void emitAdd      (treeNode_t* node, codeGenContext* context);
void emitSub      (treeNode_t* node, codeGenContext* context);
void emitMul      (treeNode_t* node, codeGenContext* context);
void emitDiv      (treeNode_t* node, codeGenContext* context);
void emitIn       (treeNode_t* node, codeGenContext* context);
void emitOut      (treeNode_t* node, codeGenContext* context);
void emitHlt      (treeNode_t* node, codeGenContext* context);
void emitGlobalVar(treeNode_t* node, codeGenContext* context);
void emitLocaleVar(treeNode_t* node, codeGenContext* context);
void emitRet      (treeNode_t* node, codeGenContext* context);
void emitInitFunc (treeNode_t* node, codeGenContext* context);
void emitCallFunc (treeNode_t* node, codeGenContext* context, variableScope scope);
void emitPlug     (treeNode_t* node, codeGenContext* context);
void emitGt       (treeNode_t* node, codeGenContext* context);
void emitLt       (treeNode_t* node, codeGenContext* context);
void emitGe       (treeNode_t* node, codeGenContext* context);
void emitLe       (treeNode_t* node, codeGenContext* context);
void emitEqual    (treeNode_t* node, codeGenContext* context);
void emitNEqual   (treeNode_t* node, codeGenContext* context);
void emitWhile    (treeNode_t* node, codeGenContext* context, variableScope scope);
void emitIf       (treeNode_t* node, codeGenContext* context, variableScope scope);
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
    {EQUAL,         emitEqual },
    {NOT_EQUAL,     emitNEqual},
    {SQRT,          emitSqrt  },
    {IF,            emitPlug  },
    {WHILE,         emitPlug  },
    {IN,            emitPlug  },
    {OUT,           emitPlug  },
    {ASSIGN,        emitPlug  },
    {END_STATEMENT, emitPlug  },
    {MAIN,          emitPlug  }
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

void emitNode(treeNode_t* node, codeGenContext* context, variableScope scope){ 
    assert(node);
    assert(context);

    if(_NODE_TYPE(node) == NAME && _L(node) && _R(node)){
        emitInitFunc(node, context);
        
        if(_L(node)){
            emitNode(_L(node), context, LOCALE);
        }
        if(_R(node)){
            emitNode(_R(node), context, LOCALE);
        }
    }
    if(_NODE_TYPE(node) == NAME && _L(node) && !_R(node)){
        emitCallFunc(node, context, scope);
        return;
    }

    if(_NODE_TYPE(node) == WHILE){
        emitWhile(node, context, scope);
        return;
    }
    else if(_NODE_TYPE(node) == IN){
        emitIn(node, context);
        if (scope == GLOBAL)
            emitGlobalVar(node->left, context);
        else
            emitLocaleVar(node->left, context);
        return;
    }
    else if(_NODE_TYPE(node) == OUT){
        if (scope == GLOBAL)
            emitGlobalVar(node->left, context); 
        else
            emitLocaleVar(node->left, context);
        emitOut(node, context);
        return;
    }
    else if(_NODE_TYPE(node) == IF){
        emitIf(node, context, scope);
        return;
    }

    if(_NODE_TYPE(node) == ASSIGN){
        if(_R(node)){
            emitNode(_R(node), context, scope);
        }
        if(_L(node)){
            emitNode(_L(node), context, scope);
        }
    }
    else {
        if(_L(node)){
            emitNode(_L(node), context, scope);
        }
        if(_R(node)){
            emitNode(_R(node), context, scope);
        }
    }

    if(_NODE_TYPE(node) == NAME){

        emitName(node, context, scope);
        return;
    }

    emitter_t curEmitter = getEmitter(_NODE_TYPE(node));
    if(curEmitter){
        curEmitter(node, context);
    }
    else if(_NODE_TYPE(node) == NUMBER){
        emitNumber(node, context);
    }
}

void emitName(treeNode_t* node, codeGenContext* context, variableScope scope){
    assert(node);
    assert(context);

    LPRINTF("offset: %lu", _CONTEXT_STACK_FRAME_OFFSET(context));

    LPRINTF("зашел в emitName, node = %p", node);

    if(!_L(node) && !_R(node) && scope == GLOBAL){
        LPRINTF("Случай NAME глобальная переменная");
        emitGlobalVar(node, context);
        return;
    }
    else if(!_L(node) && !_R(node) && scope == LOCALE){
        LPRINTF("Случай NAME локальяная переменная");
        emitLocaleVar(node, context);
        return;
    }
    else{
        LPRINTF("Случай NAME не переменная");
    }

    LPRINTF("вышел из emitName, node = %p", node);
}

void emitNumber(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitNumber, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "PUSH %d\n", _NODE_VALUE_NUM(node));

    LPRINTF("вышел из emitNumber, node = %p", node);
}

void emitAdd(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitAdd, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "ADD\n");

    LPRINTF("вышел из emitAdd, node = %p", node);
}

void emitSub(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitSub, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "SUB\n");

    LPRINTF("вышел из emitSub, node = %p", node);
}

void emitMul(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitMul, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "MUL\n");

    LPRINTF("вышел из emitMul, node = %p", node);
}

void emitDiv(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitDiv, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "DIV\n");

    LPRINTF("вышел из emitDiv, node = %p", node);
}

void emitIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitIn, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "IN\n");

    LPRINTF("вышел из emitIn, node = %p", node);
}

void emitOut(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitOut, node = %p", node);
        
    fprintf(_CONTEXT_FILE_PTR(context), "OUT\n");

    LPRINTF("вышел из emitOut, node = %p", node);
}

void emitHlt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitHlt, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "HLT\n");

    LPRINTF("вышел из emitHlt, node = %p", node);
}

void emitSqrt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitSqrt, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "SQRT\n");

    LPRINTF("вышел из emitSqrt, node = %p", node);
}

void emitRet(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitRet, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "POPREG DX \n");

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG BX \n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG  AX \n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG  BX \n");

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG DX \n");

    fprintf(_CONTEXT_FILE_PTR(context), "RET\n");

    _CONTEXT_STACK_FRAME_OFFSET(context) = 0;

    LPRINTF("вышел из emitRet, node = %p", node);
}

void emitGt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitGt, node = %p", node);
    
    fprintf(_CONTEXT_FILE_PTR(context), "GT\n");

    LPRINTF("вышел из emitGt, node = %p", node);
}

void emitLt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitLt, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "LT\n");

    LPRINTF("вышел из emitLt, node = %p", node);
}

void emitGe(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitGe, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "GE\n");

    LPRINTF("вышел из emitGe, node = %p", node);
}

void emitLe(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitLe, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "LE\n");

    LPRINTF("вышел из emitLe, node = %p", node);
}

void emitEqual(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitEqual, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "EQ\n");

    LPRINTF("вышел из emitEqual, node = %p", node);
}

void emitNEqual(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitNEqual, node = %p", node);

    fprintf(_CONTEXT_FILE_PTR(context), "NEQ\n");

    LPRINTF("вышел из emitNEqual, node = %p", node);
}

void emitInitFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitInitFunc, node = %p", node);

    char* labelName = transliterate( _NODE_VALUE_STR(node));
    fprintf(_CONTEXT_FILE_PTR(context), "\n:%s\n", labelName);
    free(labelName);

    _CONTEXT_STACK_FRAME_OFFSET(context) = 0;

    LPRINTF("вышел из emitInitFunc, node = %p", node);
}

void emitCallFunc(treeNode_t* node, codeGenContext* context,  variableScope scope){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitCallFunc, node = %p", node);

    if(_L(node)){
        emitNode(_L(node), context, scope);
        fprintf(_CONTEXT_FILE_PTR(context), "\nPOPREG DX\n");
    }

    fprintf(_CONTEXT_FILE_PTR(context), "\nPUSHREG BX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "\nPUSHREG AX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG BX\n");

    fprintf(_CONTEXT_FILE_PTR(context), "\nPUSHREG AX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "PUSH    %d\n", _CONTEXT_STACK_FRAME_OFFSET(context));
    fprintf(_CONTEXT_FILE_PTR(context), "ADD \n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG AX\n");

    fprintf(_CONTEXT_FILE_PTR(context), "\nPUSHREG DX\n");

    char* labelName = transliterate( _NODE_VALUE_STR(node));
    fprintf(_CONTEXT_FILE_PTR(context), "CALL :%s\n", labelName);
    
    free(labelName);

    LPRINTF("вышел из emitCallFunc, node = %p", node);
}

//заобертить 
void emitLocaleVar(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);
    
    LPRINTF("зашел в emitLocaleVar, node = %p", node);


    int relativeAddr = getLocalVarAddr(_CONTEXT_NAMES(context), _NODE_VALUE_STR(node), &_CONTEXT_STACK_FRAME_OFFSET(context));

    fprintf(_CONTEXT_FILE_PTR(context), "\nPUSHREG AX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "PUSH   %d\n", relativeAddr);
    fprintf(_CONTEXT_FILE_PTR(context), "ADD       \n");

    fprintf(_CONTEXT_FILE_PTR(context), "POPREG CX \n");
    

    treeNode_t* parent = _PAR(node);
    if (parent &&((_NODE_TYPE(parent) == ASSIGN && node == _L(parent)) ||
                  (_NODE_TYPE(parent) == IN) ||
                  (_NODE_TYPE(parent) == NAME && _R(parent))))
    {
    fprintf(_CONTEXT_FILE_PTR(context), "POPM [CX]\n");
    return;
}
    fprintf(_CONTEXT_FILE_PTR(context), "PUSHM [CX]\n");

    LPRINTF("вышел из emitLocaleVar, node = %p", node);
}

void emitGlobalVar(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);   

    LPRINTF("зашел в emitGlobalVar, node = %p", node);

    int addr = getGlobalVarAddr(context->names, _NODE_VALUE_STR(node));

    treeNode_t* parent = _PAR(node);
    if((parent && _NODE_TYPE(parent) == ASSIGN && node == _L(parent)) || 
        (parent && _NODE_TYPE(parent) == IN)){
            fprintf(_CONTEXT_FILE_PTR(context), "POPMA %d\n", addr);
            return;
        }
    fprintf(_CONTEXT_FILE_PTR(context), "PUSHMA %d\n", addr);

    LPRINTF("вышел из emitGlobalVar, node = %p", node);
}

void emitPlug(treeNode_t* node, codeGenContext* context){
    return;
}

void emitWhile(treeNode_t* node, codeGenContext* context, variableScope scope){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitWhile, node = %p", node);

    label_t label1 = createLabel(_CONTEXT_LABELS(context), LABEL_PREFIX_WHILE_BEGIN);
    fprintf(_CONTEXT_FILE_PTR(context), "\n:%s_%d\n", _LABEL_DATA_PREFIX(&label1), _LABEL_DATA_ID(&label1));

    label_t label2 = createLabel(_CONTEXT_LABELS(context), LABEL_PREFIX_JBE_WHILE_END);;

    if(_L(node)){
        emitNode(_L(node), context, scope);
    }

    fprintf(_CONTEXT_FILE_PTR(context), "PUSH 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "JE :%s_%d\n", _LABEL_DATA_PREFIX(&label2), _LABEL_DATA_ID(&label2));

    if(_R(node)){
        emitNode(_R(node), context, scope);
    }    

    fprintf(_CONTEXT_FILE_PTR(context), "JMP :%s_%d\n", _LABEL_DATA_PREFIX(&label1), _LABEL_DATA_ID(&label1));

    fprintf(_CONTEXT_FILE_PTR(context), ":%s_%d\n\n", _LABEL_DATA_PREFIX(&label2), _LABEL_DATA_ID(&label2));

    LPRINTF("вышел из emitWhile, node = %p", node);
}

void emitIf(treeNode_t* node, codeGenContext* context, variableScope scope){
    assert(node);
    assert(context);

    LPRINTF("зашел в emitIf, node = %p", node);

    label_t label1 = createLabel(_CONTEXT_LABELS(context), LABEL_PREFIX_IF_FALSE_JMP);

    if(_L(node)){
        emitNode(_L(node), context, scope);
    }

    fprintf(_CONTEXT_FILE_PTR(context), "PUSH 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "JE :%s_%d\n", _LABEL_DATA_PREFIX(&label1), _LABEL_DATA_ID(&label1));

    if(_R(node)){
        emitNode(_R(node), context, scope);
    }    

    fprintf(_CONTEXT_FILE_PTR(context), ":%s_%d\n\n", _LABEL_DATA_PREFIX(&label1), _LABEL_DATA_ID(&label1));

    LPRINTF("вышел из emitIf, node = %p", node);
}