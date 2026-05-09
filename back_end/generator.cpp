#include "visitorAST.h"

#include "generator.h"

#include "core/DSL.h"

#include "general/file.h"
#include "general/hash.h"

#include <malloc.h>
#include <assert.h>

static void genPreambleNasm(codeGenContext* context);
static void genEpilogueNasm(codeGenContext* context);

void genCode(tree_t* AST, const char* destFileName, generator_t generator){
    assert(AST);
    assert(destFileName);

    fileDescription asmFile{
        destFileName,
        "wb"
    };

    FILE* outputFilePtr = myOpenFile(&asmFile);
    assert(outputFilePtr);

    codeGenContext context;
    list_t varMap;
    list_t regTable;
    labelsTable_t labelsTable;

    generator(outputFilePtr, AST, &context, &varMap, &regTable, &labelsTable);

    listDtor(&varMap, varMapElemDtor);
    listDtor(_CONTEXT_REG_TABLE(&context), regTableElemDtor);
    listDtor(_CONTEXT_LABELS_TABLE(&context), labelDtor);

    fclose(outputFilePtr);
}

// void genCodeNasm(FILE* filePtr, tree_t* AST, codeGenContext* context, list_t* varMap, list_t* regTable, labelsTable_t* labelsTable){
//     assert(filePtr);
//     assert(AST);
//     assert(context);
//     assert(varMap);
//     assert(regTable);
//     assert(labelsTable);

//     initContextNasm(context, filePtr, regTable, varMap, labelsTable);

//     ASTvisitor_t visitorAstNasm = {
//         astNodeVisitorsNasm,
//         NASM_NODE_VISITORS_AMOUNT,
//         context
//     };

//     genPreambleNasm(context);
//     traversalCondOrder(AST->root, &visitorAstNasm);
//     genEpilogueNasm(context);
// }

void genCodeSpu(FILE* filePtr, tree_t* AST, codeGenContext* context, list_t* varMap, list_t* regTable, labelsTable_t* labelsTable){
    assert(filePtr);
    assert(AST);
    assert(context);
    assert(varMap);
    assert(regTable);
    assert(labelsTable);

    initContextSpu(context, filePtr, regTable, varMap, labelsTable);

    ASTvisitor_t visitorAstNasm = {
        astNodeVisitorsSpu,
        SPU_NODE_VISITORS_AMOUNT,
        context
    };

    traversalCondOrder(AST->root, &visitorAstNasm);
}

static void genPreambleNasm(codeGenContext* context){
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "section .text\n");
    fprintf(_CONTEXT_FILE_PTR(context), "global _start\n");
}

static void genEpilogueNasm(codeGenContext* context){
    assert(context);


}