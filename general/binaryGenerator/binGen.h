#ifndef BIN_GEN_H
#define BIN_GEN_H

#include <stdio.h>
#include <stdint.h>

#include "cashFriendlyList/list.h"

#define BIN_BUFFER_SIZE(buffer)     buffer->size
#define BIN_BUFFER_CAPACITY(buffer) buffer->capacity
#define BIN_BUFFER_DATA(buffer)     buffer->data

#define FIX_UP_ELEMENT_LABEL(fixUp)          fixUp->label
#define FIX_UP_ELEMENT_LABEL_OFFSET(fixUp)   fixUp->labelOffset
#define FIX_UP_ELEMENT_PATCH_OFFSETS(fixUp)  fixUp->patchOffsets

struct fixUpBinBufElem_t{
    list_t*  patchOffsets;
    uint32_t labelOffset;
    char*    label;
};

struct binBuffer_t{
    uint8_t* data;
    size_t size;
    size_t capacity;
};

binBuffer_t* binBufferCtor(size_t startCapacity);
void binBufferDtor(binBuffer_t* buffer);

void writeU8Le(FILE* stream, uint8_t x);

void writeU32LeBuf(binBuffer_t* buffer, uint32_t x);
void writeU8Buf(binBuffer_t* buffer, uint8_t x);
void binBufferRealloc(binBuffer_t* buffer);

int uInt32Cmp(void* a, void* b);
void* uInt32Copy(void* dest, void* src);

list_t* fixUpListInit(list_t* fixUpList, uint32_t currentPatchOffset);
int fixUpElemCmp(void* a, void* b);

#endif /* BIN_GEN_H */