// #include "back_end_nasm/emittersNasm.h"
#include "back_end_spu/emittersSpu.h"
#include "context.h"

#include "visitorAST.h"

#include "core/core.h"
#include "core/DSL.h"

#include <assert.h>

static ASTvisitorNode_t* getNodeVisitor(ASTnodeType typeNode, ASTvisitor_t* visitor);

// ASTvisitorNode_t astNodeVisitorsNasm[]{
//     {END_BLOCK,     NULL,              NULL,                NULL,                1},
//     {MAIN,          emitMainNasmPre,   NULL,                NULL,                1},
//     {FUNCTION,      emitFuncNasmPre,   emitFuncNasmIn,      emitFuncNasmPost,    1},
//     {RETURN,        emitRetNasmPre,    NULL,                emitRetNasmPost,     1},
//     {COMMA,         NULL,              NULL,                NULL,                1},
//     {IF,            NULL,              emitIfNasmIn,        emitIfNasmPost,      1},
//     {WHILE,         emitWhileNasmPre,  emitWhileNasmIn,     emitWhileNasmPost,   1},
//     {GT,            NULL,              emitBinaryOpNasmIn,  emitCmpNasmPost,     1},           
//     {LT,            NULL,              emitBinaryOpNasmIn,  emitCmpNasmPost,     1},
//     {GE,            NULL,              emitBinaryOpNasmIn,  emitCmpNasmPost,     1},
//     {LE,            NULL,              emitBinaryOpNasmIn,  emitCmpNasmPost,     1},
//     {EQUAL,         NULL,              emitBinaryOpNasmIn,  emitCmpNasmPost,     1},
//     {NOT_EQUAL,     NULL,              emitBinaryOpNasmIn,  emitCmpNasmPost,     1},

//     {END_STATEMENT, NULL,              NULL,                NULL,                1},
//     {ASSIGN,        NULL,              emitAssignNasmIn,    emitAssignNasmPost,  0},
//     {SUB,           NULL,              emitBinaryOpNasmIn,  emitSubNasmPost,     1},
//     {MUL,           NULL,              emitBinaryOpNasmIn,  emitMulNasmPost,     1},
//     {ADD,           NULL,              emitBinaryOpNasmIn,  emitAddNasmPost,     1},
//     {DIVIDE,        NULL,              emitBinaryOpNasmIn,  emitDivNasmPost,     1},
//     {HLT,           emitHltNasmPre,    NULL,                NULL,                1},
//     {VARIABLE,      emitVarNasmPre,    NULL,                NULL,                1},
//     {NUMBER,        emitNumberNasmPre, NULL,                NULL,                1},
//     {SQRT,          NULL,              NULL,                emitSqrtNasmPost,    1},
// };

ASTvisitorNode_t astNodeVisitorsSpu[]{
    {END_BLOCK,     NULL,              NULL,                NULL,                1},
    {MAIN,          emitMainSpuPre,    NULL,                emitMainSpuPost,     1},
    {FUNCTION,      emitFuncSpuPre,    emitFuncSpuIn,       emitFuncSpuPost,     1},
    {RETURN,        NULL,              NULL,                emitRetSpuPost,      1},
    {COMMA,         NULL,              NULL,                NULL,                1},
    {IF,            NULL,              emitIfSpuIn,         emitIfSpuPost,       1},
    {WHILE,         emitWhileSpuPre,   emitWhileSpuIn,      emitWhileSpuPost,    1},
    {GT,            NULL,              NULL,                NULL,                1},           
    {LT,            NULL,              NULL,                NULL,                1},
    {GE,            NULL,              NULL,                NULL,                1},
    {LE,            NULL,              NULL,                NULL,                1},
    {EQUAL,         NULL,              NULL,                NULL,                1},
    {NOT_EQUAL,     NULL,              NULL,                NULL,                1},

    {END_STATEMENT, NULL,              NULL,                NULL,                1},
    {ASSIGN,        NULL,              emitAssignSpuIn,     emitAssignSpuPost,   0},
    {ADD,           NULL,              NULL,                emitAddSpuPost,      1},
    {SUB,           NULL,              NULL,                emitSubSpuPost,      1},
    {MUL,           NULL,              NULL,                emitMulSpuPost,      1},
    {DIVIDE,        NULL,              NULL,                emitDivSpuPost,      1},
    {SQRT,          NULL,              NULL,                emitSqrtSpuPost,     1},
    {HLT,           NULL,              NULL,                emitHltSpuPost,      1},
    {VARIABLE,      emitVarSpuPre,     NULL,                NULL,                1},
    {NUMBER,        emitNumberSpuPre,  NULL,                NULL,                1},
};

