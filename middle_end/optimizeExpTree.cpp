#include "../general/tree/tree.h"
#include "../general/debug.h"
#include "../general/strFunc.h"

#include "../core/DSL.h"
#include "../core/expressionTree.h"

#include "mathHandlersMiddleEnd.h"

#include <assert.h>
#include <malloc.h>

static treeNode_t* collapseConstant         (tree_t* derivative, treeNode_t* subTreeRoot);

static treeNode_t* removeNeutralElements    (tree_t* derivative, treeNode_t* subTreeRoot, treeNode_t** addrToAsignNewSubTree);
static treeNode_t* removeNeutralSubtree     (tree_t* derivative, treeNode_t* subTreeRoot, treeNode_t* remainSubTreeRoot);
static treeNode_t* removeLeftNeutralSubtree (tree_t* derivative, treeNode_t* subTreeRoot);
static treeNode_t* removeRightNeutralSubtree(tree_t* derivative, treeNode_t* subTreeRoot);
static double calculateSubTree              (treeNode_t* subTreeRoot);

typedef double (*calcHandler_t)(double* params);

struct operation_t{
    ASTnodeType    type;
    calcHandler_t  calcHandler;
    size_t         paramCount;
};

static operation_t operations[]{
    {ADD,      addH, 2},
    {SUB,      subH, 2},
    {MUL,      mulH, 2},
    {DIVIDE,   divH, 2}
};

bool optimizeExpression(tree_t* derivative, treeNode_t* subTreeRoot){
    assert(derivative);
    assert(subTreeRoot);

    static int optimizeCalls = 0;
    optimizeCalls++;
    LPRINTF("optimizeCalls = %d", optimizeCalls);

    LPRINTF("\n\nstart optimization");

    if(_L(subTreeRoot)){
        optimizeExpression(derivative, _L(subTreeRoot));
    }
    if(_R(subTreeRoot)){
        optimizeExpression(derivative, _R(subTreeRoot));
    }

    if(checkSubTreeCountable(subTreeRoot)){
        LPRINTF("subTreeRoot addr: %p", subTreeRoot);
        subTreeRoot = collapseConstant(derivative, subTreeRoot);

        if(subTreeRoot == derivative->root){
            return true;
        }
    }

    LPRINTF("subTreeRoot = %p", subTreeRoot);
    LPRINTF("subTreeRoot->parent = %p", _PAR(subTreeRoot));

    treeNode_t** addrToAsignNewSubTree = NULL;
    if(_PAR(subTreeRoot)){
        LPRINTF("addrToAsignNewSubTree init with %p", &derivative->root);
        if(_L(_PAR(subTreeRoot)) == subTreeRoot){
            addrToAsignNewSubTree = &_L(_PAR(subTreeRoot));
        }
        else if(_R(_PAR(subTreeRoot)) == subTreeRoot){
            addrToAsignNewSubTree = &_R(_PAR(subTreeRoot));
        }
    }
    else{
        LPRINTF("addrToAsignNewSubTree init with %p", &derivative->root);
        addrToAsignNewSubTree = &derivative->root;
    }
    assert(addrToAsignNewSubTree);

    // subTreeRoot = removeNeutralElements(derivative, subTreeRoot, addrToAsignNewSubTree);

    return true;
}

static treeNode_t* collapseConstant(tree_t* derivative, treeNode_t* subTreeRoot){
    assert(subTreeRoot);

    static size_t collapseConstantCount = 1;
    // logTree(derivative, "collapseConst number %lu. CurSubTreeRoot = %p", collapseConstantCount, subTreeRoot);
    collapseConstantCount++;

    LPRINTF("start collapsing constant");

    treeNode_t* result = NULL;

    double calculatedVal = calculateSubTree(subTreeRoot);
    LPRINTF("calculatedVal: %d", calculatedVal);

    if(!_L(subTreeRoot) && !_R(subTreeRoot)){
        LPRINTF("collapse constant no need");

        return subTreeRoot; 
    }

    freeNode(subTreeRoot, true);
    
    LPRINTF("freed collapsingConst with no problems");

    LPRINTF("subTreeRoot = %p\n", subTreeRoot);
    if(subTreeRoot == derivative->root){
        LPRINTF("subTreeRoot == derivative->root");
        derivative->root = createNewNodeNumber(calculatedVal, NULL, NULL);
        
        freeExpressionNodeData(subTreeRoot, false, 1);
        free(subTreeRoot);

        return derivative->root;
    }

    if(_L(_PAR(subTreeRoot)) == subTreeRoot){
        LPRINTF("case subTreeRoot parent's left");
        _L(_PAR(subTreeRoot)) = createNewNodeNumber(calculatedVal, NULL, NULL);
        result = _L(_PAR(subTreeRoot));
    }
    else{
        LPRINTF("case subTreeRoot parent's left");
        _R(_PAR(subTreeRoot)) = createNewNodeNumber(calculatedVal, NULL, NULL);
        result = _R(_PAR(subTreeRoot));
    }
    LPRINTF("MEOW");
    setParent(_PAR(subTreeRoot));

    LPRINTF("setParent succeed in collapseConstant func");

    LPRINTF("free subTreeRoot: %p", subTreeRoot);
    freeExpressionNodeData(subTreeRoot, false, 1);
    free(subTreeRoot->data);
    free(subTreeRoot);

    LPRINTF("ended collapsing constant and free root");
    return result;
}

