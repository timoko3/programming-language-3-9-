#ifndef FIX_UP_LABELS_H
#define FIX_UP_LABELS_H

#include <stdio.h>
#include <stdint.h>

#include "general/binaryGenerator/binGen.h"
#include "cashFriendlyList/list.h"

#define FIX_UP_ELEMENT_LABEL(fixUp)          fixUp->label
#define FIX_UP_ELEMENT_LABEL_OFFSET(fixUp)   fixUp->labelOffset
#define FIX_UP_ELEMENT_PATCH_OFFSETS(fixUp)  fixUp->patchOffsets

struct fixUpBinBufElem_t{
    list_t*  patchOffsets;
    uint32_t labelOffset;
    char*    label;
};

int uInt32Cmp(void* a, void* b);
void* uInt32Copy(void* dest, void* src);
void uInt32Dtor(uint32_t* num);

void codeBufFixUp(binBuffer_t* binBuf, list_t* fixUpList);

fixUpBinBufElem_t* addFixUpElem(list_t* fixUpElements, char* label);
fixUpBinBufElem_t* findFixUpElem(list_t* fixUpElements, char* label);

fixUpBinBufElem_t* fixUpListAddPatchOffset(list_t* fixUpElements, char* label, uint32_t newPatchOffset);
listStatus fixUpElemAddPatchOffset(fixUpBinBufElem_t* elem, uint32_t newPatchOffset);

void fixUpSetLabelOffset(list_t* fixUpElements, char* label, uint32_t labelOffset);

fixUpBinBufElem_t* fixUpElemCtor(char* label);

list_t* fixUpListInit(list_t* fixUpList, uint32_t currentPatchOffset);
int fixUpElemCmp(void* a, void* b);
void* fixUpElemCopy(void* dest, void* src);

void fixUpElemDtor(fixUpBinBufElem_t* elem);

#endif /* FIX_UP_LABELS_H */