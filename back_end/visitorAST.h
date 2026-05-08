#ifndef VISITOR_H
#define VISITOR_H

#include "general/tree/tree.h"
#include "context.h"

struct ASTvisitorNode_t{
    ASTnodeType   type;
    visitorFunc_t preOrderfunc;
    visitorFunc_t inOrderfunc;
    visitorFunc_t postOrderfunc;
};
typedef void (*visitorFunc_t)(codeGenContext* context);

struct ASTvisitor_t{
    ASTvisitorNode_t* nodeVisitors;
    size_t            amountNodeVisitors;
    codeGenContext*   context;
};

void traversalLrAST(treeNode_t* node, ASTvisitor_t* visitor);

#endif /* VISITOR_H */