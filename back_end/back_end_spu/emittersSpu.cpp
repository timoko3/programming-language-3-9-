#include "emittersSpu.h"
// #include "format.h"

#include "core/core.h"
#include "core/DSL.h"

// #define DEBUG

#include "general/debug.h"

#include <assert.h>

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
};

void emitEb(treeNode_t* node, codeGenContext* context);
void emitBlock(treeNode_t* node, codeGenContext* context);
void emitMain(treeNode_t* node, codeGenContext* context);

void emitFunc(treeNode_t* node, codeGenContext* context);
void emitInitFunc(treeNode_t* node, codeGenContext* context);
void emitFuncProlog(treeNode_t* node, codeGenContext* context);
void emitFuncEpilog(treeNode_t* node, codeGenContext* context);
void emitInitFuncArgs(treeNode_t* node, codeGenContext* context);
void emitComma(treeNode_t* node, codeGenContext* context);
void emitRet(treeNode_t* node, codeGenContext* context);

void emitCallFunc(treeNode_t* node, codeGenContext* context);
void emitCallFuncArg(treeNode_t* node, codeGenContext* context);
// varMapElem_t* emitCallFuncFreeArgRegs(treeNode_t* node, codeGenContext* context);

void emitIf(treeNode_t* node, codeGenContext* context);
void emitWhile(treeNode_t* node, codeGenContext* context);
void emitCondition(treeNode_t* node, codeGenContext* context);

void emitEs(treeNode_t* node, codeGenContext* context);
void emitStatement(treeNode_t* node, codeGenContext* context);
void emitAssign(treeNode_t* node, codeGenContext* context);

void emitExpression(treeNode_t* node, codeGenContext* context);
void emitAdd(treeNode_t* node, codeGenContext* context);
void emitMul(treeNode_t* node, codeGenContext* context);
void emitSub(treeNode_t* node, codeGenContext* context);
void emitDiv(treeNode_t* node, codeGenContext* context);

void emitSqrt(treeNode_t* node, codeGenContext* context);

// varMapElem_t* 
void emitNumber(treeNode_t* node, codeGenContext* context);
void emitVar(treeNode_t* node, codeGenContext* context);

void emitUnaryOpPreamble(treeNode_t* node, codeGenContext* context);
void emitBinaryOpPreamble(treeNode_t* node, codeGenContext* context);

void emitHlt(treeNode_t* node, codeGenContext* context);

// inline varMapElem_t* emitCurNode(treeNode_t* node, codeGenContext* context);

// varMapElem_t* emitPlug(treeNode_t* node, codeGenContext* context);

void emitCurNode(treeNode_t* node, codeGenContext* context);

// regTableElem_t* loadToReg(varMapElem_t* var, codeGenContext* context);

static void genPreamble(codeGenContext* context);
static void genEpilogue(codeGenContext* context);

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
    {ADD,           emitAdd      },
    {SUB,           emitSub      },
    {MUL,           emitMul      },
    {DIVIDE,        emitDiv      },
    {HLT,           emitHlt      },
    {VARIABLE,      emitVar      },
    {NUMBER,        emitNumber   },
    {SQRT,          emitSqrt     },
//     // {GT,            emitGt    },           
//     // {LT,            emitLt    },
//     // {GE,            emitGe    },
//     // {LE,            emitLe    },
//     // {EQUAL,         emitEqual },
//     // {NOT_EQUAL,     emitNEqual},
//     {IN,            emitPlug     },
//     {OUT,           emitPlug     },
//     // {POPM,          emitPopm  },
//     // {DRAW,          emitDraw  }
};

// const char* MAIN_START_NAME    = "_start";

const char* ADD_OPERATION_NAME  = "ADD";
const char* SUB_OPERATION_NAME  = "SUB" ;
const char* MUL_OPERATION_NAME  = "MUL";
const char* DIV_OPERATION_NAME  = "DIV";

const char* SQRT_OPERATION_NAME = "SQRT";

