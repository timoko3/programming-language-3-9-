#include "labels.h"

#include <malloc.h>
#include <assert.h>

labelsTable_t* labelsTableCtor(labelsTable_t* labelsTable){
    labelsTable = (labelsTable_t*) calloc(1, sizeof(labelsTable_t));

    labelsTable->data = (label_t*) calloc(1, sizeof(label_t));
    assert(labelsTable->data);

    labelsTable->capacity = 1;
    labelsTable->size = 0;

    return labelsTable;
}