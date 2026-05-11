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

    // _MOV("[rax + 4]", "rbx");
    _IMUL("rax", "rbx");
    _IMUL("r8", "r15");
    _IDIV("r8");
    _IDIV("rbx");
    _IDIV("rcx");
    _MOV("rax", "rbx");
    _ADD("rax", "rbx");
    _SUB("r8", "r9");
    _SUB("r12", "5");
    _MOV("rcx", "rdx");
    _MOV("rsi", "123");
    _MOV("rdi", "rsi");
    _MOV("r8", "rsi");
    _MOV("r8", "r15");
    _MOV("r10", "117");

    _MOV("rax", "1");
    _MOV("rbx", "2");
    _MOV("rcx", "3");
    _MOV("rdx", "4");

    _MOV("r8", "5");
    _MOV("r9", "6");
    _MOV("r10", "7");
    _MOV("r15", "8");

    _MOV("rax", "rbx");
    _MOV("rbx", "rax");
    _MOV("rcx", "rdx");
    _MOV("rdx", "rcx");

    _MOV("r8", "rax");
    _MOV("rax", "r8");

    _MOV("r9", "r10");
    _MOV("r10", "r9");

    _MOV("r15", "r8");
    _MOV("r8", "r15");

    _MOV("[rax]", "rbx");
    _MOV("[rbx]", "rax");

    _MOV("[rax + 4]", "rbx");
    _MOV("[rbx + 8]", "rcx");

    _MOV("[r8 + 16]", "rax");
    _MOV("[r9 + 32]", "r15");

    _CALL("superpuperduperdumblabel13");
    _SYSCALL();
    _PUSH("5");
    _PUSH("rax");
    _PUSH("r8");
    _PUSH("[rax + 4]");
// // 
    _POP("rax");
    _POP("r8");
    _POP("[rax + 5]");
// 
    // _MOV("[r10 + 1]", "r11");
    // _MOV("[r12 + 127]", "r13");
// 
    // _MOV("[rax + 128]", "rbx");
    // _MOV("[r8 + 1024]", "r9");
// 
    // _MOV("rax", "[rbx]");
    // _MOV("rcx", "[rax + 4]");
// 
    // _MOV("r8", "[r9]");
    // _MOV("r10", "[r15 + 8]");
// 
    // _MOV("rax", "[r8 + 128]");
    // _MOV("r15", "[r10 + 1024]");
// 
    // _MOV("[rax]", "123");
    // _MOV("[rbx + 4]", "456");
// 
    // _MOV("[r8 + 8]", "789");
    // _MOV("[r15 + 16]", "111");
// 
    // _MOV("rax", "123");
    // _MOV("rbx", "456");
// 
    // _MOV("r8", "777");
    // _MOV("r15", "999");
// 
    // _MOV("r10", "-1");
    // _MOV("r11", "-123");
// 
    // _MOV("[rax + 4]", "-1");
    // _MOV("[r8 + 8]", "-5");
    genPrologueX86Elf(context);
    // traversalCondOrder(AST->root, &visitorAstX86Elf);
    // genEpilogueNasm(context);

    binBufferDtor(_CONTEXT_ELF_CODE_BUFFER(context));
}

#endif /* X86ELF */