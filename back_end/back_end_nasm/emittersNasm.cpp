#include "emittersNasm.h"
#include "format.h"

#include "core/core.h"
#include "core/DSL.h"

#define DEBUG

#include "general/debug.h"

#include <assert.h>

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
    char*       name;
};

varMapElem_t* emitEb(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitBlock(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitMain(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitFunc(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitInitFunc(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitFuncProlog(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitFuncEpilog(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitInitFuncArgs(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitComma(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitRet(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitCallFunc(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitCallFuncArg(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitCallFuncFreeArgRegs(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitIf(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitWhile(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitCondition(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitEs(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitStatement(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitAssign(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitExpression(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitMul(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitAdd(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitSub(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitDiv(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitSqrt(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitNumber(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitVar(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitUnaryOpPreamble(treeNode_t* node, codeGenContext* context);
varMapElem_t* emitBinaryOpPreamble(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitHlt(treeNode_t* node, codeGenContext* context);

inline varMapElem_t* emitCurNode(treeNode_t* node, codeGenContext* context);

varMapElem_t* emitPlug(treeNode_t* node, codeGenContext* context);

regTableElem_t* loadToReg(varMapElem_t* var, codeGenContext* context);

static emitRule emittersTable[] = {
    {END_BLOCK,     emitEb       },
    {MAIN,          emitMain     },
    {FUNCTION,      emitFunc     },
    {RETURN,        emitRet      },
    {COMMA,         emitComma    },
    {IF,            emitIf       },
    {WHILE,         emitWhile    },
    {END_STATEMENT, emitEs       },
    {ASSIGN,        emitAssign   },
    {SUB,           emitSub      },
    {MUL,           emitMul      },
    {ADD,           emitAdd      },
    {DIVIDE,        emitDiv      },
    {HLT,           emitHlt      },
    {VARIABLE,      emitVar      },
    {NUMBER,        emitNumber   },
    {SQRT,          emitSqrt     },
    // {GT,            emitGt    },           
    // {LT,            emitLt    },
    // {GE,            emitGe    },
    // {LE,            emitLe    },
    // {EQUAL,         emitEqual },
    // {NOT_EQUAL,     emitNEqual},
    {IN,            emitPlug     },
    {OUT,           emitPlug     },
    // {POPM,          emitPopm  },
    // {DRAW,          emitDraw  }
};

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

const size_t EMIT_TABLE_SIZE = sizeof(emittersTable) / sizeof(emitRule);

emitter_t getEmitter(ASTnodeType type){
for(size_t curEmitRuleInd = 0; curEmitRuleInd < EMIT_TABLE_SIZE; curEmitRuleInd++){
        if(type == _EMIT_RULE_TYPE(&emittersTable[curEmitRuleInd])){
            return _EMIT_RULE_EMITTER(&emittersTable[curEmitRuleInd]);
        }
    }

    return NULL;
}   

varMapElem_t* emitNode(treeNode_t* node, codeGenContext* context){ 
    assert(node);
    assert(context);

    LPRINTF("emitNode start");

    emitCurNode(node, context);

    LPRINTF("emitNode end");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitEb(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitEb start");

    if(_L(node)){
        emitBlock(_L(node), context);
    }

    if(_R(node)){
        emitBlock(_R(node), context);
    }

    LPRINTF("emitEb end");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitBlock(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitBlock start");

    _CONTEXT_BLOCK_IM_DEPTH(context)++;

    emitCurNode(node, context);

    _CONTEXT_BLOCK_IM_DEPTH(context)--;

    LPRINTF("emitBlock end");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitMain(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMain start");

    fprintf(_CONTEXT_FILE_PTR(context), "%s:\n", MAIN_START_NAME);

    if(_R(node)){
        emitBlock(_R(node), context);
    }

    emitHlt(node, context);

    LPRINTF("emitMain end");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFunc(node, context);
    }
    else{
        emitCallFunc(node, context);
    }

    LPRINTF("emitFunc end");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitCallFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startCallFunc\n");

    freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);

    emitCallFuncArg(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "call %s\n", _NODE_VALUE_STR(node));

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)));

    emitCallFuncFreeArgRegs(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endCallFunc\n");
    
    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitCallFuncFreeArgRegs(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 1);

    regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
    if(foundReg){
        REG_TABLE_ELEM_USE_BIT(foundReg) = 0;
        emitCallFuncFreeArgRegs(node, context);
    }

    if(foundReg) fprintf(_CONTEXT_FILE_PTR(context), "pop %s\n", REG_TABLE_ELEM_NAME(foundReg));
    
    regTableElemDtor(refReg);       

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitCallFuncArg(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    if(_L(node)){
        emitCallFuncArg(_L(node), context);
    }

    if(_R(node)){
        emitCallFuncArg(_R(node), context);
    }   

    if((_NODE_TYPE(node) != COMMA) && (_NODE_TYPE(node) != FUNCTION)){
        loadToReg(emitCurNode(node, context), context);
        regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 0);

        regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);

        regTableElemDtor(refReg);

        fprintf(_CONTEXT_FILE_PTR(context), "push %s\n", REG_TABLE_ELEM_NAME(foundReg), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
        fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(foundReg), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

        REG_TABLE_ELEM_USE_BIT(foundReg) = 1;
    }

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitInitFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    list_t newVarMap;
    initVarMap(&newVarMap, _CONTEXT_REG_TABLE(context));
    _CONTEXT_VAR_MAP(context) = &newVarMap;

    _CONTEXT_STACK_OFFSET(context) = VARIABLE_BYTES_SIZE;

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n%s:\n", _NODE_VALUE_STR(node));
    emitFuncProlog(node, context);

    if(_L(node)){
        emitInitFuncArgs(_L(node), context);
    }

    _CONTEXT_VAR_REG_USE_SCENERY(context) = NOT_REG_SCEN;
    if(_R(node)){
        emitBlock(_R(node), context);
    }
    _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;

    emitRet(node, context);

    listDtor(&newVarMap, varMapElemDtor);

    freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitInitFuncArgs(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;

    emitCurNode(node, context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;  

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitComma(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    if(_L(node)){
        emitCurNode(_L(node), context);
    }

    if(_R(node)){
        emitCurNode(_R(node), context);
    }

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitFuncProlog(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "push rbp\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rbp, rsp\n");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitRet(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_VAR_REG_USE_SCENERY(context) = FUNC_ARGS;

    varMapElem_t* retVal = emitVar(_L(node), context);
    loadToReg(retVal, context);
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    emitFuncEpilog(node, context);
    fprintf(_CONTEXT_FILE_PTR(context), "ret\n");

    _CONTEXT_VAR_REG_USE_SCENERY(context) = STORE_VAR;

    return _CONTEXT_TEMP_VAR(context);
}


varMapElem_t* emitFuncEpilog(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);
   

    fprintf(_CONTEXT_FILE_PTR(context), "add rsp, %d\n", _CONTEXT_STACK_OFFSET(context) - VARIABLE_BYTES_SIZE);

    fprintf(_CONTEXT_FILE_PTR(context), "mov rsp, rbp\n");
    fprintf(_CONTEXT_FILE_PTR(context), "pop rbp\n");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitIf(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    if(_L(node)){
        emitCondition(_L(node), context);
    }   

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

    fprintf(_CONTEXT_FILE_PTR(context), "%s .%s_%d\n", condJumpInstruction, _LABEL_DATA_NAME(ifLabel), _LABEL_DATA_ID(ifLabel));

    if(_R(node)){
        emitBlock(_R(node), context);
    }   

    fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d:\n", _LABEL_DATA_NAME(ifLabel), _LABEL_DATA_ID(ifLabel));

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitWhile(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    label_t* whileStartLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_WHILE_BEGIN);
    assert(whileStartLabel);

    fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d\n", _LABEL_DATA_NAME(whileStartLabel), _LABEL_DATA_ID(whileStartLabel));

    if(_L(node)){
        emitCondition(_L(node), context);
    }   

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

    if(_R(node)){
        emitBlock(_R(node), context);
    }   

    fprintf(_CONTEXT_FILE_PTR(context), "jmp .%s_%d:\n", _LABEL_DATA_NAME(whileStartLabel), _LABEL_DATA_ID(whileStartLabel));
    fprintf(_CONTEXT_FILE_PTR(context), ".%s_%d:\n", _LABEL_DATA_NAME(whileEndLabel), _LABEL_DATA_ID(whileEndLabel));

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitCondition(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitCondition start");

    emitBinaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", CMP_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));

    LPRINTF("emitCondition end");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitEs(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitEs start");

    if(_L(node)){
        emitStatement(_L(node), context);
    }

    if(_R(node)){
        if(_NODE_TYPE(_R(node)) != END_STATEMENT) emitStatement(_R(node), context);
        else emitEs(_R(node), context);
    }

    LPRINTF("emitEs end");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitStatement(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitStatement start");

    // emitTabs(context);

    emitCurNode(node, context);

    LPRINTF("emitStatement end");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitAssign(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitAssign start");
    
    varMapElem_t* foundVar = NULL;
    if(_L(node)){
        foundVar = emitVar(_L(node), context);
    }

    if(_R(node)){
        emitExpression(_R(node), context);
    }

    if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_REG){
        fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG((foundVar))), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
    }
    else if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_STACK){
        fprintf(_CONTEXT_FILE_PTR(context), "sub rsp, %d\n", VARIABLE_BYTES_SIZE);       
        fprintf(_CONTEXT_FILE_PTR(context), "mov [rbp - %d], %s\n", VARIABLE_MAP_LOC_STACK_OFFSET((foundVar)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
    }


    LPRINTF("emitAssign end");

    return foundVar;
}

varMapElem_t* emitExpression(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitExpression start");

    varMapElem_t* retVal = emitCurNode(node, context);

    LPRINTF("emitExpression end");    

    return retVal;
}

varMapElem_t* emitAdd(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitAdd start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startAdd\n");

    emitBinaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", ADD_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endAdd\n");

    LPRINTF("emitAdd end");    

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitSub(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitSub start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startSub\n");

    emitBinaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", SUB_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endSub\n");

    LPRINTF("emitSub end");    

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitMul(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMul start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startMul\n");

    emitBinaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", MUL_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endMul\n");

    LPRINTF("emitMul end");    

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitDiv(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitDiv start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startDiv\n");

    emitBinaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "push rax\n");
    fprintf(_CONTEXT_FILE_PTR(context), "push rdx\n");

    fprintf(_CONTEXT_FILE_PTR(context), "xor rdx, rdx\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov rax, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s\n", DIV_OPERATION_NAME, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, rax\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "pop rdx\n");
    fprintf(_CONTEXT_FILE_PTR(context), "pop rax\n");

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endDiv\n");

    LPRINTF("emitDiv end");    

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitSqrt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitSqrt start");    

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startSqrt\n");

    emitUnaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "cvtsi2ss xmm0, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "%s xmm0, xmm0\n", SQRT_OPERATION_NAME);
    fprintf(_CONTEXT_FILE_PTR(context), "cvttss2si %s, xmm0\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endSqrt\n");

    LPRINTF("emitSqrt end");    

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitVar(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitVar start");
    
    printf("amountVariables: %llu\n", _CONTEXT_VAR_MAP(context)->size);

    int curVarCode = 0;
    sscanf(_NODE_WRITE_FILE(node), "VAR%d", &curVarCode);

    varMapElem_t* refElem = varMapElemCtor(curVarCode, LOCK_ANY);
    assert(refElem);

    varMapElem_t* foundElem = varMapFind(_CONTEXT_VAR_MAP(context), varMapCmp, refElem);

    if(!foundElem){
        foundElem = varMapAddVar(_CONTEXT_VAR_MAP(context), _CONTEXT_REG_TABLE(context), curVarCode, _CONTEXT_STACK_OFFSET(context), _CONTEXT_VAR_REG_USE_SCENERY(context));

        if(VARIABLE_MAP_LOC_TYPE(foundElem) == LOCK_STACK){
            _CONTEXT_STACK_OFFSET(context) += VARIABLE_BYTES_SIZE;
        }
    }

    varMapElemDtor(refElem);

    LPRINTF("emitVar end");

    return foundElem;
}

varMapElem_t* emitNumber(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitNumber start");

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %d\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), _NODE_VALUE_NUM(node));

    LPRINTF("emitNumber end");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitHlt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "\n; sys_exit(0)\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rax, 60\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rdi, 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "syscall\n");

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitPlug(treeNode_t* node, codeGenContext* context){
    LPRINTF("plug");
    return _CONTEXT_TEMP_VAR(context);
}

inline varMapElem_t* emitCurNode(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    varMapElem_t* retVal = NULL;

    emitter_t curEmitter = getEmitter(_NODE_TYPE(node));    
    if(curEmitter){
        retVal = curEmitter(node, context);
    }

    return retVal;
}

varMapElem_t* emitBinaryOpPreamble(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    varMapElem_t* retVal = NULL;
    if(_L(node)){
        retVal = emitExpression(_L(node), context);
    }

    loadToReg(retVal, context);
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "push %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    if(_R(node)){
        retVal = emitExpression(_R(node), context);
    }

    loadToReg(retVal, context);
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "pop %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    return _CONTEXT_TEMP_VAR(context);
}

varMapElem_t* emitUnaryOpPreamble(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    varMapElem_t* retVal = NULL;
    if(_R(node)){
        retVal = emitExpression(_R(node), context);
    }

    loadToReg(retVal, context);
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    return _CONTEXT_TEMP_VAR(context);
}

regTableElem_t* loadToReg(varMapElem_t* var, codeGenContext* context){
    if (VARIABLE_MAP_LOC_TYPE(var) == LOCK_REG){
        if(REG_TABLE_ELEM_USE_SCENERY(VARIABLE_MAP_LOC_REG(var)) != TEMP_STORE) fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG(var)));
    }
    else{
        fprintf(_CONTEXT_FILE_PTR(context), "mov %s, [rbp - %d]\n", 
        REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), VARIABLE_MAP_LOC_STACK_OFFSET(var));
    }

    return _CONTEXT_TEMP_REG(context);
}