static treeNode_t* removeNeutralElements(tree_t* derivative, treeNode_t* subTreeRoot, treeNode_t** addrToAsignNewSubTree){
    assert(subTreeRoot);
    assert(derivative);
    assert(addrToAsignNewSubTree);

    static size_t removeNeutralCount = 1;
    // logTree(derivative, "removeNeutral number %lu. CurSubTreeRoot = %p", removeNeutralCount, subTreeRoot);
    removeNeutralCount++;

    LPRINTF("check if can removal neutral subTreeRoot = %p", subTreeRoot);
    if(_NODE_TYPE(subTreeRoot) == MUL && _NODE_VALUE_NUM(_L(subTreeRoot)) == 0){
        LPRINTF("zero multiply case removal neutral");

        freeNode(subTreeRoot, true);

        assert(_PAR(subTreeRoot));

        treeNode_t* newNode = NULL;
        if(_L(_PAR(subTreeRoot)) == subTreeRoot){
            _L(_PAR(subTreeRoot)) = createNewNodeNumber(0, NULL, NULL);
            newNode = _L(_PAR(subTreeRoot));
        }
        else{
            _R(_PAR(subTreeRoot)) = createNewNodeNumber(0, NULL, NULL);
            newNode = _R(_PAR(subTreeRoot));
        }
        _PAR(newNode) = _PAR(subTreeRoot);

        freeExpressionNodeData(subTreeRoot, false, 1);
        free(subTreeRoot);

        LPRINTF("after removing zero multiply subTreeRoot = %p", newNode);

        static size_t removeNeutralCaseZeroMulCount = 1;
        // logTree(derivative, "removeNeutral case multiply zero number %lu. CurSubTreeRoot = %p", removeNeutralCaseZeroMulCount, newNode);
        removeNeutralCaseZeroMulCount++;

        // logTree(derivative, "during removal of neutral mul 0");

        return newNode;
    }

    LPRINTF("subTreeRoot->left = %p, subTreeRoot->right = %p", _L(subTreeRoot), _R(subTreeRoot));
    treeNode_t* newRootLeft = removeLeftNeutralSubtree(derivative, subTreeRoot);
    if(newRootLeft){
        LPRINTF("*addrToAsignNewSubTree = %p", *addrToAsignNewSubTree);
        *addrToAsignNewSubTree = newRootLeft;
        subTreeRoot = newRootLeft;

    }

    treeNode_t* newRootRight = removeRightNeutralSubtree(derivative, subTreeRoot);
    if(newRootRight){
        LPRINTF("*addrToAsignNewSubTree = %p", *addrToAsignNewSubTree);
        *addrToAsignNewSubTree = newRootRight;
        subTreeRoot = newRootRight;

    }

    return subTreeRoot;
}

