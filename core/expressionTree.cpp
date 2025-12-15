#include "expressionTree.h"
#include "DSL.h"

#include "../general/debug.h"

#include <assert.h>
#include <malloc.h>

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


