
#ifndef LEXICAL_ANALYZE_H
#define LEXICAL_ANALYZE_H

#include "../general/tokens/tokens.h"
#include "../core/core.h"

tokensSequence_t* tokenize(char* curBufferPos, tokensSequence_t* tokensSequence);

#endif /* LEXICAL_ANALYZE_H */