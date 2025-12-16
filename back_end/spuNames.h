#ifndef SPU_NAMES_H
#define SPU_NAMES_H

#include "../general/nametable.h"

struct spuName_t{
    char*        str;
    nameType     type;
    int          addr;
};

struct spuNameTable_t{
    spuName_t* data;
    size_t     size;
    size_t     capacity;
};

spuNameTable_t* spuNameTableCtor(spuNameTable_t* spuNameTable);
spuNameTable_t* spuNameTableDtor(spuNameTable_t* spuNameTable);
int getVarAddr(spuNameTable_t* spuNameTable, const char* name);

#endif  /* SPU_NAMES_H */