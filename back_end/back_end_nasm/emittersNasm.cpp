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

// void emitEb(treeNode_t* node, codeGenContext* context);
// void emitBlock(treeNode_t* node, codeGenContext* context);
// void emitMain(treeNode_t* node, codeGenContext* context);

// void emitFunc(treeNode_t* node, codeGenContext* context);
// void emitInitFunc(treeNode_t* node, codeGenContext* context);
// void emitFuncProlog(treeNode_t* node, codeGenContext* context);
// void emitFuncEpilog(treeNode_t* node, codeGenContext* context);
// void emitInitFuncArgs(treeNode_t* node, codeGenContext* context);
// void emitComma(treeNode_t* node, codeGenContext* context);
// void emitRet(treeNode_t* node, codeGenContext* context);

// void emitCallFunc(treeNode_t* node, codeGenContext* context);
// void emitCallFuncArg(treeNode_t* node, codeGenContext* context);
// void emitCallFuncFreeArgRegs(treeNode_t* node, codeGenContext* context);

// void emitIf(treeNode_t* node, codeGenContext* context);
// void emitWhile(treeNode_t* node, codeGenContext* context);
// void emitCondition(treeNode_t* node, codeGenContext* context);

// void emitEs(treeNode_t* node, codeGenContext* context);
// void emitStatement(treeNode_t* node, codeGenContext* context);
// void emitAssign(treeNode_t* node, codeGenContext* context);

// void emitExpression(treeNode_t* node, codeGenContext* context);
// void emitSub(treeNode_t* node, codeGenContext* context);
// void emitAdd(treeNode_t* node, codeGenContext* context);
// void emitMul(treeNode_t* node, codeGenContext* context);
// void emitDiv(treeNode_t* node, codeGenContext* context);


// void emitSqrt(treeNode_t* node, codeGenContext* context);

// void emitNumber(treeNode_t* node, codeGenContext* context);
// void emitVar(treeNode_t* node, codeGenContext* context);

// void emitUnaryOpPreamble(treeNode_t* node, codeGenContext* context);
// void emitBinaryOpPreamble(treeNode_t* node, codeGenContext* context);

// void emitHlt(treeNode_t* node, codeGenContext* context);

// inline void emitCurNode(treeNode_t* node, codeGenContext* context);

// void emitPlug(treeNode_t* node, codeGenContext* context);

void loadToReg(varMapElem_t* var, codeGenContext* context);

    // {IN,            emitPlug     },
    // {OUT,           emitPlug     },
    // {POPM,          emitPopm  },
    // {DRAW,          emitDraw  }

const char* MAIN_START_NAME    = "_start";

const char* ADD_OPERATION_NAME = "add";
const char* SUB_OPERATION_NAME = "sub" ;
const char* MUL_OPERATION_NAME = "imul";
const char* DIV_OPERATION_NAME = "idiv";

const char* SQRT_OPERATION_NAME = "sqrtss";

const char* CMP_OPERATION_NAME = "cmp";
const char* JG_OPERATION_NAME  = "jg";
const char* JGE_OPERATION_NAME = "jge";
const char* JL_OPERATION_NAME  = "jl";
const char* JLE_OPERATION_NAME = "jle";
const char* JE_OPERATION_NAME  = "je";
const char* JNE_OPERATION_NAME = "jne";

// const size_t EMIT_TABLE_SIZE = sizeof(emittersTable) / sizeof(emitRule);

// emitter_t getEmitter(ASTnodeType type){
// for(size_t curEmitRuleInd = 0; curEmitRuleInd < EMIT_TABLE_SIZE; curEmitRuleInd++){
//         if(type == _EMIT_RULE_TYPE(&emittersTable[curEmitRuleInd])){
//             return _EMIT_RULE_EMITTER(&emittersTable[curEmitRuleInd]);
//         }
//     }

//     return NULL;
// }   

// void emitStartNasm(treeNode_t* node, codeGenContext* context){ 
//     assert(node);
//     assert(context);

//     LPRINTF("emitStart start");

//     genPreamble(context);

//     emitCurNode(node, context);

//     genEpilogue(context);

//     LPRINTF("emitStart end");
// }

// void emitEb(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     LPRINTF("emitEb start");

//     if(_L(node)){
//         emitBlock(_L(node), context);
//     }

//     if(_R(node)){
//         emitBlock(_R(node), context);
//     }

//     LPRINTF("emitEb end");
// }

// void emitBlock(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     LPRINTF("emitBlock start");

//     _CONTEXT_BLOCK_IM_DEPTH(context)++;

//     emitCurNode(node, context);

//     _CONTEXT_BLOCK_IM_DEPTH(context)--;

//     LPRINTF("emitBlock end");
// }

void emitMainNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMain start");

    fprintf(_CONTEXT_FILE_PTR(context), "%s:\n", MAIN_START_NAME);

    LPRINTF("emitMain end");
}

