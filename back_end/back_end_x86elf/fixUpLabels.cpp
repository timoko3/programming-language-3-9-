#include "fixUpLabels.h"

#include <assert.h>
#include <string.h>
#include <malloc.h>

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

void codeBufFixUp(binBuffer_t* binBuf, list_t* fixUpList){
    assert(binBuf);
    assert(fixUpList);

    int curFixUpElem = *head(fixUpList);

    for(size_t i = 0; i < fixUpList->size; i++){
        fixUpBinBufElem_t* fixUpElem = (fixUpBinBufElem_t*) *data(fixUpList, curFixUpElem);

        int curPatchOffsetElem = *head(FIX_UP_ELEMENT_PATCH_OFFSETS(fixUpElem));

        for(size_t j = 0; j < FIX_UP_ELEMENT_PATCH_OFFSETS(fixUpElem)->size; j++){
            uint32_t* patchOffset = (uint32_t*) *data(FIX_UP_ELEMENT_PATCH_OFFSETS(fixUpElem), curPatchOffsetElem);

            binBufPatch(binBuf, *patchOffset - sizeof(uint32_t), FIX_UP_ELEMENT_LABEL_OFFSET(fixUpElem) - *patchOffset);

            curPatchOffsetElem = *next(FIX_UP_ELEMENT_PATCH_OFFSETS(fixUpElem), curPatchOffsetElem);
        }

        curFixUpElem = *next(fixUpList, curFixUpElem);
    }
}

fixUpBinBufElem_t* addFixUpElem(list_t* fixUpElements, char* label){
    assert(fixUpElements);
    assert(label);

    fixUpBinBufElem_t* foundElem = findFixUpElem(fixUpElements, label);

    if(foundElem){
        return foundElem;
    }

    fixUpBinBufElem_t* newElem = fixUpElemCtor(label);
    assert(newElem);

    listInsertToTail(fixUpElements, newElem);

    fixUpElemDtor(newElem);

    return (fixUpBinBufElem_t*) *data(fixUpElements, *tail(fixUpElements));
}

fixUpBinBufElem_t* findFixUpElem(list_t* fixUpElements, char* label){
    assert(fixUpElements);
    assert(label);

    int curElem = fixUpElements->elem->next;

    for(size_t i = 0; i < fixUpElements->size; i++){
        fixUpBinBufElem_t* curFixUpElem = (fixUpBinBufElem_t*) fixUpElements->elem[curElem].data;

        if(curFixUpElem != NULL){
            if(!strcmp(FIX_UP_ELEMENT_LABEL(curFixUpElem), label)){
                return curFixUpElem;
            }
        }

        curElem = fixUpElements->elem[curElem].next;
    }

    return NULL;
}

fixUpBinBufElem_t* fixUpListAddPatchOffset(list_t* fixUpElements, char* label, uint32_t newPatchOffset){
    assert(fixUpElements);
    assert(label);

    fixUpBinBufElem_t* fixUpElem = addFixUpElem(fixUpElements, label);
    assert(fixUpElem);

    fixUpElemAddPatchOffset(fixUpElem, newPatchOffset);

    return fixUpElem;
}

void fixUpSetLabelOffset(list_t* fixUpElements, char* label, uint32_t labelOffset){
    assert(fixUpElements);
    assert(label);

    fixUpBinBufElem_t* fixUpElem = addFixUpElem(fixUpElements, label);
    assert(fixUpElem);

    FIX_UP_ELEMENT_LABEL_OFFSET(fixUpElem) = labelOffset;
}

listStatus fixUpElemAddPatchOffset(fixUpBinBufElem_t* elem, uint32_t newPatchOffset){
    assert(elem);

    int curElem = FIX_UP_ELEMENT_PATCH_OFFSETS(elem)->elem->next;

    for(size_t i = 0; i < FIX_UP_ELEMENT_PATCH_OFFSETS(elem)->size; i++){
        uint32_t* curPatchOffset = (uint32_t*) FIX_UP_ELEMENT_PATCH_OFFSETS(elem)->elem[curElem].data;

        if((curPatchOffset != NULL) && (*curPatchOffset == newPatchOffset)){
            return PROCESS_OK_LIST;
        }

        curElem = FIX_UP_ELEMENT_PATCH_OFFSETS(elem)->elem[curElem].next;
    }

    uint32_t patchOffset = newPatchOffset;

    listInsertToTail(FIX_UP_ELEMENT_PATCH_OFFSETS(elem), &patchOffset);

    return PROCESS_OK_LIST;
}

fixUpBinBufElem_t* fixUpElemCtor(char* label){
    assert(label);

    fixUpBinBufElem_t* elem = (fixUpBinBufElem_t*) calloc(1, sizeof(fixUpBinBufElem_t));
    assert(elem);

    FIX_UP_ELEMENT_LABEL(elem) = (char*) calloc(strlen(label) + 1, sizeof(char));
    assert(FIX_UP_ELEMENT_LABEL(elem));

    strcpy(FIX_UP_ELEMENT_LABEL(elem), label);

    FIX_UP_ELEMENT_PATCH_OFFSETS(elem) = (list_t*) calloc(1, sizeof(list_t));
    assert(FIX_UP_ELEMENT_PATCH_OFFSETS(elem));

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

    FIX_UP_ELEMENT_PATCH_OFFSETS(destRegT) = (list_t*) calloc(1, sizeof(list_t));
    assert(FIX_UP_ELEMENT_PATCH_OFFSETS(destRegT));

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
    
    listDtor(FIX_UP_ELEMENT_PATCH_OFFSETS(elem), (listFreeDataFunc_t) uInt32Dtor);
    free(FIX_UP_ELEMENT_PATCH_OFFSETS(elem));

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

void uInt32Dtor(uint32_t* num){
    free(num);
}