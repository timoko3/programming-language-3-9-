#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H

union treeVal_t{
    char* op;
    char* var;
    int num;
};

enum nodeType{
    NO_TYPE,
    OPERATOR,
    VARIABLE,
    NUMBER
};

struct treeNode_t{
    nodeType    type;
    treeVal_t   data;
    treeNode_t* right;
    treeNode_t* left;
    treeNode_t* parent;
};

treeNode_t* readExpression(tree_t* expression, char* buffer, size_t* curBufferPos);

treeNode_t* createNewNodeNumber  (int value,  treeNode_t* left, treeNode_t* right);
treeNode_t* createNewNodeVariable(const char* type, treeNode_t* left, treeNode_t* right);
treeNode_t* createNewNodeOperator(const char* type, treeNode_t* left, treeNode_t* right);

void copyExpressionNode(treeNode_t* copy, treeNode_t* node);
bool freeExpressionNodeData(treeNode_t* node, bool withoutRoot, int depth);

bool checkNotHaveVariables(treeNode_t* curNode);

#endif /* EXPRESSION_TREE_H */