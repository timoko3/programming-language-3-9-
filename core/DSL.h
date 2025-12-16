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

#define _CONTEXT_FILE_PTR(context)       (context)->filePtr
#define _CONTEXT_NAMES(context)          (context)->names
// #define _CONTEXT_LABELS(context)         (context)->labels
// #define _CONTEXT_LABELS_AMOUNT(context)  (context)->labelsAmount

#define _SPU_NAME_TABLE_SIZE(spuNameTable) (spuNameTable)->size
#define _SPU_NAME_TABLE_CAPACITY(spuNameTable) (spuNameTable)->capacity
#define _SPU_NAME_TABLE_DATA(spuNameTable) (spuNameTable)->data

#define _SPU_NAME_DATA_STR(name) (name).str
#define _SPU_NAME_DATA_TYPE(name) (name).type
#define _SPU_NAME_ADDR(name) (name).addr

#define _LABEL_DATA_NAME(label) (label)->name
#define _LABEL_DATA_NUM(label)  (label)->number
#define _LABEL_TABLE_DATA(label) (label)->data

#define _LABEL_TABLE_SIZE(labelTable) (labelTable)->size
#define _LABEL_TABLE_CAPACITY(labelTable) (labelTable)->capacity

#endif /* DSL_H */