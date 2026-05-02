#include "emittersX86.h"
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

regTableElem_t* emitEb(treeNode_t* node, codeGenContext* context);
regTableElem_t* emitBlock(treeNode_t* node, codeGenContext* context);
regTableElem_t* emitMain(treeNode_t* node, codeGenContext* context);


regTableElem_t* emitEs(treeNode_t* node, codeGenContext* context);
regTableElem_t* emitStatement(treeNode_t* node, codeGenContext* context);
regTableElem_t* emitAssign(treeNode_t* node, codeGenContext* context);

regTableElem_t* emitExpression(treeNode_t* node, codeGenContext* context);
regTableElem_t* emitMul(treeNode_t* node, codeGenContext* context);
regTableElem_t* emitAdd(treeNode_t* node, codeGenContext* context);
regTableElem_t* emitSub(treeNode_t* node, codeGenContext* context);

regTableElem_t* emitNumber(treeNode_t* node, codeGenContext* context);
regTableElem_t* emitVar(treeNode_t* node, codeGenContext* context);

regTableElem_t* emitBinaryOp(treeNode_t* node, codeGenContext* context, const char* nameOp);

inline regTableElem_t* emitNonTerminal(treeNode_t* node, codeGenContext* context);

regTableElem_t* emitPlug(treeNode_t* node, codeGenContext* context);

static emitRule emittersTable[] = {
    {END_BLOCK,     emitEb       },
    {MAIN,          emitMain     },
    {END_STATEMENT, emitEs       },
    {ASSIGN,        emitAssign   },
    {VARIABLE,      emitVar      },
    {NUMBER,        emitNumber   },
    {SUB,           emitSub      },
    {MUL,           emitMul      },
    {ADD,           emitAdd      },
    // {DIVIDE,        emitDiv   },
    // {HLT,           emitHlt   },
    // {RETURN,        emitRet   },
    // {GT,            emitGt    },           
    // {LT,            emitLt    },
    // {GE,            emitGe    },
    // {LE,            emitLe    },
    // {EQUAL,         emitEqual },
    // {NOT_EQUAL,     emitNEqual},
    // {SQRT,          emitSqrt  },
    {IF,            emitPlug     },
    {WHILE,         emitPlug     },
    {IN,            emitPlug     },
    {OUT,           emitPlug     },
    // {POPM,          emitPopm  },
    // {DRAW,          emitDraw  }
};

const char* ADD_OPERATION_NAME = "add";
const char* SUB_OPERATION_NAME = "sub";
const char* MUL_OPERATION_NAME = "imul";

const size_t EMIT_TABLE_SIZE = sizeof(emittersTable) / sizeof(emitRule);

emitter_t getEmitter(ASTnodeType type){
for(size_t curEmitRuleInd = 0; curEmitRuleInd < EMIT_TABLE_SIZE; curEmitRuleInd++){
        if(type == _EMIT_RULE_TYPE(&emittersTable[curEmitRuleInd])){
            return _EMIT_RULE_EMITTER(&emittersTable[curEmitRuleInd]);
        }
    }

    return NULL;
}   

