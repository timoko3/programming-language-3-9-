#include "visitorAST.h"

#include "core/core.h"
#include "core/DSL.h"

#include "general/tree/tree.h"

#include "context.h"

#include <assert.h>

struct ASTvisitorNode_t{
    ASTnodeType type;
    visitorFunc_t preOrderfunc;
    visitorFunc_t inOrderfunc;
    visitorFunc_t inOrderfunc;
};
typedef void (*visitorFunc_t)(treeNode_t*, ASTvisitorNode_t*);

struct ASTvisitor_t{
    ASTvisitorNode_t* nodeVisitors;
    codeGenContext* context;
};

void traversalLRAST(treeNode_t* node, ASTvisitor_t* visitor){
    assert(visitor);

    

    if(_L(node)){
        traversalLRAST(_L(node), visitor);
    }

    if(_R(node)){
        traversalLRAST(_R(node), visitor);
    }

}