// const size_t NASM_NODE_VISITORS_AMOUNT = sizeof(astNodeVisitorsNasm) / sizeof(ASTvisitorNode_t);
const size_t SPU_NODE_VISITORS_AMOUNT  = sizeof(astNodeVisitorsSpu) / sizeof(ASTvisitorNode_t);


void traversalCondOrder(treeNode_t* node, ASTvisitor_t* visitor){
    assert(visitor);

    ASTvisitorNode_t* curNodeVisitor = getNodeVisitor(_NODE_TYPE(node), visitor);

    if(VISITOR_PRE_ORDER_FUNC(curNodeVisitor)) VISITOR_PRE_ORDER_FUNC(curNodeVisitor)(node, VISITOR_CONTEXT(visitor));

    if(VISITOR_IS_LR_ORDER(curNodeVisitor)){
        if(_L(node)){
            traversalCondOrder(_L(node), visitor);
        }
    }
    else{
        if(_R(node)){
            traversalCondOrder(_R(node), visitor);
        }
    }
    

    if(VISITOR_IN_ORDER_FUNC(curNodeVisitor)) VISITOR_IN_ORDER_FUNC(curNodeVisitor)(node, VISITOR_CONTEXT(visitor));

    if(VISITOR_IS_LR_ORDER(curNodeVisitor)){
        if(_R(node)){
            traversalCondOrder(_R(node), visitor);
        }
    }
    else{
        if(_L(node)){
            traversalCondOrder(_L(node), visitor);
        }
    }

    if(VISITOR_POST_ORDER_FUNC(curNodeVisitor)) VISITOR_POST_ORDER_FUNC(curNodeVisitor)(node, VISITOR_CONTEXT(visitor));

}

// void traversalRlAST(treeNode_t* node, ASTvisitor_t* visitor){
//     assert(visitor);

//     ASTvisitorNode_t* curNodeVisitor = getNodeVisitor(_NODE_TYPE(node), visitor);

//     if(VISITOR_PRE_ORDER_FUNC(curNodeVisitor)) VISITOR_PRE_ORDER_FUNC(curNodeVisitor)(node, VISITOR_CONTEXT(visitor));

//     if(_(Rnode)){
//         traversalRlAST(_L(node), visitor);
//     }

//     if(VISITOR_IN_ORDER_FUNC(curNodeVisitor)) VISITOR_IN_ORDER_FUNC(curNodeVisitor)(node, VISITOR_CONTEXT(visitor));

//     if(_R(node)){
//         traversalRlAST(_R(node), visitor);
//     }

//     if(VISITOR_POST_ORDER_FUNC(curNodeVisitor)) VISITOR_POST_ORDER_FUNC(curNodeVisitor)(node, VISITOR_CONTEXT(visitor));

// }

static ASTvisitorNode_t* getNodeVisitor(ASTnodeType typeNode, ASTvisitor_t* visitor){
    assert(visitor);

    for(size_t curVisiterInd = 0; curVisiterInd < VISITORS_AMOUNT(visitor); curVisiterInd++){
        if(typeNode == VISITOR_TYPE((&NODE_VISITORS(visitor)[curVisiterInd]))){
            return (&NODE_VISITORS(visitor)[curVisiterInd]);
        }
    }

    return NULL;
}