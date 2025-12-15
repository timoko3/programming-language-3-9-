#ifndef DSL_H
#define DSL_H

#define _NODE_TYPE(node)       node->data->type
#define _NODE_VALUE_STR(node)  node->data->value.str
#define _NODE_VALUE_NUM(node)  node->data->value.num
#define _NODE_WRITE_FILE(node) node->data->writeFile
#define _L(node)               node->left
#define _R(node)               node->right

#define _TOKEN_TYPE(token)       (token)->type
#define _TOKEN_VALUE_STR(token)  (token)->value.str
#define _TOKEN_VALUE_NUM(token)  (token)->type.num
#define _TOKEN_WRITE_FILE(token) (token)->writeFile

#endif /* DSL_H */