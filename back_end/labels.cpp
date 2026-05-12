#include "labels.h"

#include "general/debug.h"
#include "general/poison.h"
#include "core/DSL.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

const size_t MAX_LABEL_NAME_LEN = 64;

const char* LABEL_PREFIX_WHILE_BEGIN = "whileBegin";
const char* LABEL_PREFIX_WHILE_END   = "whileEnd";
const char* LABEL_PREFIX_IF_END      = "ifEnd";
const char* LABEL_PREFIX_FUNC        = "func";
const char* LABEL_PREFIX_READ        = "read";
const char* LABEL_PREFIX_READ_END    = "readEnd";
const char* LABEL_PREFIX_WRITE       = "write";

size_t labelNum = 0;

label_t* createLabel(labelsTable_t* labelsTable, const char* name, int id, bool saveLabel){
    assert(labelsTable);
    assert(name);

    label_t* curLabel = (label_t*) calloc(1, sizeof(label_t));
    assert(curLabel);

    _LABEL_DATA_NAME(curLabel) = (char*) calloc(MAX_LABEL_NAME_LEN, sizeof(char));
    assert(_LABEL_DATA_NAME(curLabel));

    LPRINTF("name = %s\n", name);

    strcpy(_LABEL_DATA_NAME(curLabel), name);

     _LABEL_DATA_ID(curLabel) = labelNum;
    // if(id == 0) _LABEL_DATA_ID(curLabel) = *freeInd(labelsTable);
    // else _LABEL_DATA_ID(curLabel) = id;
    
    LPRINTF("curLabelAddr = %p", curLabel);
    if(saveLabel) listInsertToTail(labelsTable, (void*) curLabel);

    labelDtor(curLabel);

    labelNum++;

    return (label_t*) *data(labelsTable, *tail(labelsTable));
}

void* labelCopy(void* dest, void* src){
    if(dest == NULL || src == NULL) return NULL;

    label_t* destLabel = (label_t*) calloc(1, sizeof(label_t));
    assert(destLabel);

    label_t* srcLabel  = (label_t*) src;

    _LABEL_DATA_ID(destLabel) = _LABEL_DATA_ID(srcLabel);
    _LABEL_DATA_NAME(destLabel) = (char*) calloc(MAX_LABEL_NAME_LEN, sizeof(char));
    strcpy(_LABEL_DATA_NAME(destLabel), _LABEL_DATA_NAME(srcLabel));

    return (void*) destLabel;
}

label_t* labelFind(labelsTable_t* labelsTable, listCmpFunc_t findRule, label_t* refElem){
    assert(labelsTable);

    listCmpFunc_t saveCmp = labelsTable->cmpFunc;
    
    labelsTable->cmpFunc = findRule;

    int result = 0;
    listFind(labelsTable, (void**) refElem, &result);

    if(result == SEARCH_NOT_FOUND_VALUE) return NULL;

    labelsTable->cmpFunc = saveCmp;

    return (label_t*) labelsTable->elem[result].data;
}

int labelCmp(void* a, void* b){
    if(a == NULL && b == NULL) return 0;
    if(a == NULL || b == NULL) return 1;

    label_t* labelA  = (label_t*) a;
    label_t* labelB  = (label_t*) b;

    int result = 1;

    if(!strcmp(_LABEL_DATA_NAME(labelA), _LABEL_DATA_NAME(labelB)) &&
        _LABEL_DATA_ID(labelA) == _LABEL_DATA_ID(labelB)){
        result = 0;
    }

    return result;
}

void labelDtor(void* ptr){
    label_t* label = (label_t*) ptr;

    free(_LABEL_DATA_NAME(label));

    poisonMemory(label, sizeof(label));
    free(label);
}
