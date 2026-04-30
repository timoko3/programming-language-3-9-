#include "emitters.h"

#include "core/core.h"
#include "core/DSL.h"

#include <assert.h>

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
};

static emitRule emittersTable[] = {
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
    {END_STATEMENT, emitPlug  },
    {MAIN,          emitPlug  },
    // {POPM,          emitPopm  },
    // {DRAW,          emitDraw  }
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

    emitter_t curEmitter = getEmitter(_NODE_TYPE(node));    
    if(curEmitter){
        curEmitter(node, context);
    }
}

void emitPlug(treeNode_t* node, codeGenContext* context){
    return;
}