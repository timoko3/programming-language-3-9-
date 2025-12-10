#include "protection.h"

#include "../general/debug.h"
#include "../general/file.h"

#include <assert.h>

void dumpTokenSequence(tokensSequence_t* tokenSequence){
    assert(tokenSequence);

    fileDescription logFile = {
        LOG_FILE_NAME,
        "ab+"
    };    

    FILE* logFilePtr = myOpenFile(&logFile);
    if(!logFilePtr) return;

    for(size_t curTokenInd = 0; curTokenInd < tokenSequence->size; curTokenInd++){
        if(tokenSequence->data[curTokenInd].tClass != EMPTY_TOKEN){
            if(tokenSequence->data[curTokenInd].type != NUMBER){
                fprintf(logFilePtr, "token with index %lu, data: %s\n", curTokenInd, *tokenStrData(&tokenSequence->data[curTokenInd]));
            }
            else if(tokenSequence->data[curTokenInd].type == NUMBER){
                fprintf(logFilePtr, "token with index %lu, data: %d\n", curTokenInd, *tokenNumData(&tokenSequence->data[curTokenInd]));
            }
        }
    }

    fclose(logFilePtr);
}