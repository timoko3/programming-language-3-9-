#include "emittersSpu.h"
// #include "format.h"

#include "core/core.h"
#include "core/DSL.h"

// #define DEBUG

#include "general/debug.h"

#include <assert.h>
#include <malloc.h>

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
};

//     {IN,            emitPlug     },
//     {OUT,           emitPlug     },
//     // {POPM,          emitPopm  },
//     // {DRAW,          emitDraw  }

const char* ADD_OPERATION_NAME_SPU  = "ADD";
const char* SUB_OPERATION_NAME_SPU  = "SUB" ;
const char* MUL_OPERATION_NAME_SPU  = "MUL";
const char* DIV_OPERATION_NAME_SPU  = "DIV";

const char* SQRT_OPERATION_NAME_SPU = "SQRT";

const char* JMP_OPERATION_NAME_SPU  = "JMP";
const char* JG_OPERATION_NAME_SPU   = "JG";
const char* JGE_OPERATION_NAME_SPU  = "JGE";
const char* JL_OPERATION_NAME_SPU   = "JL";
const char* JLE_OPERATION_NAME_SPU  = "JLE";
const char* JE_OPERATION_NAME_SPU   = "JE";
const char* JNE_OPERATION_NAME_SPU  = "JNE";

const char* CALL_OPERATION_NAME_SPU = "CALL";
const char* RET_OPERATION_NAME_SPU  = "RET";

void emitMainSpuPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMain start");

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
}

void emitMainSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "POPREG JX\n");
}

void emitFuncSpuPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFuncSpuPre(node, context);
    }
    else{
        emitCallFuncSpuPre(node, context);
    }

    LPRINTF("emitFunc end");
}
void emitFuncSpuIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFuncSpuIn(node, context);
    }

    LPRINTF("emitFunc end");
}
void emitFuncSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitFunc start");

    if(_R(node)){
        emitInitFuncSpuPost(node, context);
    }
    else{
        emitCallFuncSpuPost(node, context);
    }

    LPRINTF("emitFunc end");
}

void emitCallFuncSpuPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startCallFunc\n");

    _CONTEXT_FUNC_ARGS_AMOUNT(context) = 0;
    _CONTEXT_COUNT_ARGS(context)       = 1;
}

void emitCallFuncSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "CALL :%s\n", _NODE_VALUE_STR(node));

    for(size_t i = 0; i < _CONTEXT_FUNC_ARGS_AMOUNT(context); i++){
        fprintf(_CONTEXT_FILE_PTR(context), "STKSHRINK\n");
    }

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)));
}

void emitInitFuncSpuPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);
    
    _CONTEXT_STACK_OFFSET(context) = -2;
    _CONTEXT_STACK_SHIFT(context)  = -1;

    list_t* newVarMap = (list_t*) calloc(1, sizeof(list_t));
    initVarMap(newVarMap, _CONTEXT_REG_TABLE(context));

    _CONTEXT_VAR_MAP(context) = newVarMap;

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n:%s\n", _NODE_VALUE_STR(node));
    emitFuncPrologSpu(node, context);

    _CONTEXT_IS_CALL_FUNC_ARG(context) = 1;
}

void emitInitFuncSpuIn(treeNode_t* node, codeGenContext* context){

    _CONTEXT_IS_CALL_FUNC_ARG(context) = 0;

    _CONTEXT_STACK_OFFSET(context) = 0;
    _CONTEXT_STACK_SHIFT(context)  = 1;
}

void emitInitFuncSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    listDtor(_CONTEXT_VAR_MAP(context), varMapElemDtor);
    free(_CONTEXT_VAR_MAP(context));

    freeTypeRegs(_CONTEXT_REG_TABLE(context), FUNC_ARGS);
}

void emitFuncPrologSpu(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG IX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG JX\n");

    for(size_t i = 0; i < LOCAL_VARIABLE_MAX_AMOUNT; i++){
        fprintf(_CONTEXT_FILE_PTR(context), "STKEXTEND\n");
    }
}

void emitRetSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "POPREG %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_FUNC_RET_REG(context)));

    emitFuncEpilogSpu(node, context);
    fprintf(_CONTEXT_FILE_PTR(context), "RET\n");
}


void emitFuncEpilogSpu(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    for(size_t i = 0; i < LOCAL_VARIABLE_MAX_AMOUNT; i++){
        fprintf(_CONTEXT_FILE_PTR(context), "STKSHRINK\n");
    }

    fprintf(_CONTEXT_FILE_PTR(context), "PUSHREG JX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG IX\n");
    fprintf(_CONTEXT_FILE_PTR(context), "POPREG JX\n");
}

