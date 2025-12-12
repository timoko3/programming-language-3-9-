#ifndef SYNTAX_ANALYZE_H
#define SYNTAX_ANALYZE_H

#include "tokens.h"
#include "../general/tree.h"

treeNode_t* syntaxAnalyze(tree_t* syntaxTree, tokensSequence_t* tokensSequence);

#endif /* SYNTAX_ANALYZE_H */