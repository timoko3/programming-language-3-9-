#ifndef EMITTERS_X86
#define EMITTERS_X86

#include "nasmGeneration.h"

void emitNode(treeNode_t* node, codeGenContext* context, variableScope scope = GLOBAL);
emitter_t getEmitter(ASTnodeType type);

#endif /* EMITERS_X86 */