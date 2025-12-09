#include "lexicalAnalyze.h"

#include "../general/file.h"
// #include "../general/tree.h"

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

const char* CODE_FILE_NAME = "beta.tale";

int main(){
    token_t* tokenSequence = tokenize(CODE_FILE_NAME);

    tokenSequenceDtor(tokenSequence);
    free(tokenSequence);   
}