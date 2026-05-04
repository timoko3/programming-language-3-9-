#ifndef EMITTERS_X86
#define EMITTERS_X86

#include "nasmGeneration.h"

typedef varMapElem_t* (*emitter_t) (treeNode_t*, codeGenContext*);

varMapElem_t* emitNode(treeNode_t* node, codeGenContext* context);
emitter_t getEmitter(ASTnodeType type);

#endif /* EMITERS_X86 */