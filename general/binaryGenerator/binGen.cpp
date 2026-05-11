#include "binGen.h"

#include <string.h>
#include <assert.h>
#include <malloc.h>

binBuffer_t* binBufferCtor(size_t startCapacity){

    binBuffer_t* buffer = (binBuffer_t*) calloc(1, sizeof(binBuffer_t));
    assert(buffer);

    BIN_BUFFER_DATA(buffer)     = (uint8_t*) calloc(startCapacity, sizeof(uint8_t));
    BIN_BUFFER_CAPACITY(buffer) = startCapacity;

    return buffer;
}

void binBufferDtor(binBuffer_t* buffer){
    free(BIN_BUFFER_DATA(buffer));
    free(buffer);
}

void writeU32LeBuf(binBuffer_t* buffer, uint32_t x){
    uint8_t byte[4];

    byte[0] = x;
    byte[1] = x >> 8;
    byte[2] = x >> 16;
    byte[3] = x >> 24;

    for(size_t i = 0; i < 4; i++){
        writeU8Buf(buffer, byte[i]);
    }
}

void writeU8Buf(binBuffer_t* buffer, uint8_t x){
    assert(buffer);

    BIN_BUFFER_DATA(buffer)[BIN_BUFFER_SIZE(buffer)] = x;
    BIN_BUFFER_SIZE(buffer)++;

    if(BIN_BUFFER_SIZE(buffer) >= BIN_BUFFER_CAPACITY(buffer)) binBufferRealloc(buffer);
}

void binBufferRealloc(binBuffer_t* buffer){
    assert(buffer);

    BIN_BUFFER_CAPACITY(buffer) *= 2;

    uint8_t* newBufferData = (uint8_t*) realloc(BIN_BUFFER_DATA(buffer), BIN_BUFFER_CAPACITY(buffer) * sizeof(uint8_t));
    assert(newBufferData);
    
    BIN_BUFFER_DATA(buffer) = newBufferData;
}

// fixUpBinBufElem_t* fixUpListInit(fixUpBinBufElem_t* fixUpList){
//     listCtor(fixUpList, 3, fixUpElemCmp, varMapCopy);

//     regTableElem_t* refReg = regTableElemCtor(NONE, "", TEMP_STORE, 0);
//     assert(refReg);
//     regTableElem_t* foundReg = regTableFind(regTable, findTypeRegFree, refReg);        
//     assert(foundReg);

//     varMapElem_t* tempVar = varMapElemCtor(TEMP_VARIABLE_CODE, LOCK_REG, foundReg);
//     listInsertToTail(varMap, tempVar);

//     varMapElemDtor(tempVar);
//     regTableElemDtor(refReg);

//     return varMap;
// }

fixUpBinBufElem_t* fixUpElemCtor(char* label, uint32_t labelOffset){
    assert(label);

    fixUpBinBufElem_t* elem = (fixUpBinBufElem_t*) calloc(1, sizeof(fixUpBinBufElem_t));
    assert(elem);

    FIX_UP_ELEMENT_LABEL_OFFSET(elem) = labelOffset;

    FIX_UP_ELEMENT_LABEL(elem) = (char*) calloc(strlen(label) + 1, sizeof(char));
    assert(FIX_UP_ELEMENT_LABEL(elem));

    strcpy(FIX_UP_ELEMENT_LABEL(elem), label);

    listCtor(FIX_UP_ELEMENT_PATCH_OFFSETS(elem), 3, uInt32Cmp, uInt32Copy);

    return elem;
}

void* fixUpElemCopy(void* dest, void* src){
    if(dest == NULL || src == NULL) return NULL;

    fixUpBinBufElem_t* destRegT = (fixUpBinBufElem_t*) calloc(1, sizeof(fixUpBinBufElem_t));
    assert(destRegT);

    fixUpBinBufElem_t* srcRegT  = (fixUpBinBufElem_t*) src;

    FIX_UP_ELEMENT_LABEL_OFFSET(destRegT) = FIX_UP_ELEMENT_LABEL_OFFSET(srcRegT);

    FIX_UP_ELEMENT_LABEL(destRegT) = (char*) calloc(strlen(FIX_UP_ELEMENT_LABEL(srcRegT)) + 1, sizeof(char));
    assert(FIX_UP_ELEMENT_LABEL(destRegT));

    strcpy(FIX_UP_ELEMENT_LABEL(destRegT), FIX_UP_ELEMENT_LABEL(srcRegT));

    listCtor(FIX_UP_ELEMENT_PATCH_OFFSETS(destRegT), 3, uInt32Cmp, uInt32Copy);

    listCopy(FIX_UP_ELEMENT_PATCH_OFFSETS(destRegT), FIX_UP_ELEMENT_PATCH_OFFSETS(srcRegT));

    return (void*) destRegT;
}

int fixUpElemCmp(void* a, void* b){
    if(a == NULL && b == NULL) return 0;
    if(a == NULL || b == NULL) return 1;

    fixUpBinBufElem_t* fixUpElemA  = (fixUpBinBufElem_t*) a;
    fixUpBinBufElem_t* fixUpElemB  = (fixUpBinBufElem_t*) b;

    int result = 1;

    if(!strcmp(FIX_UP_ELEMENT_LABEL(fixUpElemA), FIX_UP_ELEMENT_LABEL(fixUpElemB))){
        result = 0;
    }

    return result;
}

void fixUpElemDtor(fixUpBinBufElem_t* elem){
    if(elem == NULL) return;

    free(FIX_UP_ELEMENT_LABEL(elem));

    listDtor(FIX_UP_ELEMENT_PATCH_OFFSETS(elem));

    free(elem);
}

int uInt32Cmp(void* a, void* b){
    if(a == NULL && b == NULL) return 0;
    if(a == NULL || b == NULL) return 1;

    uint32_t* fixUpElemA  = (uint32_t*) a;
    uint32_t* fixUpElemB  = (uint32_t*) b;

    int result = 1;

    if(*fixUpElemA == *fixUpElemB){
        result = 0;
    }

    return result;
}

void* uInt32Copy(void* dest, void* src){
    if(dest == NULL || src == NULL) return NULL;

    uint32_t* destNum = (uint32_t*) calloc(1, sizeof(uint32_t));
    assert(destNum);

    uint32_t* srcNum  = (uint32_t*) src;

    *destNum = *srcNum;

    return (void*) destNum;
}