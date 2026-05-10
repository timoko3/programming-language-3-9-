#include "visitorAST.h"

#include "generator.h"

#include "core/DSL.h"

#include "general/file.h"
#include "general/hash.h"

#include "back_end_x86elf/emitBinaryCommands.h"

#include <malloc.h>
#include <assert.h>

#ifdef NASM
static void genPreambleNasm(codeGenContext* context);
static void genEpilogueNasm(codeGenContext* context);
#endif /* NASM */

#ifdef X86ELF
#include "back_end_x86elf/elfGenerate.h"
#endif /* X86ELF */

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

#ifdef NASM
void genCodeNasm(FILE* filePtr, tree_t* AST, codeGenContext* context, list_t* varMap, list_t* regTable, labelsTable_t* labelsTable){
    assert(filePtr);
    assert(AST);
    assert(context);
    assert(varMap);
    assert(regTable);
    assert(labelsTable);

    initContextNasm(context, filePtr, regTable, varMap, labelsTable);

    ASTvisitor_t visitorAstNasm = {
        astNodeVisitorsNasm,
        NASM_NODE_VISITORS_AMOUNT,
        context
    };

    genPreambleNasm(context);
    traversalCondOrder(AST->root, &visitorAstNasm);
    genEpilogueNasm(context);
}

static void genPreambleNasm(codeGenContext* context){
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "section .text\n");
    fprintf(_CONTEXT_FILE_PTR(context), "global _start\n");
}

static void genEpilogueNasm(codeGenContext* context){
    assert(context);


}

#endif /* NASM */

#ifdef SPU
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
#endif /* SPU */

#ifdef X86ELF
void genCodeX86ELF(FILE* filePtr, tree_t* AST, codeGenContext* context, list_t* varMap, list_t* regTable, labelsTable_t* labelsTable){
    assert(filePtr);
    assert(AST);
    assert(context);
    assert(varMap);
    assert(regTable);
    assert(labelsTable);

    initContextX86Elf(context, filePtr, regTable, varMap, labelsTable);

    ASTvisitor_t visitorAstX86Elf = {
        astNodeVisitorsX86Elf,
        X86ELF_NODE_VISITORS_AMOUNT,
        context
    };

    _MOV("[rax + 4]", "rbx");
    _MOV("rax", "rbx");
    _MOV("rcx", "rdx");
    _MOV("rsi", "123");
    _MOV("rdi", "rsi");
    _MOV("r8", "rsi");
    _MOV("r8", "r15");
    _MOV("r10", "117");
    genPrologueX86Elf(context);
    // traversalCondOrder(AST->root, &visitorAstX86Elf);
    // genEpilogueNasm(context);

    binBufferDtor(_CONTEXT_ELF_CODE_BUFFER(context));
}

#endif /* X86ELF */