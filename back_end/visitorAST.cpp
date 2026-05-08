#include "visitorAST.h"

#include "core/core.h"
#include "core/DSL.h"

#include <assert.h>

static ASTvisitorNode_t* getNodeVisitor(ASTnodeType typeNode, ASTvisitor_t* visitor);

ASTvisitorNode_t astNodeVisitors[]{
    {END_BLOCK,     emitEbNasmPre,     emitEbNasmIn,     emitEbNasmPost     },
    {MAIN,          emitMainNasmPre,   emitMainNasmIn,   emitMainNasmPost   },
    {FUNCTION,      emitFuncPre,       emitFuncIn,       emitFuncPost       },
    {RETURN,        emitRetNasmPre,    emitRetNasmIn,    emitRetNasmPost    },
    {COMMA,         emitCommaNasmPre,  emitCommaNasmIn,  emitCommaNasmPost  },
    {IF,            emitIfNasmPre,     emitIfNasmIn,     emitIfNasmPost     },
    {WHILE,         emitWhileNasmPre,  emitWhileNasmIn,  emitWhileNasmPost  },
    {END_STATEMENT, emitEsNasmPre,     emitEsNasmIn,     emitEsNasmPost     },
    {ASSIGN,        emitAssignNasmPre, emitAssignNasmIn, emitAssignNasmPost },
    {SUB,           emitSubNasmPre,    emitSubNasmIn,    emitSubNasmPost    },
    {MUL,           emitMulNasmPre,    emitMulNasmIn,    emitMulNasmPost    },
    {ADD,           emitAddNasmPre,    emitAddNasmIn,    emitAddNasmPost    },
    {DIVIDE,        emitDivNasmPre,    emitDivNasmIn,    emitDivNasmPost    },
    {HLT,           emitHltNasmPre,    emitHltNasmIn,    emitHltNasmPost    },
    {VARIABLE,      emitVarNasmPre,    emitVarNasmIn,    emitVarNasmPost    },
    {NUMBER,        emitNumNasmPre,    emitNumNasmIn,    emitNumNasmPost    },
    {SQRT,          emitSqrtNasmPre,   emitSqrtNasmIn,   emitSqrtNasmPost   },
};

void traversalLrAST(treeNode_t* node, ASTvisitor_t* visitor){
    assert(visitor);

    ASTvisitorNode_t* curNodeVisitor = getNodeVisitor(_NODE_TYPE(node), visitor);

    VISITOR_PRE_ORDER_FUNC(curNodeVisitor)(VISITOR_CONTEXT(visitor));

    if(_L(node)){
        traversalLrAST(_L(node), visitor);
    }

    VISITOR_IN_ORDER_FUNC(curNodeVisitor)(VISITOR_CONTEXT(visitor));

    if(_R(node)){
        traversalLrAST(_R(node), visitor);
    }

    VISITOR_POST_ORDER_FUNC(curNodeVisitor)(VISITOR_CONTEXT(visitor));

}

static ASTvisitorNode_t* getNodeVisitor(ASTnodeType typeNode, ASTvisitor_t* visitor){
    assert(visitor);

    for(size_t curVisiterInd = 0; curVisiterInd < VISITORS_AMOUNT(visitor); curVisiterInd++){
        if(typeNode == VISITOR_TYPE((&NODE_VISITORS(visitor)[curVisiterInd]))){
            return (&NODE_VISITORS(visitor)[curVisiterInd]);
        }
    }

    return NULL;
}