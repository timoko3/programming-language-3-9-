#ifndef VISITOR_H
#define VISITOR_H

#include "general/tree/tree.h"

struct codeGenContext;

typedef void (*visitorFunc_t)(treeNode_t* ,codeGenContext*);
struct ASTvisitorNode_t{
    ASTnodeType   type;
    visitorFunc_t preOrderfunc;
    visitorFunc_t inOrderfunc;
    visitorFunc_t postOrderfunc;
    bool          isLrOrderFlag;
};

struct ASTvisitor_t{
    ASTvisitorNode_t* nodeVisitors;
    size_t            amountNodeVisitors;
    codeGenContext*   context;
};

// extern ASTvisitorNode_t astNodeVisitorsNasm[];
extern ASTvisitorNode_t astNodeVisitorsSpu[];
// extern const size_t NASM_NODE_VISITORS_AMOUNT;
extern const size_t SPU_NODE_VISITORS_AMOUNT;

void traversalCondOrder(treeNode_t* node, ASTvisitor_t* visitor);

#endif /* VISITOR_H */