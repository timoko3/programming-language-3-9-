#include "emittersNasm.h"
// #include "format.h"

#include "core/core.h"
#include "core/DSL.h"

#define DEBUG

#include "general/debug.h"

#include <assert.h>
#include <malloc.h>

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
};

void loadToReg(varMapElem_t* var, codeGenContext* context);

    // {IN,            emitPlug     },
    // {OUT,           emitPlug     },
    // {POPM,          emitPopm  },
    // {DRAW,          emitDraw  }

const char* MAIN_START_NAME_NASM    = "_start";

const char* ADD_OPERATION_NAME_NASM = "add";
const char* SUB_OPERATION_NAME_NASM = "sub" ;
const char* MUL_OPERATION_NAME_NASM = "imul";
const char* DIV_OPERATION_NAME_NASM = "idiv";

const char* SQRT_OPERATION_NAME_NASM = "sqrtss";

const char* CMP_OPERATION_NAME_NASM = "cmp";
const char* JG_OPERATION_NAME_NASM = "jg";
const char* JGE_OPERATION_NAME_NASM = "jge";
const char* JL_OPERATION_NAME_NASM  = "jl";
const char* JLE_OPERATION_NAME_NASM = "jle";
const char* JE_OPERATION_NAME_NASM  = "je";
const char* JNE_OPERATION_NAME_NASM = "jne";

void emitMainNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMain start");

    fprintf(_CONTEXT_FILE_PTR(context), "%s:\n", MAIN_START_NAME_NASM);

    LPRINTF("emitMain end");
}

void emitFuncNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFuncNasmPre(node, context);
    }
    else{
        emitCallFuncNasmPre(node, context);
    }

    LPRINTF("emitFunc end");
}

void emitFuncNasmIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFuncNasmIn(node, context);
    }

    LPRINTF("emitFunc end");
}

void emitFuncNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFuncNasmPost(node, context);
    }
    else{
        emitCallFuncNasmPost(node, context);
    }

    LPRINTF("emitFunc end");
}

void emitCallFuncNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);
    _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;
    _CONTEXT_IS_CALL_FUNC_ARG(context) = 1;  
}

void emitCallFuncNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "call %s\n", _NODE_VALUE_STR(node));

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)));

    emitCallFuncFreeArgRegsNasm(node, context);
    _CONTEXT_IS_CALL_FUNC_ARG(context) = 0;
    _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endCallFunc\n");
}

void emitCallFuncFreeArgRegsNasm(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 1);

    regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    if(foundReg){
        REG_TABLE_ELEM_USE_BIT(foundReg) = 0;
        emitCallFuncFreeArgRegsNasm(node, context);
    }

    if(foundReg) fprintf(_CONTEXT_FILE_PTR(context), "pop %s\n", REG_TABLE_ELEM_NAME(foundReg));
    
    regTableElemDtor(refReg);       
}

void emitInitFuncNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    list_t* newVarMap = (list_t*) calloc(1, sizeof(list_t));
    initVarMap(newVarMap, _CONTEXT_REG_TABLE(context));
    _CONTEXT_VAR_MAP(context) = newVarMap;

    _CONTEXT_STACK_OFFSET(context) = VARIABLE_NASM_BYTES_SIZE;

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n%s:\n", _NODE_VALUE_STR(node));
    emitFuncPrologNasm(node, context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;
}

void emitInitFuncNasmIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = NOT_REG_SCEN;
}

void emitInitFuncNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;

    listDtor(_CONTEXT_VAR_MAP(context), varMapElemDtor);
    free(_CONTEXT_VAR_MAP(context));

    freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);
}

void emitFuncPrologNasm(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "push rbp\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rbp, rsp\n");
}

void emitRetNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;
}

void emitRetNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    loadToReg(_CONTEXT_CUR_VAR(context), context);
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    emitFuncEpilog(node, context);
    fprintf(_CONTEXT_FILE_PTR(context), "ret\n");

    _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;
}

void emitFuncEpilog(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);
   

    fprintf(_CONTEXT_FILE_PTR(context), "add rsp, %d\n", _CONTEXT_STACK_OFFSET(context) - VARIABLE_NASM_BYTES_SIZE);

    fprintf(_CONTEXT_FILE_PTR(context), "mov rsp, rbp\n");
    fprintf(_CONTEXT_FILE_PTR(context), "pop rbp\n");
}

void emitIfNasmIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    const char* condJumpInstruction = "";

    switch(_NODE_TYPE(_L(node))){
        case LE:        condJumpInstruction = JG_OPERATION_NAME_NASM ; break; 
        case LT:        condJumpInstruction = JGE_OPERATION_NAME_NASM; break; 
        case GE:        condJumpInstruction = JL_OPERATION_NAME_NASM ; break; 
        case GT:        condJumpInstruction = JLE_OPERATION_NAME_NASM; break; 
        case EQUAL:     condJumpInstruction = JNE_OPERATION_NAME_NASM; break; 
        case NOT_EQUAL: condJumpInstruction = JE_OPERATION_NAME_NASM ; break; 
        default: break;
    }

    label_t* ifLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_IF_END);
    assert(ifLabel);

    _CONTEXT_CUR_LABEL_A(context) = ifLabel;

    fprintf(_CONTEXT_FILE_PTR(context), "%s .%s_%d\n", condJumpInstruction, _LABEL_DATA_NAME(ifLabel), _LABEL_DATA_ID(ifLabel));
}

void emitIfNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d:\n", _LABEL_DATA_NAME(_CONTEXT_CUR_LABEL_A(context)), _LABEL_DATA_ID(_CONTEXT_CUR_LABEL_A(context)));
}

void emitWhileNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    label_t* whileStartLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_WHILE_BEGIN);
    assert(whileStartLabel);

    fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d\n", _LABEL_DATA_NAME(whileStartLabel), _LABEL_DATA_ID(whileStartLabel));

    _CONTEXT_CUR_LABEL_A(context) = whileStartLabel;
}

void emitWhileNasmIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    const char* condJumpInstruction = "";

    switch(_NODE_TYPE(_L(node))){
        case LE:        condJumpInstruction = JG_OPERATION_NAME_NASM ; break; 
        case LT:        condJumpInstruction = JGE_OPERATION_NAME_NASM; break; 
        case GE:        condJumpInstruction = JL_OPERATION_NAME_NASM ; break; 
        case GT:        condJumpInstruction = JLE_OPERATION_NAME_NASM; break; 
        case EQUAL:     condJumpInstruction = JNE_OPERATION_NAME_NASM; break; 
        case NOT_EQUAL: condJumpInstruction = JE_OPERATION_NAME_NASM ; break; 
        default: break;
    }

    label_t* whileEndLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_WHILE_END);
    assert(whileEndLabel);

    fprintf(_CONTEXT_FILE_PTR(context), "%s .%s_%d\n", condJumpInstruction, _LABEL_DATA_NAME(whileEndLabel), _LABEL_DATA_ID(whileEndLabel));

    _CONTEXT_CUR_LABEL_B(context) = whileEndLabel;
}

void emitWhileNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "jmp .%s_%d:\n", _LABEL_DATA_NAME(_CONTEXT_CUR_LABEL_A(context)), _LABEL_DATA_ID(_CONTEXT_CUR_LABEL_A(context)));
    fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d:\n", _LABEL_DATA_NAME(_CONTEXT_CUR_LABEL_B(context)), _LABEL_DATA_ID(_CONTEXT_CUR_LABEL_B(context)));
}

void emitCmpNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpNasmPost(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", CMP_OPERATION_NAME_NASM, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
}

void emitAssignNasmIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_IS_L_VALUE(context) = 1;
}

void emitAssignNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitAssign start");

    _CONTEXT_IS_L_VALUE(context) = 0;

    varMapElem_t* foundVar = _CONTEXT_CUR_VAR(context);

    if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_REG){
        fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG((foundVar))), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
    }
    else if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_STACK){
        fprintf(_CONTEXT_FILE_PTR(context), "sub rsp, %d\n", VARIABLE_NASM_BYTES_SIZE);       
        fprintf(_CONTEXT_FILE_PTR(context), "mov [rbp - %d], %s\n", VARIABLE_MAP_LOC_STACK_OFFSET((foundVar)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
    }

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endAssign\n");

    LPRINTF("emitAssign end");
}

void emitBinaryOpNasmIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "push %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitBinaryOpNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "pop %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitAddNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpNasmPost(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", ADD_OPERATION_NAME_NASM, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitSubNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpNasmPost(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", SUB_OPERATION_NAME_NASM, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitMulNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpNasmPost(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", MUL_OPERATION_NAME_NASM, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitDivNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpNasmPost(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "push rax\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push rdx\n");

    fprintf(_CONTEXT_FILE_PTR(context), "xor rdx, rdx\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov rax, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s\n", DIV_OPERATION_NAME_NASM, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, rax\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "pop rdx\n");
    fprintf(_CONTEXT_FILE_PTR(context), "pop rax\n");
}

void emitSqrtNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    loadToReg(_CONTEXT_CUR_VAR(context), context);
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "cvtsi2ss xmm0, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "%s xmm0, xmm0\n", SQRT_OPERATION_NAME_NASM);
    fprintf(_CONTEXT_FILE_PTR(context), "cvttss2si %s, xmm0\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitVarNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitVar start");
    
    LPRINTF("amountVariables: %llu\n", _CONTEXT_VAR_MAP(context)->size);

    int curVarCode = 0;
    sscanf(_NODE_WRITE_FILE(node), "VAR%d", &curVarCode);

    LPRINTF("curVarCode: %d\n", curVarCode);

    varMapElem_t* refElem = varMapElemCtor(curVarCode, LOCK_ANY);
    assert(refElem);

    varMapElem_t* foundElem = varMapFind(_CONTEXT_VAR_MAP(context), varMapCmp, refElem);

    if(!foundElem){
        foundElem = varMapAddVar(_CONTEXT_VAR_MAP(context), _CONTEXT_REG_TABLE(context), curVarCode, _CONTEXT_STACK_OFFSET(context), _CONTEXT_VAR_REG_USE_SCENERY(context));

        if(VARIABLE_MAP_LOC_TYPE(foundElem) == LOCK_STACK){
            _CONTEXT_STACK_OFFSET(context) += VARIABLE_NASM_BYTES_SIZE;
        }
    }

    varMapElemDtor(refElem);

    if(_CONTEXT_IS_CALL_FUNC_ARG(context)){
        loadToReg(foundElem, context);
        regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 0);

        regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);

        regTableElemDtor(refReg);

        fprintf(_CONTEXT_FILE_PTR(context), "push %s\n", REG_TABLE_ELEM_NAME(foundReg), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
        fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(foundReg), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

        REG_TABLE_ELEM_USE_BIT(foundReg) = 1;
    }

    if(_CONTEXT_SAVE_CUR_VAR(context)) _CONTEXT_CUR_VAR(context) = foundElem; 


    LPRINTF("_CONTEXT_IS_L_VALUE = %d, _CONTEXT_IS_CALL_FUNC_ARG = %d, _CONTEXT_VAR_REG_USE_SCENERY  = %d", _CONTEXT_IS_L_VALUE(context), _CONTEXT_IS_CALL_FUNC_ARG(context), _CONTEXT_VAR_REG_USE_SCENERY(context) );
    if(!_CONTEXT_IS_L_VALUE(context) && 
    !_CONTEXT_IS_CALL_FUNC_ARG(context) && 
    !(_CONTEXT_VAR_REG_USE_SCENERY(context) == FUNC_ARGS)){
        loadToReg(foundElem, context);
    } 

    LPRINTF("emitVar end");
}

void emitNumberNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitNumber start");

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %d\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), _NODE_VALUE_NUM(node));

    LPRINTF("emitNumber end");
}

void emitHltNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "\n; sys_exit(0)\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rax, 60\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rdi, 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "syscall\n");
}

void emitInNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_IS_L_VALUE(context) = 1;
}

void emitInNasmIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_IS_L_VALUE(context) = 0;

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, 0\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "push rdi\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push rsi\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push rdx\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push rax\n");

    label_t* readLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_READ);
    assert(readLabel);

    fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d:\n", _LABEL_DATA_NAME(readLabel), _LABEL_DATA_ID(readLabel));   

    fprintf(_CONTEXT_FILE_PTR(context), "push 0\n");

    fprintf(_CONTEXT_FILE_PTR(context), "mov rax, 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rdi, 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rsi, rsp\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rdx, 1\n");
    fprintf(_CONTEXT_FILE_PTR(context), "syscall\n");

    label_t* readLabelEnd = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_READ_END);
    assert(readLabelEnd);

    fprintf(_CONTEXT_FILE_PTR(context), "test rax, rax\n");
    fprintf(_CONTEXT_FILE_PTR(context), "jle .%s_%d\n", _LABEL_DATA_NAME(readLabelEnd), _LABEL_DATA_ID(readLabelEnd));

    fprintf(_CONTEXT_FILE_PTR(context), "pop rcx\n");

    fprintf(_CONTEXT_FILE_PTR(context), "cmp cl, 10\n");
    fprintf(_CONTEXT_FILE_PTR(context), "je .%s_%d\n", _LABEL_DATA_NAME(readLabelEnd), _LABEL_DATA_ID(readLabelEnd));

    fprintf(_CONTEXT_FILE_PTR(context), "sub cl, '0'\n");
    fprintf(_CONTEXT_FILE_PTR(context), "imul rbx, 10\n");
    fprintf(_CONTEXT_FILE_PTR(context), "add rbx, rcx\n");

    fprintf(_CONTEXT_FILE_PTR(context), "jmp .%s_%d\n", _LABEL_DATA_NAME(readLabel), _LABEL_DATA_ID(readLabel));

    fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d:\n", _LABEL_DATA_NAME(readLabelEnd), _LABEL_DATA_ID(readLabelEnd));

    varMapElem_t* foundVar = _CONTEXT_CUR_VAR(context);

    if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_REG){
        fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG((foundVar))), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
    }
    else if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_STACK){
        fprintf(_CONTEXT_FILE_PTR(context), "sub rsp, %d\n", VARIABLE_NASM_BYTES_SIZE);       
        fprintf(_CONTEXT_FILE_PTR(context), "mov [rbp - %d], %s\n", VARIABLE_MAP_LOC_STACK_OFFSET((foundVar)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
    }
}

void emitOutNasmIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "push rdi\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push rsi\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push rdx\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push rax\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push rcx\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push r12\n");

    
    fprintf(_CONTEXT_FILE_PTR(context), "sub rsp, 1\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov byte [rsp], 10\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov r12, 1\n");
    
    fprintf(_CONTEXT_FILE_PTR(context), "mov rax, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov rcx, 10\n");

    label_t* writeLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_WRITE);
    assert(writeLabel);

    fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d:\n", _LABEL_DATA_NAME(writeLabel), _LABEL_DATA_ID(writeLabel));   

    fprintf(_CONTEXT_FILE_PTR(context), "mov rdx, 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "div rcx\n");

    fprintf(_CONTEXT_FILE_PTR(context), "add rdx, '0'\n");

    fprintf(_CONTEXT_FILE_PTR(context), "sub rsp, 1\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov [rsp], dl\n");
    fprintf(_CONTEXT_FILE_PTR(context), "add r12, 1\n");


    fprintf(_CONTEXT_FILE_PTR(context), "test rax, rax\n");
    fprintf(_CONTEXT_FILE_PTR(context), "jnz .%s_%d\n", _LABEL_DATA_NAME(writeLabel), _LABEL_DATA_ID(writeLabel));

    fprintf(_CONTEXT_FILE_PTR(context), "mov rax, 1\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rdi, 1\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rsi, rsp\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rdx, r12\n");
    fprintf(_CONTEXT_FILE_PTR(context), "syscall\n");
    
    fprintf(_CONTEXT_FILE_PTR(context), "add rsp, r12\n");

    fprintf(_CONTEXT_FILE_PTR(context), "pop r12\n");
    fprintf(_CONTEXT_FILE_PTR(context), "pop rcx\n");
    fprintf(_CONTEXT_FILE_PTR(context), "pop rax\n");
    fprintf(_CONTEXT_FILE_PTR(context), "pop rdx\n");
    fprintf(_CONTEXT_FILE_PTR(context), "pop rsi\n");
    fprintf(_CONTEXT_FILE_PTR(context), "pop rdi\n");

}

void loadToReg(varMapElem_t* var, codeGenContext* context){
    if (VARIABLE_MAP_LOC_TYPE(var) == LOCK_REG){
        if(REG_TABLE_ELEM_USE_SCENERY(VARIABLE_MAP_LOC_REG(var)) != TEMP_STORE) fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG(var)));
    }
    else{
        fprintf(_CONTEXT_FILE_PTR(context), "mov %s, [rbp - %d]\n", 
        REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), VARIABLE_MAP_LOC_STACK_OFFSET(var));
    }
}

