#include "expressionTree.h"
#include "DSL.h"

#include "../general/debug.h"

#include <assert.h>
#include <malloc.h>

bool checkSubTreeCountable(treeNode_t* curNode){
    assert(curNode);

    if(_NODE_TYPE(curNode) != NUMBER &&
       _NODE_TYPE(curNode) != ADD    &&
       _NODE_TYPE(curNode) != SUB    &&
       _NODE_TYPE(curNode) != MUL    &&
       _NODE_TYPE(curNode) != DIVIDE){
        return false;
    }

    if(curNode->left){
        if(!checkSubTreeCountable(curNode->left)) return false;
    }
    if(curNode->right){
        if(!checkSubTreeCountable(curNode->right)) return false;
    }

    return true;
}

bool freeExpressionNodeData(treeNode_t* node, bool withoutRoot, int depth){
    assert(node);
    
    LPRINTF("entered freeExpressionNodeData func with node %p, withoutRoot = %d, depth = %d", node, withoutRoot, depth);

    if(!withoutRoot || (withoutRoot && depth != 1)){
        if(_NODE_TYPE(node) != NUMBER){
            free(_NODE_VALUE_STR(node));
        }
        free(_NODE_WRITE_FILE(node));
        return true;
    }

    return false;
}


