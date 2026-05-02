#include "format.h"

#include "core/DSL.h"

#include <assert.h>

void emitTabs(codeGenContext* context){
    assert(context);

    for(int i = 0; i < _CONTEXT_BLOCK_IM_DEPTH(context); i++) {
        fputc('\t', _CONTEXT_FILE_PTR(context));
    }
}