regTableElem_t* emitNode(treeNode_t* node, codeGenContext* context){ 
    assert(node);
    assert(context);

    LPRINTF("emitNode start");

    emitNonTerminal(node, context);

    LPRINTF("emitNode end");

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitEb(treeNode_t* node, codeGenContext* context){
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

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitBlock(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitBlock start");

    _CONTEXT_BLOCK_IM_DEPTH(context)++;

    emitNonTerminal(node, context);

    _CONTEXT_BLOCK_IM_DEPTH(context)--;

    LPRINTF("emitBlock end");

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitMain(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMain start");

    fprintf(_CONTEXT_FILE_PTR(context), "_start:\n");

    if(_R(node)){
        emitBlock(_R(node), context);
    }

    fprintf(_CONTEXT_FILE_PTR(context), "\n; sys_exit(0)\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rax, 60\n");
    fprintf(_CONTEXT_FILE_PTR(context), "mov rdi, 0\n");
    fprintf(_CONTEXT_FILE_PTR(context), "syscall\n");

    LPRINTF("emitMain end");

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitEs(treeNode_t* node, codeGenContext* context){
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

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitStatement(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitStatement start");

    // emitTabs(context);

    emitNonTerminal(node, context);

    LPRINTF("emitStatement end");

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitAssign(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitAssign start");
    
    regTableElem_t* foundVar = NULL;
    if(_L(node)){
        foundVar = emitVar(_L(node), context);
    }

    if(_R(node)){
        emitExpression(_R(node), context);
    }

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(foundVar), REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)));       

    LPRINTF("emitAssign end");

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitExpression(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitExpression start");

    regTableElem_t* retVal = emitNonTerminal(node, context);

    LPRINTF("emitExpression end");    

    return retVal;
}

regTableElem_t* emitAdd(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitSub start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startSub\n");

    emitBinaryOp(node, context, ADD_OPERATION_NAME);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endSub\n");

    LPRINTF("emitSub end");    

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitSub(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitSub start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startSub\n");

    emitBinaryOp(node, context, SUB_OPERATION_NAME);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endSub\n");

    LPRINTF("emitSub end");    

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitMul(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitMul start");

    fprintf(_CONTEXT_FILE_PTR(context), "\n;startMul\n");

    emitBinaryOp(node, context, MUL_OPERATION_NAME);

    fprintf(_CONTEXT_FILE_PTR(context), "\n\n;endMul\n");

    LPRINTF("emitMul end");    

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitVar(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitVar start");

    int curVarCode = 0;
    sscanf(_NODE_WRITE_FILE(node), "VAR%d", &curVarCode);

    regTableElem_t* refReg = regTableElemCtor(NONE, "", STORE_VAR, curVarCode, 0);
    assert(refReg);

    regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findVar, refReg);

    if(!foundReg){
        REG_TABLE_ELEM_VARIABLE_CODE(refReg) = PZN_VARIABLE_CODE;
        REG_TABLE_ELEM_USE_SCENERY(refReg)   = STORE_VAR;

        foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findTypeRegFree, refReg);
        assert(foundReg);   /// remove when stack added

        REG_TABLE_ELEM_VARIABLE_CODE(foundReg) = curVarCode;
        REG_TABLE_ELEM_USE_BIT(foundReg) = 1;
    }

    printf("code = %d\n", REG_TABLE_ELEM_VARIABLE_CODE(foundReg));

    regTableElemDtor(refReg);

    LPRINTF("emitVar end");

    return foundReg;
}

regTableElem_t* emitNumber(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    LPRINTF("emitNumber start");

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %d\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), _NODE_VALUE_NUM(node));

    LPRINTF("emitNumber end");

    return _CONTEXT_TEMP_REG(context);
}

regTableElem_t* emitPlug(treeNode_t* node, codeGenContext* context){
    LPRINTF("plug");
    return _CONTEXT_TEMP_REG(context);
}

inline regTableElem_t* emitNonTerminal(treeNode_t* node, codeGenContext* context){
    assert(node);
    assert(context);

    regTableElem_t* retVal = NULL;

    emitter_t curEmitter = getEmitter(_NODE_TYPE(node));    
    if(curEmitter){
        retVal = curEmitter(node, context);
    }

    return retVal;
}

regTableElem_t* emitBinaryOp(treeNode_t* node, codeGenContext* context, const char* nameOp){
    assert(node);
    assert(context);
    assert(nameOp);

    regTableElem_t* retVal = NULL;
    if(_L(node)){
        retVal = emitExpression(_L(node), context);
    }

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(retVal));
    fprintf(_CONTEXT_FILE_PTR(context), "push %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    if(_R(node)){
        retVal = emitExpression(_R(node), context);
    }

    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)), REG_TABLE_ELEM_NAME(retVal));
    fprintf(_CONTEXT_FILE_PTR(context), "pop %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    fprintf(_CONTEXT_FILE_PTR(context), "%s %s, %s\n", nameOp, REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_B(context)));
    fprintf(_CONTEXT_FILE_PTR(context), "mov %s, %s\n", REG_TABLE_ELEM_NAME(_CONTEXT_TEMP_REG(context)), REG_TABLE_ELEM_NAME(_CONTEXT_CALC_REG_A(context)));

    return _CONTEXT_TEMP_REG(context);
}