// const char* CMP_OPERATION_NAME = "cmp";
const char* JMP_OPERATION_NAME  = "JMP";
const char* JG_OPERATION_NAME   = "JG";
const char* JGE_OPERATION_NAME  = "JGE";
const char* JL_OPERATION_NAME   = "JL";
const char* JLE_OPERATION_NAME  = "JLE";
const char* JE_OPERATION_NAME   = "JE";
const char* JNE_OPERATION_NAME  = "JNE";

const char* CALL_OPERATION_NAME = "CALL";
const char* RET_OPERATION_NAME  = "RET";

const size_t EMIT_TABLE_SIZE = sizeof(emittersTable) / sizeof(emitRule);

emitter_t getEmitter(ASTnodeType type){
for(size_t curEmitRuleInd = 0; curEmitRuleInd < EMIT_TABLE_SIZE; curEmitRuleInd++){
        if(type == _EMIT_RULE_TYPE(&emittersTable[curEmitRuleInd])){
            return _EMIT_RULE_EMITTER(&emittersTable[curEmitRuleInd]);
        }
    }

    return NULL;
}   

// varMapElem_t*
void emitStart(treeNode_t* node, codeGenContext* context){ 
    assert(node);
    assert(context);

    LPRINTF("emitStart start");

    genPreamble(context);

    emitCurNode(node, context);

    genEpilogue(context);

    LPRINTF("emitStart end");
}

void emitEb(treeNode_t* node, codeGenContext* context){
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
}

void emitBlock(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitBlock start");

    emitCurNode(node, context);

    LPRINTF("emitBlock end");
}

void emitMain(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMain start");

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
    
    if(_R(node)){
        emitBlock(_R(node), context);
    }
    
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG JX\n");

    LPRINTF("emitMain end");
}

void emitFunc(treeNode_t* node, codeGenContext* context){
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
}

void emitCallFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startCallFunc\n");

    _CONTEXT_FUNC_ARGS_AMOUNT(context) = 0;

    emitCallFuncArg(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "CALL :%s\n", _NODE_VALUE_STR(node));

    for(size_t i = 0; i < _CONTEXT_FUNC_ARGS_AMOUNT(context); i++){
        fprintf(_CONTEXT_FILE_PTR(context), "STKSHRINK\n");
    }

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endCallFunc\n");
}

// varMapElem_t* emitCallFuncFreeArgRegs(treeNode_t* node, codeGenContext* context){
//     assert(node);
//     assert(context);

//     regTableElem_t* refReg = regTableElemCtor(NONE, "", FUNC_ARGS, 1);

//     regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
//     if(foundReg){
//         REG_TABLE_ELEM_USE_BIT(foundReg) = 0;
//         emitCallFuncFreeArgRegs(node, context);
//     }

//     if(foundReg) fprintf(_CONTEXT_FILE_PTR(context), "pop %s\n", REG_TABLE_ELEM_NAME(foundReg));
    
//     regTableElemDtor(refReg);       

//     return _CONTEXT_TEMP_VAR(context);
// }

void emitCallFuncArg(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    if(_L(node)){
        _CONTEXT_FUNC_ARGS_AMOUNT(context)++;
        emitCurNode(_L(node), context);
    }

    // if((_NODE_TYPE(node) != COMMA) && (_NODE_TYPE(node) != FUNCTION)){
        
    //     if

    //     fprintf(_CONTEXT_FILE_PTR(context), "push %s\n", REG_TABLE_ELEM_NAME(foundReg), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    //     fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(foundReg), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    //     REG_TABLE_ELEM_USE_BIT(foundReg) = 1;
    // }
}

