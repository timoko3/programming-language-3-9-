#ifndef SPU_NAMES_H
#define SPU_NAMES_H

#include "../general/nametable.h"

const size_t global

enum variableScope{
    LOCALE,
    GLOBAL
};

struct spuName_t{
    char*         str;
    nameType      type;
    int           addr;
    variableScope scope;
};

struct spuNameTable_t{
    spuName_t* data;
    size_t     size;
    size_t     capacity;
};

spuNameTable_t* spuNameTableCtor(spuNameTable_t* spuNameTable);
spuNameTable_t* spuNameTableDtor(spuNameTable_t* spuNameTable);
int getGlobalVarAddr(spuNameTable_t* spuNameTable, const char* name);
int getLocalVarAddr(spuNameTable_t* spuNameTable, const char* name, size_t* stackFrameOffset);

#endif  /* SPU_NAMES_H */