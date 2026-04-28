// #ifndef NAME_TABLE_H
// #define NAME_TABLE_H

// #include "../general/stack/stack.h"

// #include <stddef.h>

// enum variableScope{
//     LOCALE,
//     GLOBAL
// };

// enum nameType{
//     VARIABLE,
//     FUNCTION
// };

// struct name_t{
//     char*    nameString; 
//     nameType type;
// };

// struct nametable_t{
//     name_t* data;
//     size_t size;
//     size_t capacity;
// };

// nametable_t* nameTableCtor(nametable_t* nametable);
// nametable_t* nameTableDtor(nametable_t* nametable);
// void initTokensSequence(nametable_t* nametable, size_t startInd);
// nametable_t* reallocateNameTable(nametable_t* nametable);

// void nameTableAddElem(stack* nametables, 
//                       char* stringName, 
//                       nameType type);

// bool checkExistsName(stack* nametables, char* name);

// void dumpNameTable(nametable_t* nameTable, FILE* stream);

// #endif /* NAME_TABLE_H */