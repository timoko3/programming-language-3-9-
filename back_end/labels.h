#ifndef LABELS_H
#define LABELS_H

#include <stddef.h>

extern const char* LABEL_PREFIX_JBE_WHILE_END;
extern const char* LABEL_PREFIX_WHILE_BEGIN;
extern const char* LABEL_PREFIX_IF_FALSE_JMP; 

struct label_t{
    char* prefix;
    int id;
};

struct labelsTable_t{
    label_t* data;
    size_t   size;
    size_t   capacity;
};

labelsTable_t* labelsTableCtor(labelsTable_t* labelsTable);
// label_t* getLabel(labelsTable_t* labelsTable, const char* name);
label_t createLabel(labelsTable_t* labelsTable, const char* name);
labelsTable_t* labelsTableDtor(labelsTable_t* labelsTable);

#endif /* LABELS_H */