void  emitIfSpuIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    const char* condJumpInstruction = "";

    switch(_NODE_TYPE(_L(node))){
        case LE:        condJumpInstruction = JG_OPERATION_NAME_SPU ; break; 
        case LT:        condJumpInstruction = JGE_OPERATION_NAME_SPU; break; 
        case GE:        condJumpInstruction = JL_OPERATION_NAME_SPU ; break; 
        case GT:        condJumpInstruction = JLE_OPERATION_NAME_SPU; break; 
        case EQUAL:     condJumpInstruction = JNE_OPERATION_NAME_SPU; break; 
        case NOT_EQUAL: condJumpInstruction = JE_OPERATION_NAME_SPU ; break; 
        default: break;
    }

    label_t* ifLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_IF_END);
    assert(ifLabel);

    _CONTEXT_CUR_LABEL_A(context) = ifLabel;

    fprintf(_CONTEXT_FILE_PTR(context), "%s :%s_%d\n", condJumpInstruction, _LABEL_DATA_NAME(ifLabel), _LABEL_DATA_ID(ifLabel));
}

void emitIfSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), ":%s_%d\n", _LABEL_DATA_NAME(_CONTEXT_CUR_LABEL_A(context)), _LABEL_DATA_ID(_CONTEXT_CUR_LABEL_A(context)));
}

void emitWhileSpuPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    label_t* whileStartLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_WHILE_BEGIN);
    assert(whileStartLabel);

    fprintf(_CONTEXT_FILE_PTR(context), ":%s_%d\n", _LABEL_DATA_NAME(whileStartLabel), _LABEL_DATA_ID(whileStartLabel));

    _CONTEXT_CUR_LABEL_A(context) = whileStartLabel;
}

void emitWhileSpuIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    const char* condJumpInstruction = "";

    switch(_NODE_TYPE(_L(node))){
        case LE:        condJumpInstruction = JG_OPERATION_NAME_SPU ; break; 
        case LT:        condJumpInstruction = JGE_OPERATION_NAME_SPU; break; 
        case GE:        condJumpInstruction = JL_OPERATION_NAME_SPU ; break; 
        case GT:        condJumpInstruction = JLE_OPERATION_NAME_SPU; break; 
        case EQUAL:     condJumpInstruction = JNE_OPERATION_NAME_SPU; break; 
        case NOT_EQUAL: condJumpInstruction = JE_OPERATION_NAME_SPU ; break; 
        default: break;
    }

    label_t* whileEndLabel = createLabel(_CONTEXT_LABELS_TABLE(context), LABEL_PREFIX_WHILE_END);
    assert(whileEndLabel);

    fprintf(_CONTEXT_FILE_PTR(context), "%s :%s_%d\n", condJumpInstruction, _LABEL_DATA_NAME(whileEndLabel), _LABEL_DATA_ID(whileEndLabel));

    _CONTEXT_CUR_LABEL_B(context) = whileEndLabel;
}

void emitWhileSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s :%s_%d\n", JMP_OPERATION_NAME_SPU, _LABEL_DATA_NAME(_CONTEXT_CUR_LABEL_A(context)), _LABEL_DATA_ID(_CONTEXT_CUR_LABEL_A(context)));
    fprintf(_CONTEXT_FILE_PTR(context), ":%s_%d\n", _LABEL_DATA_NAME(_CONTEXT_CUR_LABEL_B(context)), _LABEL_DATA_ID(_CONTEXT_CUR_LABEL_B(context)));
}

void emitAssignSpuIn(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_IS_L_VALUE(context) = 1;
}

void emitAssignSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    _CONTEXT_IS_L_VALUE(context) = 0;
    varMapElem_t* foundVar = _CONTEXT_CUR_VAR(context);

    printf("LOC_TYPE = %d\n", VARIABLE_MAP_LOC_TYPE(foundVar));


    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endAssign\n");

    LPRINTF("emitAssign end");
}

void emitAddSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", ADD_OPERATION_NAME_SPU);

}

void emitSubSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", SUB_OPERATION_NAME_SPU);
}

void emitMulSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", MUL_OPERATION_NAME_SPU);
}

void emitDivSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", DIV_OPERATION_NAME_SPU);   
}

void emitSqrtSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "%s\n", SQRT_OPERATION_NAME_SPU);
}

void emitVarSpuPre(treeNode_t* node, codeGenContext* context){
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

    if(!_CONTEXT_IS_CALL_FUNC_ARG(context)){
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

    if(_CONTEXT_COUNT_ARGS(context)) _CONTEXT_FUNC_ARGS_AMOUNT(context)++;

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endVar\n");

    LPRINTF("emitVar end");
}

void emitNumberSpuPre(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "PUSH %d\n", _NODE_VALUE_NUM(node));

    if(_CONTEXT_COUNT_ARGS(context)) _CONTEXT_FUNC_ARGS_AMOUNT(context)++;
}

void emitHltSpuPost(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    fprintf(_CONTEXT_FILE_PTR(context), "HLT\n");

}