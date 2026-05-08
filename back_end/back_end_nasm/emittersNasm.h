#ifndef EMITTERS_X86
#define EMITTERS_X86

#include "generator.h"

typedef void (*emitter_t) (treeNode_t*, codeGenContext*);

void emitStartNasm(treeNode_t* node, codeGenContext* context);
emitter_t getEmitter(ASTnodeType type);

#endif /* EMITERS_X86 */