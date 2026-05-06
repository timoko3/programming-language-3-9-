// #include "emitters.h"

// #include "../core/DSL.h"

// #include "../general/file.h"

// #include <assert.h>

// const char* ASM_FILE_NAME = "program.txt";

// void genAsmCode(tree_t* syntaxTree){
//     assert(syntaxTree);

//     fileDescription asmFile{
//         ASM_FILE_NAME,
//         "wb"
//     };

//     FILE* asmFilePtr = myOpenFile(&asmFile);
//     assert(asmFilePtr);

//     codeGenContext context;

//     _CONTEXT_FILE_PTR(&context) = asmFilePtr;
//     context.names  = spuNameTableCtor(NULL);
//     context.labels = labelsTableCtor(NULL);
//     context.stackFrameOffset = 0;

//     emitNode(syntaxTree->root, &context);

//     spuNameTableDtor(context.names);
//     labelsTableDtor(context.labels);

//     fclose(asmFilePtr);
// }