static treeNode_t* removeNeutralSubtree(tree_t* derivative, treeNode_t* subTreeRoot, treeNode_t* remainSubTreeRoot){
    assert(derivative);
    assert(subTreeRoot);
    assert(remainSubTreeRoot);

    if(!_PAR(subTreeRoot)){
        return NULL;
    }

    if(_NODE_TYPE(subTreeRoot) != NUMBER){
        return NULL;
    }

    // LPRINTF("subTreeRoot = %p, subTreeRoot->parent->data.op = %s", subTreeRoot, subTreeRoot->parent->data.num);
    // LPRINTF("subTreeRoot->parent = %p, subTreeRoot->parent->data.op = %s", subTreeRoot->parent->data.op);

    // LPRINTF("Проверка на то, есть ли умножение на 1 или плюсование 0. subTreeRoot->data.num = %d, subTreeRoot->parent->data.op[0] == '%c', subTreeRoot = %p, subTreeRoot->parent->right = %p", subTreeRoot->data.num, subTreeRoot->parent->data.op[0], subTreeRoot, subTreeRoot->parent->right);
    if((_NODE_TYPE(_PAR(subTreeRoot)) == MUL && _NODE_VALUE_NUM(_PAR(subTreeRoot)) == 1)  || 
        ((_NODE_TYPE(_PAR(subTreeRoot)) == ADD || _NODE_TYPE(_PAR(subTreeRoot)) == SUB) && _NODE_VALUE_NUM(subTreeRoot) == 0) ||
        (_NODE_TYPE(_PAR(subTreeRoot)) == DIVIDE && _NODE_VALUE_NUM(subTreeRoot) == 1 && _R(_PAR(subTreeRoot)) == subTreeRoot)){
        LPRINTF("удаляем поддерево с корнем %p", subTreeRoot);

        if(_PAR(_PAR(subTreeRoot))){
            if(_L(_PAR(_PAR(subTreeRoot))) == _PAR(subTreeRoot)){
                _L(_PAR(_PAR(subTreeRoot))) = remainSubTreeRoot;
                _PAR(remainSubTreeRoot) = _PAR(_PAR(subTreeRoot));
            }
            else{
                _R(_PAR(_PAR(subTreeRoot))) = remainSubTreeRoot;
                _PAR(remainSubTreeRoot) = _PAR(_PAR(subTreeRoot));
            }
        }
        else{
            derivative->root = remainSubTreeRoot;
            _PAR(remainSubTreeRoot) = NULL;
        }

        return remainSubTreeRoot;
    }   
    
    return NULL;
}

static treeNode_t* removeLeftNeutralSubtree(tree_t* derivative, treeNode_t* subTreeRoot){
    assert(subTreeRoot);

    if(_L(subTreeRoot) && _R(subTreeRoot)){
        LPRINTF("subTreeRoot->left = %p, subTreeRoot->right = %p", _L(subTreeRoot), _R(subTreeRoot));
        if(removeNeutralSubtree(derivative, _L(subTreeRoot), _R(subTreeRoot))){
            treeNode_t* result = _R(subTreeRoot);
            LPRINTF("during optimization freeing leftSubtree");
            // logTree(derivative, "during optimization freeing leftSubtree of %p", subTreeRoot);

            freeLeftSubtree(subTreeRoot, false);
            freeExpressionNodeData(subTreeRoot, false, 1);
            free(subTreeRoot);

            return result;
        }
    }
    return NULL;
}

static treeNode_t* removeRightNeutralSubtree(tree_t* derivative, treeNode_t* subTreeRoot){
    assert(subTreeRoot);

    if(_L(subTreeRoot) && _R(subTreeRoot)){
        if(removeNeutralSubtree(derivative, _R(subTreeRoot), _L(subTreeRoot))){
            treeNode_t* result = _L(subTreeRoot);
            LPRINTF("during optimization freeing RightSubtree of %p", subTreeRoot);
            // logTree(derivative, "during optimization freeing RightSubtree of %p", subTreeRoot);

            freeRightSubtree(subTreeRoot, false);
            freeExpressionNodeData(subTreeRoot, false, 1);
            free(subTreeRoot);

            return result;
        }
    }

    return NULL;
}

static double calculateSubTree(treeNode_t* subTreeRoot){
    assert(subTreeRoot);

    if(_NODE_TYPE(subTreeRoot) != NUMBER){
        for(size_t curOper = 0; curOper < sizeof(operations) / sizeof(operation_t); curOper++){
            if(operations[curOper].type == _NODE_TYPE(subTreeRoot)){
                double* params = (double*) calloc(operations[curOper].paramCount, sizeof(double));
                assert(params);

                switch(operations[curOper].paramCount){
                    case 1: params[0] = calculateSubTree(_L(subTreeRoot)); break;
                    case 2: params[0] = calculateSubTree(_L(subTreeRoot)); params[1] = calculateSubTree(_R(subTreeRoot)); break;
                    default: break;
                }
                
                double result = operations[curOper].calcHandler(params);
                LPRINTF("result = %d", result);

                free(params);
                return result;
            }
        }
    }
    else{
        return _NODE_VALUE_NUM(subTreeRoot);
    }

    LPRINTF("calculation failure");
    return 0;
}