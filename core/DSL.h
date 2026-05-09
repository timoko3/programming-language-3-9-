#ifndef DSL_H
#define DSL_H

#define _NODE_TYPE(node)       node->data->type
#define _NODE_VALUE_STR(node)  node->data->value.str
#define _NODE_VALUE_NUM(node)  node->data->value.num
#define _NODE_WRITE_FILE(node) node->data->writeFile
#define _L(node)               node->left
#define _R(node)               node->right
#define _PAR(node)             node->parent

#define _TOKEN_TYPE(token)       (token)->type
#define _TOKEN_VALUE_STR(token)  (token)->value.str
#define _TOKEN_VALUE_NUM(token)  (token)->value.num
#define _TOKEN_WRITE_FILE(token) (token)->writeFile

#define _AST_NODE_TYPE(ASTnode)       (ASTnode)->type
#define _AST_NODE_VALUE_STR(ASTnode)  (ASTnode)->value.str
#define _AST_NODE_VALUE_NUM(ASTnode)  (ASTnode)->value.num
#define _AST_NODE_WRITE_FILE(ASTnode) (ASTnode)->writeFile

#define _EMIT_RULE_TYPE(emitRule) (emitRule)->type
#define _EMIT_RULE_EMITTER(emitRule) (emitRule)->emitter

#define _CONTEXT_FILE_PTR(context)            (context)->filePtr
#define _CONTEXT_NAMES(context)               (context)->names
#define _CONTEXT_LABELS_TABLE(context)        (context)->labelsTable
#define _CONTEXT_CUR_LABEL_A(context)         (context)->curLabelA
#define _CONTEXT_CUR_LABEL_B(context)         (context)->curLabelB
#define _CONTEXT_STACK_OFFSET(context)        (context)->stackOffset
#define _CONTEXT_STACK_SHIFT(context)         (context)->stackShift
#define _CONTEXT_FUNC_ARGS_AMOUNT(context)    (context)->curFuncArgsAmount
#define _CONTEXT_VAR_MAP(context)             (context)->varMap
#define _CONTEXT_TEMP_VAR(context)            (context)->tempVar
#define _CONTEXT_CUR_VAR(context)             (context)->curVar
#define _CONTEXT_SAVE_CUR_VAR(context)        (context)->saveCurVar  
#define _CONTEXT_VAR_REG_USE_SCENERY(context) (context)->curVarRegUseScenery
#define _CONTEXT_REG_TABLE(context)           (context)->regTable
#define _CONTEXT_TEMP_REG(context)            (context)->tempReg
#define _CONTEXT_FUNC_RET_REG(context)        (context)->funcRetReg
#define _CONTEXT_CALC_REG_A(context)          (context)->calcRegA
#define _CONTEXT_CALC_REG_B(context)          (context)->calcRegB
#define _CONTEXT_BLOCK_IM_DEPTH(context)      (context)->blockImmersionDepth
#define _CONTEXT_IS_L_VALUE(context)          (context)->isLValue
#define _CONTEXT_IS_CALL_FUNC_ARG(context)    (context)->isFuncArg
#define _CONTEXT_COUNT_ARGS(context)          (context)->countArgs

#define _CONTEXT_ELF_SIZE_EH(context)               (context)->elfGenContext.elfHeaderSize
#define _CONTEXT_ELF_SIZE_PH(context)               (context)->elfGenContext.programHeaderSize
#define _CONTEXT_ELF_CODE_SIZE(context)             (context)->elfGenContext.codeSize
#define _CONTEXT_ELF_SECTION_TABLE_SIZE(context)    (context)->elfGenContext.sectionTableSize
#define _CONTEXT_ELF_TABLE_NAMES_TS(context)        (context)->elfGenContext.tableNamesTs

#define _SPU_NAME_TABLE_SIZE(spuNameTable) (spuNameTable)->size
#define _SPU_NAME_TABLE_CAPACITY(spuNameTable) (spuNameTable)->capacity
#define _SPU_NAME_TABLE_DATA(spuNameTable) (spuNameTable)->data

#define _SPU_NAME_DATA_STR(name) (name).str
#define _SPU_NAME_DATA_TYPE(name) (name).type
#define _SPU_NAME_ADDR(name) (name).addr
#define _SPU_NAME_SCOPE(name) (name).scope

#define _LABEL_DATA_NAME(label)   (label)->name
#define _LABEL_DATA_ID(label)     (label)->id

#define HASH_TABLE_CAPACITY(hashTable)         hashTable->capacity
#define HASH_TABLE_AMOUNT_ELEMENTS(hashTable)  hashTable->amountElements
#define HASH_TABLE_FUNCTION(hashTable)         hashTable->hashFunction
#define HASH_TABLE_CELLS(hashTable)            hashTable->cells

#define HASH_TABLE_CELL_VALUE(hashTableCell)   hashTableCell->value
#define HASH_TABLE_CELL_HASH(hashTableCell)    hashTableCell->hash

#define REG_TABLE_ELEM_REG(regTable)           regTable->reg
#define REG_TABLE_ELEM_NAME(regTable)          regTable->name
#define REG_TABLE_ELEM_USE_BIT(regTable)       regTable->useBit
#define REG_TABLE_ELEM_USE_SCENERY(regTable)   regTable->useScenery
#define REG_TABLE_ELEM_VARIABLE_CODE(regTable) regTable->variableCode

#define VARIABLE_MAP_VARIABLE_CODE(variableMap)    variableMap->variableCode       
#define VARIABLE_MAP_LOC_TYPE(variableMap)         variableMap->locType        
#define VARIABLE_MAP_LOC_REG(variableMap)          variableMap->loc.reg        
#define VARIABLE_MAP_LOC_STACK_OFFSET(variableMap) variableMap->loc.stackOffset        

#define VISITOR_PRE_ORDER_FUNC(visitor)    visitor->preOrderfunc
#define VISITOR_IN_ORDER_FUNC(visitor)     visitor->inOrderfunc
#define VISITOR_POST_ORDER_FUNC(visitor)   visitor->postOrderfunc
#define VISITOR_TYPE(visitor)              visitor->type
#define VISITOR_IS_LR_ORDER(visitor)       visitor->isLrOrderFlag
#define VISITOR_CONTEXT(visitor)           visitor->context
#define VISITORS_AMOUNT(visitor)           visitor->amountNodeVisitors
#define NODE_VISITORS(visitor)             visitor->nodeVisitors

#endif /* DSL_H */