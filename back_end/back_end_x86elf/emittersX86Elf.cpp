#include "emittersX86Elf.h"
// #include "format.h"

#include "emitBinaryCommands.h"

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

void loadToRegX86Elf(varMapElem_t* var, codeGenContext* context);

    // {IN,            emitPlug     },
    // {OUT,           emitPlug     },
    // {POPM,          emitPopm  },
    // {DRAW,          emitDraw  }

const char* ADD_OPERATION_NAME_X86ELF = "add";
const char* SUB_OPERATION_NAME_X86ELF = "sub" ;
const char* MUL_OPERATION_NAME_X86ELF = "imul";
const char* DIV_OPERATION_NAME_X86ELF = "idiv";

const char* SQRT_OPERATION_NAME_X86ELF = "sqrtss";

const char* CMP_OPERATION_NAME_X86ELF = "cmp";
const char* JG_OPERATION_NAME_X86ELF = "jg";
const char* JGE_OPERATION_NAME_X86ELF = "jge";
const char* JL_OPERATION_NAME_X86ELF  = "jl";
const char* JLE_OPERATION_NAME_X86ELF = "jle";
const char* JE_OPERATION_NAME_X86ELF  = "je";
const char* JNE_OPERATION_NAME_X86ELF = "jne";

const size_t NUMBER_MAX_SIZE          = 64;

void emitFuncX86ElfPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFuncX86ElfPre(node, context);
    }
    else{
        emitCallFuncX86ElfPre(node, context);
    }

    LPRINTF("emitFunc end");
}

void emitFuncX86ElfIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFuncX86ElfIn(node, context);
    }

    LPRINTF("emitFunc end");
}

void emitFuncX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFuncX86ElfPost(node, context);
    }
    else{
        emitCallFuncX86ElfPost(node, context);
    }

    LPRINTF("emitFunc end");
}

void emitCallFuncX86ElfPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);
    _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;
    _CONTEXT_IS_CALL_FUNC_ARG(context) = 1;  
}

void emitCallFuncX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CALL(_NODE_VALUE_STR(node));
    _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)));

    emitCallFuncFreeArgRegsX86Elf(node, context);
    _CONTEXT_IS_CALL_FUNC_ARG(context) = 0;
    _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;
}

void emitCallFuncFreeArgRegsX86Elf(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 1);

    regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    if(foundReg){
        REG_TABLE_ELEM_USE_BIT(foundReg) = 0;
        emitCallFuncFreeArgRegsX86Elf(node, context);
    }
    
    if(foundReg) _POP(REG_TABLE_ELEM_NAME(foundReg));
    
    regTableElemDtor(refReg);       
}

void emitInitFuncX86ElfPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    list_t* newVarMap = (list_t*) calloc(1, sizeof(list_t));
    initVarMap(newVarMap, _CONTEXT_REG_TABLE(context));
    _CONTEXT_VAR_MAP(context) = newVarMap;

    _CONTEXT_STACK_OFFSET(context) = VARIABLE_NASM_BYTES_SIZE;

    // fprintf(_CONTEXT_FILE_PTR(context), "\n\n%s:\n", _NODE_VALUE_STR(node));
    emitFuncPrologX86Elf(node, context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;
}

void emitInitFuncX86ElfIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = NOT_REG_SCEN;
}

void emitInitFuncX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;

    listDtor(_CONTEXT_VAR_MAP(context), varMapElemDtor);
    free(_CONTEXT_VAR_MAP(context));

    freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);
}

void emitFuncPrologX86Elf(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _PUSH("rbp");
    _MOV("rbp", "rsp");
}

void emitRetX86ElfPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;
}

void emitRetX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    loadToRegX86Elf(_CONTEXT_CUR_VAR(context), context);
    _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    emitFuncEpilogX86Elf(node, context);
    _RET();
    _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;
}

void emitFuncEpilogX86Elf(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);
   
    char numStr[NUMBER_MAX_SIZE] = ""; 

    sprintf(numStr, "%d", _CONTEXT_STACK_OFFSET(context) - VARIABLE_NASM_BYTES_SIZE);

    _ADD("rsp", numStr);

    _MOV("rsp", "rbp");
    _POP("rbp");
}

void emitIfX86ElfIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    label_t* ifLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_IF_END);
    assert(ifLabel);

    _CONTEXT_CUR_LABEL_A(context) = ifLabel;

    char fullLabelName[NUMBER_MAX_SIZE] = "";

    sprintf(fullLabelName, "%s%d", _LABEL_DATA_NAME(ifLabel), _LABEL_DATA_ID(ifLabel));

    switch(_NODE_TYPE(_L(node))){
        case LE:        _JG(fullLabelName) ; break; 
        case LT:        _JGE(fullLabelName); break; 
        case GE:        _JL(fullLabelName) ; break; 
        case GT:        _JLE(fullLabelName); break; 
        case EQUAL:     _JNE(fullLabelName); break; 
        case NOT_EQUAL: _JE(fullLabelName) ; break; 
        default: break;
    }

}

void emitIfX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    // fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d:\n", _LABEL_DATA_NAME(_CONTEXT_CUR_LABEL_A(context)), _LABEL_DATA_ID(_CONTEXT_CUR_LABEL_A(context)));
}

void emitWhileX86ElfPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    label_t* whileStartLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_WHILE_BEGIN);
    assert(whileStartLabel);

    // fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d\n", _LABEL_DATA_NAME(whileStartLabel), _LABEL_DATA_ID(whileStartLabel));

    _CONTEXT_CUR_LABEL_A(context) = whileStartLabel;
}

