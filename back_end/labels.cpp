// #include "labels.h"

// #include "../core/DSL.h"

// #include "../general/debug.h"
// #include "../general/strFunc.h"

// #include <malloc.h>
// #include <assert.h>
// #include <string.h>

// const char* LABEL_PREFIX_JBE_WHILE_END = "whileEndJump";
// const char* LABEL_PREFIX_WHILE_BEGIN   = "whileBegin";
// const char* LABEL_PREFIX_IF_FALSE_JMP  = "ifFalseJmp";

// static labelsTable_t* reallocateLabelsTable(labelsTable_t* labelsTable);

// labelsTable_t* labelsTableCtor(labelsTable_t* labelsTable){
//     labelsTable = (labelsTable_t*) calloc(1, sizeof(labelsTable_t));

//     labelsTable->data = (label_t*) calloc(1, sizeof(label_t));
//     assert(labelsTable->data);

//     labelsTable->capacity = 1;
//     labelsTable->size = 0;

//     return labelsTable;
// }

// labelsTable_t* labelsTableDtor(labelsTable_t* labelsTable){
//     assert(labelsTable);

//     for(size_t curLabelInd = 0; curLabelInd < labelsTable->size; curLabelInd++){
//         free(_LABEL_DATA_PREFIX(&labelsTable->data[curLabelInd]));
//     }

//     free(_LABEL_TABLE_DATA(labelsTable));

//     free(labelsTable);

//     return NULL;
// }

// label_t createLabel(labelsTable_t* labelsTable, const char* name){
//     assert(labelsTable);
//     assert(name);

//     if(_LABEL_TABLE_SIZE(labelsTable) >= _LABEL_TABLE_CAPACITY(labelsTable)){
//         reallocateLabelsTable(labelsTable);
//     }

//     int num = _LABEL_TABLE_SIZE(labelsTable);

//     size_t curSize = _LABEL_TABLE_SIZE(labelsTable);

//     LPRINTF("WRITE slot addr=%p", &labelsTable->data[curSize]);

//     _LABEL_DATA_PREFIX(&labelsTable->data[curSize])  = myStrDup(name);
//     _LABEL_DATA_ID(&labelsTable->data[curSize]) = num;

//     _LABEL_TABLE_SIZE(labelsTable)++;

//     return labelsTable->data[curSize];
// }

// static labelsTable_t* reallocateLabelsTable(labelsTable_t* labelsTable){
//     assert(labelsTable);

//     LPRINTF("start reallocation");

//     size_t startSize = _LABEL_TABLE_CAPACITY(labelsTable);

//     label_t* temp = (label_t*) realloc(_LABEL_TABLE_DATA(labelsTable), _LABEL_TABLE_CAPACITY(labelsTable) * 2 * sizeof(label_t));
//     assert(temp);

//     _LABEL_TABLE_DATA(labelsTable) = temp;
//     _LABEL_TABLE_CAPACITY(labelsTable) *= 2;

//     LPRINTF("ended reallocation");

//     return labelsTable;
// }



