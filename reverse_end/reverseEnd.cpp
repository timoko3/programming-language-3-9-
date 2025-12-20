#include "reverseEnd.h"
#include "taleEmitters.h"

#include "../core/core.h"

#include "../general/file.h"

#include <assert.h>
#include <stdio.h>

void genTaleCode(tree_t* syntaxTree, const char* taleFileName){
    assert(syntaxTree);

    fileDescription taleFile{
        taleFileName,
        "wb"
    };

    FILE* taleFilePtr = myOpenFile(&taleFile);
    assert(taleFilePtr);

    fprintf(taleFilePtr, "Жили-были\n");

    taleEmitNode(syntaxTree->root, taleFilePtr, GLOBAL);
    
    fprintf(taleFilePtr, "\nи я там был мед пиво пил по усам текло, а в рот не попало\n");

    fclose(taleFilePtr);
}