void emitWhileX86ElfIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    label_t* whileEndLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_WHILE_END);
    assert(whileEndLabel);

    _CONTEXT_CUR_LABEL_B(context) = whileEndLabel;

    char fullLabelName[NUMBER_MAX_SIZE] = "";

    sprintf(fullLabelName, "%s%d", _LABEL_DATA_NAME(whileEndLabel), _LABEL_DATA_ID(whileEndLabel));

    switch(_NODE_TYPE(_L(node))){
        case LE:        _JG(fullLabelName);  break; 
        case LT:        _JGE(fullLabelName); break; 
        case GE:        _JL(fullLabelName);  break; 
        case GT:        _JLE(fullLabelName); break; 
        case EQUAL:     _JNE(fullLabelName); break; 
        case NOT_EQUAL: _JE(fullLabelName);  break; 
        default: break;
    }
    
}

void emitWhileX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    char fullLabelName[NUMBER_MAX_SIZE] = "";

    sprintf(fullLabelName, "%s%d", _LABEL_DATA_NAME(_CONTEXT_CUR_LABEL_A(context)), _LABEL_DATA_ID(_CONTEXT_CUR_LABEL_A(context)));

    _JMP(fullLabelName);

    // fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d:\n", _LABEL_DATA_NAME(_CONTEXT_CUR_LABEL_B(context)), _LABEL_DATA_ID(_CONTEXT_CUR_LABEL_B(context)));
}

void emitCmpX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpX86ElfPost(node, context);

    _CMP(REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
}

void emitAssignX86ElfIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_IS_L_VALUE(context) = 1;
}

void emitAssignX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitAssign start");

    _CONTEXT_IS_L_VALUE(context) = 0;

    varMapElem_t* foundVar = _CONTEXT_CUR_VAR(context);

    if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_REG){
        _MOV(REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG((foundVar))), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    }
    else if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_STACK){
        char numStr[NUMBER_MAX_SIZE] = ""; 

        sprintf(numStr, "%d", VARIABLE_NASM_BYTES_SIZE);

        _SUB("rsp", numStr);

        sprintf(numStr, "[rbp - %d]", VARIABLE_MAP_LOC_STACK_OFFSET((foundVar)));

        _MOV(numStr, REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    }

    LPRINTF("emitAssign end");
}

void emitBinaryOpX86ElfIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    _PUSH(REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitBinaryOpX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    _POP(REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitAddX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpX86ElfPost(node, context);

    _ADD(REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitSubX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpX86ElfPost(node, context);

    _SUB(REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitMulX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpX86ElfPost(node, context);

    _IMUL(REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitDivX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpX86ElfPost(node, context);

    _PUSH("rax");
    _PUSH("rdx");

    _MOV("rdx", "0");
    _MOV("rax", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    _IDIV(REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));

    _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), "rax");

    _POP("rdx");
    _POP("rax");
}

void emitSqrtX86ElfPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    loadToRegX86Elf(_CONTEXT_CUR_VAR(context), context);
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "cvtsi2ss xmm0, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "%s xmm0, xmm0\n", SQRT_OPERATION_NAME_X86ELF);
    fprintf(_CONTEXT_FILE_PTR(context), "cvttss2si %s, xmm0\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitVarX86ElfPre(treeNode_t* node, codeGenContext* context){
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
        loadToRegX86Elf(foundElem, context);
        regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 0);

        regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);

        regTableElemDtor(refReg);

        _PUSH(REG_TABLE_ELEM_NAME(foundReg));
        _MOV(REG_TABLE_ELEM_NAME(foundReg), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

        REG_TABLE_ELEM_USE_BIT(foundReg) = 1;
    }

    if(_CONTEXT_SAVE_CUR_VAR(context)) _CONTEXT_CUR_VAR(context) = foundElem; 


    LPRINTF("_CONTEXT_IS_L_VALUE = %d, _CONTEXT_IS_CALL_FUNC_ARG = %d, _CONTEXT_VAR_REG_USE_SCENERY  = %d", _CONTEXT_IS_L_VALUE(context), _CONTEXT_IS_CALL_FUNC_ARG(context), _CONTEXT_VAR_REG_USE_SCENERY(context) );
    if(!_CONTEXT_IS_L_VALUE(context) && 
    !_CONTEXT_IS_CALL_FUNC_ARG(context) && 
    !(_CONTEXT_VAR_REG_USE_SCENERY(context) == FUNC_ARGS)){
        loadToRegX86Elf(foundElem, context);
    } 

    LPRINTF("emitVar end");
}

void emitNumberX86ElfPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitNumber start");

    char numStr[NUMBER_MAX_SIZE] = ""; 

    sprintf(numStr, "%d", _NODE_VALUE_NUM(node));

    _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), numStr);

    LPRINTF("emitNumber end");
}

void emitHltX86ElfPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    char numStr[NUMBER_MAX_SIZE] = ""; 

    sprintf(numStr, "%d", 60);
    _MOV("rax", numStr);

    sprintf(numStr, "%d", 0);
    _MOV("rdi", numStr);

    _SYSCALL();
}

void loadToRegX86Elf(varMapElem_t* var, codeGenContext* context){
    if (VARIABLE_MAP_LOC_TYPE(var) == LOCK_REG){
        if(REG_TABLE_ELEM_USE_SCENERY(VARIABLE_MAP_LOC_REG(var)) != TEMP_STORE) _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG(var)));
    }
    else{
        char memAddrStr[NUMBER_MAX_SIZE] = ""; 

        sprintf(memAddrStr, "[rbp - %d]", VARIABLE_MAP_LOC_STACK_OFFSET(var));

        _MOV(REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), memAddrStr);
    }
}
