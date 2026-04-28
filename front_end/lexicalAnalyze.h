
#ifndef LEXICAL_ANALYZE_H
#define LEXICAL_ANALYZE_H

#include "general/stack/stack.h"
#include "general/tokens/tokens.h"
#include "general/hashTable/hashTable.h"
#include "core/core.h"

tokensSequence_t* tokenize(char* curBufferPos, tokensSequence_t* tokensSequence, bool debugSymbols);

#endif /* LEXICAL_ANALYZE_H */