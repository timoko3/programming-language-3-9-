#include "binGen.h"

#include <string.h>

void writeU32Le(FILE* stream, uint32_t x){
    uint8_t byte[4];

    byte[0] = x;
    byte[1] = x >> 8;
    byte[2] = x >> 16;
    byte[3] = x >> 24;

    fwrite(byte, 1, 4, stream);
}

void writeU8Le(FILE* stream, uint8_t x){
    uint8_t byte = x;

    fwrite(&byte, 1, 1, stream);
}