void emitInitFunc(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);
    
    _CONTEXT_STACK_OFFSET(context) = -2;
    _CONTEXT_STACK_SHIFT(context)  = -1;

    list_t newVarMap;
    initVarMap(&newVarMap, _CONTEXT_REG_TABLE(context));

    _CONTEXT_VAR_MAP(context) = &newVarMap;


    fprintf(_CONTEXT_FILE_PTR(context), "\n\n:%s\n", _NODE_VALUE_STR(node));
    emitFuncProlog(node, context);

    _CONTEXT_IS_FUNC_ARG(context) = 1;

    if(_L(node)){
        emitInitFuncArgs(_L(node), context);
    }

    _CONTEXT_IS_FUNC_ARG(context) = 0;

    _CONTEXT_STACK_OFFSET(context) = 0;
    _CONTEXT_STACK_SHIFT(context)  = 1;

    if(_R(node)){
        emitBlock(_R(node), context);
    }

    listDtor(&newVarMap, varMapElemDtor);

    freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);
}

void emitInitFuncArgs(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    // _CONTEXT_IS_L_VALUE(context) = 1;
    emitCurNode(node, context);
    // _CONTEXT_IS_L_VALUE(context) = 0;
}

void emitComma(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_FUNC_ARGS_AMOUNT(context)++;
    emitBinaryOpPreamble(node, context);
}

void emitFuncProlog(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG IX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG JX\n");

    for(size_t i = 0; i < LOCAL_VARIABLE_MAX_AMOUNT; i++){
        fprintf(_CONTEXT_FILE_PTR(context), "STKEXTEND\n");
    }
}

void emitRet(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    emitVar(_L(node), context);
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)));

    emitFuncEpilog(node, context);
    fprintf(_CONTEXT_FILE_PTR(context), "RET\n");
}


void emitFuncEpilog(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    for(size_t i = 0; i < LOCAL_VARIABLE_MAX_AMOUNT; i++){
        fprintf(_CONTEXT_FILE_PTR(context), "STKSHRINK\n");
    }

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG IX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG JX\n");
}

void  emitIf(treeNode_t* node, codeGenContext* context){
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

    fprintf(_CONTEXT_FILE_PTR(context), "%s :%s_%d\n", condJumpInstruction, _LABEL_DATA_NAME(ifLabel), _LABEL_DATA_ID(ifLabel));

    if(_R(node)){
        emitBlock(_R(node), context);
    }   

    fprintf(_CONTEXT_FILE_PTR(context), ":%s_%d\n", _LABEL_DATA_NAME(ifLabel), _LABEL_DATA_ID(ifLabel));
}

void emitWhile(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    label_t* whileStartLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_WHILE_BEGIN);
    assert(whileStartLabel);

    fprintf(_CONTEXT_FILE_PTR(context), ":%s_%d\n", _LABEL_DATA_NAME(whileStartLabel), _LABEL_DATA_ID(whileStartLabel));

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


    fprintf(_CONTEXT_FILE_PTR(context), "%s :%s_%d\n", condJumpInstruction, _LABEL_DATA_NAME(whileEndLabel), _LABEL_DATA_ID(whileEndLabel));

    if(_R(node)){
        emitBlock(_R(node), context);
    }   

    fprintf(_CONTEXT_FILE_PTR(context), "%s :%s_%d\n", JMP_OPERATION_NAME, _LABEL_DATA_NAME(whileStartLabel), _LABEL_DATA_ID(whileStartLabel));
    fprintf(_CONTEXT_FILE_PTR(context), ":%s_%d\n", _LABEL_DATA_NAME(whileEndLabel), _LABEL_DATA_ID(whileEndLabel));
}

void emitCondition(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitCondition start");

    emitBinaryOpPreamble(node, context);

    LPRINTF("emitCondition end");
}


void emitEs(treeNode_t* node, codeGenContext* context){
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
}

void emitStatement(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitStatement start");

    // emitTabs(context);

    emitCurNode(node, context);

    LPRINTF("emitStatement end");
}

// varMapElem_t* 
void emitAssign(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitAssign start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startAssign\n");
    
    varMapElem_t* foundVar = NULL;  
    if(_R(node)){
        emitExpression(_R(node), context);
    }

    if(_L(node)){
        _CONTEXT_IS_L_VALUE(context) = 1;
        emitVar(_L(node), context);
        _CONTEXT_IS_L_VALUE(context) = 0;
    }
    foundVar = _CONTEXT_CUR_VAR(context);

    printf("LOC_TYPE = %d\n", VARIABLE_MAP_LOC_TYPE(foundVar));
    if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_REG){
        // fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG((foundVar))), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
    }
    else if(VARIABLE_MAP_LOC_TYPE(foundVar) == LOCK_STACK){
        // fprintf(_CONTEXT_FILE_PTR(context), "PUSH 100\n");       /// stack give mem


    }

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endAssign\n");

    LPRINTF("emitAssign end");

    // return foundVar;
}

