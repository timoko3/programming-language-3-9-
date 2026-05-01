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

#define _CONTEXT_FILE_PTR(context)           (context)->filePtr
#define _CONTEXT_NAMES(context)              (context)->names
#define _CONTEXT_LABELS(context)             (context)->labels
#define _CONTEXT_STACK_FRAME_OFFSET(context) (context)->stackFrameOffset
#define _CONTEXT_REG_TABLE(context)          (context)->regTable

#define _SPU_NAME_TABLE_SIZE(spuNameTable) (spuNameTable)->size
#define _SPU_NAME_TABLE_CAPACITY(spuNameTable) (spuNameTable)->capacity
#define _SPU_NAME_TABLE_DATA(spuNameTable) (spuNameTable)->data

#define _SPU_NAME_DATA_STR(name) (name).str
#define _SPU_NAME_DATA_TYPE(name) (name).type
#define _SPU_NAME_ADDR(name) (name).addr
#define _SPU_NAME_SCOPE(name) (name).scope

#define _LABEL_DATA_PREFIX(label) (label)->prefix
#define _LABEL_DATA_ID(label)  (label)->id
#define _LABEL_TABLE_DATA(label) (label)->data

#define _LABEL_TABLE_SIZE(labelTable) (labelTable)->size
#define _LABEL_TABLE_CAPACITY(labelTable) (labelTable)->capacity

#define HASH_TABLE_CAPACITY(hashTable)         hashTable->capacity
#define HASH_TABLE_AMOUNT_ELEMENTS(hashTable)  hashTable->amountElements
#define HASH_TABLE_FUNCTION(hashTable)         hashTable->hashFunction
#define HASH_TABLE_CELLS(hashTable)            hashTable->cells

#define HASH_TABLE_CELL_VALUE(hashTableCell)   hashTableCell->value
#define HASH_TABLE_CELL_HASH(hashTableCell)    hashTableCell->hash

#define REG_TABLE_ELEM_REG(regTable)           regTable->reg
#define REG_TABLE_ELEM_USE_BIT(regTable)       regTable->useBit
#define REG_TABLE_ELEM_USE_SCENERY(regTable)   regTable->useScenery
#define REG_TABLE_ELEM_VARIABLE_CODE(regTable) regTable->variableCode

#endif /* DSL_H */