// void emitMain(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     LPRINTF("emitMain start");

//     fprintf(_CONTEXT_FILE_PTR(context), "%s:\n", MAIN_START_NAME);

//     if(_R(node)){
//         emitBlock(_R(node), context);
//     }

//     emitHlt(node, context);

//     LPRINTF("emitMain end");
// }

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

// void emitCallFunc(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     fprintf(_CONTEXT_FILE_PTR(context), "\n;startCallFunc\n");

//     freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);

//     emitCallFuncArg(node, context);

//     fprintf(_CONTEXT_FILE_PTR(context), "call %s\n", _NODE_VALUE_STR(node));

//     fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)));

//     emitCallFuncFreeArgRegs(node, context);

//     fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endCallFunc\n");
// }


// void emitCallFuncArg(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     if(_L(node)){
//         emitCallFuncArg(_L(node), context);
//     }

//     if(_R(node)){
//         emitCallFuncArg(_R(node), context);
//     }   

//     if((_NODE_TYPE(node) != COMMA) && (_NODE_TYPE(node) != FUNCTION)){
//         emitCurNode(node, context);
//         loadToReg(_CONTEXT_CUR_VAR(context), context);
//         regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 0);

//         regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);

//         regTableElemDtor(refReg);

//         fprintf(_CONTEXT_FILE_PTR(context), "push %s\n", REG_TABLE_ELEM_NAME(foundReg), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
//         fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(foundReg), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

//         REG_TABLE_ELEM_USE_BIT(foundReg) = 1;
//     }
// }

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

// void emitInitFunc(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     list_t newVarMap;
//     initVarMap(&newVarMap, _CONTEXT_REG_TABLE(context));
//     _CONTEXT_VAR_MAP(context) = &newVarMap;

//     _CONTEXT_STACK_OFFSET(context) = VARIABLE_NASM_BYTES_SIZE;

//     fprintf(_CONTEXT_FILE_PTR(context), "\n\n%s:\n", _NODE_VALUE_STR(node));
//     emitFuncProlog(node, context);

//     if(_L(node)){
//         emitInitFuncArgs(_L(node), context);
//     }

//     _CONTEXT_VAR_REG_USE_SCENERY(context) = NOT_REG_SCEN;
//     if(_R(node)){
//         emitBlock(_R(node), context);
//     }
//     _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;

//     emitRet(node, context);

//     listDtor(&newVarMap, varMapElemDtor);

//     freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);
// }

// void emitInitFuncArgs(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;

//     emitCurNode(node, context);

//     _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;  
// }

// void emitComma(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     if(_L(node)){
//         emitCurNode(_L(node), context);
//     }

//     if(_R(node)){
//         emitCurNode(_R(node), context);
//     }
// }

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

// void emitRet(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;

//     emitVar(_L(node), context);
//     loadToReg(_CONTEXT_CUR_VAR(context), context);
//     fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

//     emitFuncEpilog(node, context);
//     fprintf(_CONTEXT_FILE_PTR(context), "ret\n");

//     _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;
// }

void emitIfNasmIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    const char* condJumpInstruction = "";

    switch(_NODE_TYPE(_L(node))){
        case LE:        condJumpInstruction = JG_OPERATION_NAME ; break; 
        case LT:        condJumpInstruction = JGE_OPERATION_NAME; break; 
        case GE:        condJumpInstruction = JL_OPERATION_NAME ; break; 
        case GT:        condJumpInstruction = JLE_OPERATION_NAME; break; 
        case EQUAL:     condJumpInstruction = JNE_OPERATION_NAME; break; 
        case NOT_EQUAL: condJumpInstruction = JE_OPERATION_NAME ; break; 
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
        case LE:        condJumpInstruction = JG_OPERATION_NAME ; break; 
        case LT:        condJumpInstruction = JGE_OPERATION_NAME; break; 
        case GE:        condJumpInstruction = JL_OPERATION_NAME ; break; 
        case GT:        condJumpInstruction = JLE_OPERATION_NAME; break; 
        case EQUAL:     condJumpInstruction = JNE_OPERATION_NAME; break; 
        case NOT_EQUAL: condJumpInstruction = JE_OPERATION_NAME ; break; 
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

// void emitCondition(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     LPRINTF("emitCondition start");

//     emitBinaryOpPreamble(node, context);

//     fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", CMP_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));

//     LPRINTF("emitCondition end");
// }

void emitCmpNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpNasmPost(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", CMP_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
}

// void emitEs(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     LPRINTF("emitEs start");

//     if(_L(node)){
//         emitStatement(_L(node), context);
//     }

//     if(_R(node)){
//         if(_NODE_TYPE(_R(node)) != END_STATEMENT) emitStatement(_R(node), context);
//         else emitEs(_R(node), context);
//     }

//     LPRINTF("emitEs end");
// }

// void emitStatement(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     LPRINTF("emitStatement start");

//     // emitTabs(context);

//     emitCurNode(node, context);

//     LPRINTF("emitStatement end");
// }

void emitAssignNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    // LPRINTF("emitAssign start");

    // fprintf(_CONTEXT_FILE_PTR(context), "\n;startAssign\n");
    
    

    // LPRINTF("emitAssign end");
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

// void emitAssign(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     LPRINTF("emitAssign start");

//     fprintf(_CONTEXT_FILE_PTR(context), "\n;startAssign\n");
    
//     varMapElem_t* foundVar = NULL;

//     if(_L(node)){
//         _CONTEXT_IS_L_VALUE(context) = 1;
//         emitVar(_L(node), context);
//         _CONTEXT_IS_L_VALUE(context) = 0;
//     }

//     _CONTEXT_SAVE_CUR_VAR(context) = 0;

//     if(_R(node)){
//         emitExpression(_R(node), context);
//         if(_NODE_TYPE(_R(node)) == VARIABLE) loadToReg(_CONTEXT_CUR_VAR(context), context);
//     }

//     foundVar = _CONTEXT_CUR_VAR(context);

//     if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_REG){
//         fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG((foundVar))), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
//     }
//     else if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_STACK){
//         fprintf(_CONTEXT_FILE_PTR(context), "sub rsp, %d\n", VARIABLE_NASM_BYTES_SIZE);       
//         fprintf(_CONTEXT_FILE_PTR(context), "mov [rbp - %d], %s\n", VARIABLE_MAP_LOC_STACK_OFFSET((foundVar)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
//     }

//     fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endAssign\n");

//     LPRINTF("emitAssign end");
// }

// void emitExpression(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     LPRINTF("emitExpression start");

//     emitCurNode(node, context);

//     LPRINTF("emitExpression end");    
// }

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

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", ADD_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitSubNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpNasmPost(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", SUB_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitMulNasmPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitBinaryOpNasmPost(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", MUL_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
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

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s\n", DIV_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));

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
    fprintf(_CONTEXT_FILE_PTR(context), "%s xmm0, xmm0\n", SQRT_OPERATION_NAME);
    fprintf(_CONTEXT_FILE_PTR(context), "cvttss2si %s, xmm0\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
}

void emitVarNasmPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitVar start");
    
    printf("amountVariables: %llu\n", _CONTEXT_VAR_MAP(context)->size);

    int curVarCode = 0;
    sscanf(_NODE_WRITE_FILE(node), "VAR%d", &curVarCode);

    printf("curVarCode: %d\n", curVarCode);

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


    printf("_CONTEXT_IS_L_VALUE = %d, _CONTEXT_IS_CALL_FUNC_ARG = %d, _CONTEXT_VAR_REG_USE_SCENERY  = %d", _CONTEXT_IS_L_VALUE(context), _CONTEXT_IS_CALL_FUNC_ARG(context), _CONTEXT_VAR_REG_USE_SCENERY(context) );
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

// void emitPlug(treeNode_t* node, codeGenContext* context){
//     LPRINTF("plug");
// }

// inline void emitCurNode(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     emitter_t curEmitter = getEmitter(_NODE_TYPE(node));    
//     if(curEmitter){
//         curEmitter(node, context);
//     }
// }

// void emitBinaryOpPreamble(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     if(_L(node)){
//         emitExpression(_L(node), context);
//     }
    
//     if(_NODE_TYPE(_L(node)) == VARIABLE) loadToReg(_CONTEXT_CUR_VAR(context), context);
//     fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
//     fprintf(_CONTEXT_FILE_PTR(context), "push %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

//     if(_R(node)){
//         emitExpression(_R(node), context);
//     }

//     if(_NODE_TYPE(_R(node)) == VARIABLE) loadToReg(_CONTEXT_CUR_VAR(context), context);
//     fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
//     fprintf(_CONTEXT_FILE_PTR(context), "pop %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
// }

// void emitUnaryOpPreamble(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     if(_R(node)){
//         emitExpression(_R(node), context);
//     }

//     loadToReg(_CONTEXT_CUR_VAR(context), context);
//     fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
// }

void loadToReg(varMapElem_t* var, codeGenContext* context){
    if (VARIABLE_MAP_LOC_TYPE(var) == LOCK_REG){
        if(REG_TABLE_ELEM_USE_SCENERY(VARIABLE_MAP_LOC_REG(var)) != TEMP_STORE) fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG(var)));
    }
    else{
        fprintf(_CONTEXT_FILE_PTR(context), "mov %s, [rbp - %d]\n", 
        REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), VARIABLE_MAP_LOC_STACK_OFFSET(var));
    }
}