// varMapElem_t* 
void emitExpression(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitExpression start");

    // varMapElem_t* retVal = 
    emitCurNode(node, context);

    LPRINTF("emitExpression end");    

    // return retVal;
}

void emitAdd(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitAdd start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startAdd\n");

    emitBinaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", ADD_OPERATION_NAME);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endAdd\n");

    LPRINTF("emitAdd end");    
}

void emitSub(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitSub start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startSub\n");

    emitBinaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", SUB_OPERATION_NAME);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endSub\n");

    LPRINTF("emitSub end");    
}

void emitMul(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMul start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startMul\n");

    emitBinaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", MUL_OPERATION_NAME);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endMul\n");

    LPRINTF("emitMul end");    
}

void emitDiv(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitDiv start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startDiv\n");

    emitBinaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", DIV_OPERATION_NAME);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endDiv\n");

    LPRINTF("emitDiv end");    
}

void emitSqrt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitSqrt start");    

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startSqrt\n");

    emitUnaryOpPreamble(node, context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", SQRT_OPERATION_NAME);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endSqrt\n");

    LPRINTF("emitSqrt end");    
}

void emitVar(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitVar start");
    
    fprintf(_CONTEXT_FILE_PTR(context), "\n;startVar\n");

    printf("amountVariables: %llu\n", _CONTEXT_VAR_MAP(context)->size);

    int curVarCode = 0;
    sscanf(_NODE_WRITE_FILE(node), "VAR%d", &curVarCode);

    varMapElem_t* refElem = varMapElemCtor(curVarCode, LOCK_ANY);
    assert(refElem);

    varMapElem_t* foundElem = varMapFind(_CONTEXT_VAR_MAP(context), varMapCmp, refElem);

    if(!foundElem){
        foundElem = varMapAddVar(_CONTEXT_VAR_MAP(context), _CONTEXT_REG_TABLE(context), curVarCode, _CONTEXT_STACK_OFFSET(context), _CONTEXT_VAR_REG_USE_SCENERY(context));

        if(VARIABLE_MAP_LOC_TYPE(foundElem) == LOCK_STACK){
            // printf("stack case\n");
            _CONTEXT_STACK_OFFSET(context) += _CONTEXT_STACK_SHIFT(context);
        }
    }

    if(!_CONTEXT_IS_FUNC_ARG(context)){
        if(_CONTEXT_IS_L_VALUE(context)){
            fprintf(_CONTEXT_FILE_PTR(context), "PUSH %d\n", VARIABLE_MAP_LOC_STACK_OFFSET(foundElem));
            fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
            fprintf(_CONTEXT_FILE_PTR(context), "ADD\n");
            fprintf(_CONTEXT_FILE_PTR(context), "POPREG %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

            fprintf(_CONTEXT_FILE_PTR(context), "SET %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       
            // fprintf(_CONTEXT_FILE_PTR(context), "STKEXTEND\n");       
        }
        else{
            fprintf(_CONTEXT_FILE_PTR(context), "PUSH %d\n", VARIABLE_MAP_LOC_STACK_OFFSET(foundElem));
            fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
            fprintf(_CONTEXT_FILE_PTR(context), "ADD\n");
            fprintf(_CONTEXT_FILE_PTR(context), "POPREG %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

            fprintf(_CONTEXT_FILE_PTR(context), "GET %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));     
        }
    }

    _CONTEXT_CUR_VAR(context) = foundElem;

    varMapElemDtor(refElem);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endVar\n");

    LPRINTF("emitVar end");
}

// varMapElem_t* 
void emitNumber(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitNumber start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endCallFunc\n");

    fprintf(_CONTEXT_FILE_PTR(context), "PUSH %d\n", _NODE_VALUE_NUM(node));

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endNumber\n");

    LPRINTF("emitNumber end");

    // return _CONTEXT_TEMP_VAR(context);
}

void emitHlt(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "HLT\n");

}

// varMapElem_t* emitPlug(treeNode_t* node, codeGenContext* context){
//     LPRINTF("plug");
//     return _CONTEXT_TEMP_VAR(context);
// }

// inline varMapElem_t* 
void emitCurNode(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    // varMapElem_t* retVal = NULL;

    emitter_t curEmitter = getEmitter(_NODE_TYPE(node));    
    if(curEmitter){
        // retVal = 
        curEmitter(node, context);
    }

    // return retVal;
}

void emitBinaryOpPreamble(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    varMapElem_t* retVal = NULL;
    if(_L(node)){
        emitExpression(_L(node), context);
    }
    retVal = _CONTEXT_CUR_VAR(context);

    // if(_L(node) && _NODE_TYPE(_L(node)) == VARIABLE){
    //     fprintf(_CONTEXT_FILE_PTR(context), "PUSH %d\n", VARIABLE_MAP_LOC_STACK_OFFSET(retVal));
    //     fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
    //     fprintf(_CONTEXT_FILE_PTR(context), "ADD\n");
    //     fprintf(_CONTEXT_FILE_PTR(context), "POPREG %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    //     fprintf(_CONTEXT_FILE_PTR(context), "GET %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));     
    // } 


    if(_R(node)){
        emitExpression(_R(node), context);
    }
    retVal = _CONTEXT_CUR_VAR(context);

    printf("Stack offset var = %d\n", VARIABLE_MAP_LOC_STACK_OFFSET(retVal));

    // if(_R(node) && _NODE_TYPE(_R(node)) == VARIABLE){
    //     fprintf(_CONTEXT_FILE_PTR(context), "PUSH %d\n", VARIABLE_MAP_LOC_STACK_OFFSET(retVal));
    //     fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
    //     fprintf(_CONTEXT_FILE_PTR(context), "ADD\n");
    //     fprintf(_CONTEXT_FILE_PTR(context), "POPREG %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    //     fprintf(_CONTEXT_FILE_PTR(context), "GET %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));     
    // }

}

void emitUnaryOpPreamble(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    varMapElem_t* retVal = NULL;
    if(_R(node)){
        emitExpression(_R(node), context);
    }
    retVal = _CONTEXT_CUR_VAR(context);

    // if(_NODE_TYPE(_R(node)) == VARIABLE){
    //     fprintf(_CONTEXT_FILE_PTR(context), "PUSH %d\n", VARIABLE_MAP_LOC_STACK_OFFSET(retVal));
    //     fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
    //     fprintf(_CONTEXT_FILE_PTR(context), "ADD\n");
    //     fprintf(_CONTEXT_FILE_PTR(context), "POPREG %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));

    //     fprintf(_CONTEXT_FILE_PTR(context), "GET %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));
    // }
}

// regTableElem_t* loadToReg(varMapElem_t* var, codeGenContext* context){
//     if (VARIABLE_MAP_LOC_TYPE(var) == LOCK_REG){
//         if(REG_TABLE_ELEM_USE_SCENERY(VARIABLE_MAP_LOC_REG(var)) != TEMP_STORE) fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(VARIABLE_MAP_LOC_REG(var)));
//     }
//     else{
//         fprintf(_CONTEXT_FILE_PTR(context), "mov %s, [rbp - %d]\n", 
//         REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), VARIABLE_MAP_LOC_STACK_OFFSET(var));
//     }

//     return _CONTEXT_TEMP_REG(context);
// }

static void genPreamble(codeGenContext* context){
    assert(context);

    // fprintf(_CONTEXT_FILE_PTR(context), "section .text\n");
    // fprintf(_CONTEXT_FILE_PTR(context), "global _start\n");
}

static void genEpilogue(codeGenContext* context){
    assert(context);


}