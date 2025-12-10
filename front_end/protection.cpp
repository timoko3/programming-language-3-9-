#include "protection.h"

#include "../general/debug.h"
#include "../general/file.h"

#include <assert.h>

void dumpTokenSequence(token_t* tokenSequence){
    assert(tokenSequence);

    fileDescription logFile = {
        LOG_FILE_NAME,
        "ab+"
    };    

    FILE* logFilePtr = myOpenFile(&logFile);
    if(!logFilePtr) return;

    for(size_t curTokenInd = 0; curTokenInd < TOKENS_COUNT /*FIX WRONG token count*/; curTokenInd++){
        if(tokenSequence[curTokenInd].type != NUMBER){
            fprintf(logFilePtr, "token with index %lu, data: %s\n", curTokenInd, *tokenStrData(&tokenSequence[curTokenInd]));
        }
        else if(tokenSequence[curTokenInd].type == NUMBER){
            fprintf(logFilePtr, "token with index %lu, data: %d\n", curTokenInd, *tokenNumData(&tokenSequence[curTokenInd]));
        }
    }

    fclose(logFilePtr);
}