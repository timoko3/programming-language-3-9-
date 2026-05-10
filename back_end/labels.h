#ifndef LABELS_H
#define LABELS_H

#include <stddef.h>

#include "general/cashFriendlyList/list.h"

extern const char* LABEL_PREFIX_WHILE_BEGIN;
extern const char* LABEL_PREFIX_WHILE_END;
extern const char* LABEL_PREFIX_IF_END; 

const size_t AMOUNT_LABELS    = 5;

struct label_t{
    char* name;
    int id;
};

typedef list_t labelsTable_t;

// label_t* getLabel(labelsTable_t* labelsTable, const char* name);
label_t* createLabel(labelsTable_t* labelsTable, const char* name,  int id = 0, bool saveLabel = 1);
label_t* labelFind(labelsTable_t* labelsTable, listCmpFunc_t findRule, label_t* refElem);

void* labelCopy(void* dest, void* src);
int labelCmp(void* a, void* b);
void labelDtor(void* ptr);

#endif /* LABELS_H */