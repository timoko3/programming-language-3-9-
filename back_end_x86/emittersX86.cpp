#include "emittersX86.h"

#include "core/core.h"
#include "core/DSL.h"

#define DEBUG

#include "general/debug.h"

#include <assert.h>

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
};

void emitEb(treeNode_t* node, codeGenContext* context);
void emitBlock(treeNode_t* node, codeGenContext* context);
void emitMain(treeNode_t* node, codeGenContext* context);
void emitEs(treeNode_t* node, codeGenContext* context);
void emitStatement(treeNode_t* node, codeGenContext* context);
void emitAssign(treeNode_t* node, codeGenContext* context);
void emitVar(treeNode_t* node, codeGenContext* context);
void emitExpression(treeNode_t* node, codeGenContext* context);
void emitNumber(treeNode_t* node, codeGenContext* context);

inline void emitNonTerminal(treeNode_t* node, codeGenContext* context);

void emitPlug(treeNode_t* node, codeGenContext* context);

static emitRule emittersTable[] = {
    {END_BLOCK,     emitEb       },
    {MAIN,          emitMain     },
    {END_STATEMENT, emitEs       },
    {ASSIGN,        emitAssign   },
    {VARIABLE,      emitVar      },
    {NUMBER,        emitNumber   },
    // {NUMBER,        emitNumber},
    // {ADD,           emitAdd   },
    // {SUB,           emitSub   },
    // {MUL,           emitMul   },
    // {DIVIDE,        emitDiv   },
    // {HLT,           emitHlt   },
    // {RETURN,        emitRet   },
    // {GT,            emitGt    },           
    // {LT,            emitLt    },
    // {GE,            emitGe    },
    // {LE,            emitLe    },
    // {EQUAL,         emitEqual },
    // {NOT_EQUAL,     emitNEqual},
    // {SQRT,          emitSqrt  },
    {IF,            emitPlug  },
    {WHILE,         emitPlug  },
    {IN,            emitPlug  },
    {OUT,           emitPlug  },
    {ASSIGN,        emitPlug  },
    {MAIN,          emitPlug  },
    // {POPM,          emitPopm  },
    // {DRAW,          emitDraw  }
};

const size_t EMIT_TABLE_SIZE = sizeof(emittersTable) / sizeof(emitRule);

#define GET_FUNC_NAME(f) ( #f )

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

    LPRINTF("emitNode start");

    emitNonTerminal(node, context);

    LPRINTF("emitNode end");
}

void emitEb(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitEb start");

    if(_L(node)){
        emitBlock(_L(node), context);
    }

    if(_R(node)){
        emitBlock(_R(node), context);
    }

    LPRINTF("emitEb end");
}

void emitBlock(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitBlock start");

    emitNonTerminal(node, context);

    LPRINTF("emitBlock end");
}

void emitMain(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMain start");

    fprintf(_CONTEXT_FILE_PTR(context), "_start:\n");

    if(_R(node)){
        emitBlock(_R(node), context);
    }

    LPRINTF("emitMain end");
}

void emitEs(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitEs start");

    if(_L(node)){
        emitStatement(_L(node), context);
    }

    if(_R(node)){
        emitStatement(_R(node), context);
    }

    LPRINTF("emitEs end");
}

void emitStatement(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitStatement start");

    emitNonTerminal(node, context);

    LPRINTF("emitStatement end");
}

void emitAssign(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitAssign start");

    fprintf(_CONTEXT_FILE_PTR(context), "mov ");    

    if(_L(node)){
        emitVar(_L(node), context);
    }
    
    fprintf(_CONTEXT_FILE_PTR(context), ", ");    

    if(_R(node)){
        emitExpression(_R(node), context);
    }

    fprintf(_CONTEXT_FILE_PTR(context), "\n");    

    LPRINTF("emitAssign end");
}

void emitVar(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitVar end");

    regTableElem_t* refReg = regTableElemCtor(NONE, "", STORE_VAR, PZN_VARIABLE_CODE, 0);
    assert(refReg);

    regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findFreeRegStoreValRule, refReg);
    assert(foundReg);

    sscanf(_NODE_WRITE_FILE(node), "VAR%d", &REG_TABLE_ELEM_VARIABLE_CODE(foundReg));
    REG_TABLE_ELEM_USE_BIT(foundReg) = 1;

    printf("code = %d\n", REG_TABLE_ELEM_VARIABLE_CODE(foundReg));

    fprintf(_CONTEXT_FILE_PTR(context), "%s", REG_TABLE_ELEM_NAME(foundReg));

    regTableElemDtor(refReg);

    LPRINTF("emitVar end");
}

void emitExpression(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitExpression start");

    emitNonTerminal(node, context);

    LPRINTF("emitExpression end");    
}

void emitNumber(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitNumber start");

    fprintf(_CONTEXT_FILE_PTR(context), "%d", _NODE_VALUE_NUM(node));

    LPRINTF("emitNumber end");
}

void emitPlug(treeNode_t* node, codeGenContext* context){
    LPRINTF("plug");
    return;
}

inline void emitNonTerminal(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitter_t curEmitter = getEmitter(_NODE_TYPE(node));    
    if(curEmitter){
        curEmitter(node, context);
    }
}

// inline void emitBinaryOp(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);


// }