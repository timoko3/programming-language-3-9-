#ifndef EMITTERS_H
#define EMITTERS_H

#include "codeGeneration.h"

#include "../core/core.h"
#include "../core/expressionTree.h"

typedef void (*emitter_t) (treeNode_t*, codeGenContext*);

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
};

static emitRule emittersTable[] = {
    {NUMBER, emitNumber},
    {ADD, emitAdd},
    {SUB, emitSub},
    {MUL, emitMul},
    {IN,  emitIn},
    {OUT, emitOut},
    {DIVIDE, emitDiv},
    {ASSIGN, emitAssign},
    {IF, emitIf},
    {WHILE, emitWhile},
    {RETURN, emitReturn},
    {END_STATEMENT, emitPlug},
    {MAIN,          emitPlug}
};

const size_t EMIT_TABLE_SIZE = sizeof(emittersTable) / sizeof(emitRule);

void emitNode(treeNode_t* node, codeGenContext* context);

#endif /* EMITTERS_H*/