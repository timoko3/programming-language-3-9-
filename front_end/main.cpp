#include "../general/tree.h"
#include "../expressionTree.h"

#include "general/file.h"

#include <stdio.h>
#include <assert.h>

const char* CODE_FILE_NAME = "test.tale";

int main(){
    fileDescription codeFile{
        CODE_FILE_NAME,
        "rb"
    };

    FILE* codeFilePtr = myOpenFile(&codeFile);
    assert(codeFilePtr);

    treeNode_t* tokenizedCode = tokenize(codeFilePtr);
}