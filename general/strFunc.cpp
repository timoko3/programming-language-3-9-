#include "strFunc.h"
#include "hash.h"
#include "poison.h"
#include "debug.h"

#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

struct transliterationTable_t{
    char* russian;
    char* translit;
};

transliterationTable_t transliterationTable[]{
    {"а", "a"},
    {"б", "b"},
    {"в", "v"},
    {"г", "g"},
    {"д", "d"},
    {"е", "e"},
    {"ё", "yo"},
    {"ж", "zh"},
    {"з", "z"},
    {"и", "i"},
    {"й", "j"},
    {"к", "k"},
    {"л", "l"},
    {"м", "m"},
    {"н", "n"},
    {"о", "o"},
    {"п", "p"},
    {"р", "r"},
    {"с", "s"},
    {"т", "t"},
    {"у", "u"},
    {"ф", "f"},
    {"х", "h"},
    {"ц", "c"},
    {"ч", "ch"},
    {"ш", "sh"},
    {"щ", "shch"},
    {"ь", ""},
    {"ъ", ""},
    {"ы", "y"},
    {"э", "e"},
    {"ю", "yu"},
    {"я", "ya"},
        
    {"А", "A"},
    {"Б", "B"},
    {"В", "V"},
    {"Г", "G"},
    {"Д", "D"},
    {"Е", "E"},
    {"Ё", "Yo"},
    {"Ж", "Zh"},
    {"З", "Z"},
    {"И", "I"},
    {"Й", "J"},
    {"К", "K"},
    {"Л", "L"},
    {"М", "M"},
    {"Н", "N"},
    {"О", "O"},
    {"П", "P"},
    {"Р", "R"},
    {"С", "S"},
    {"Т", "T"},
    {"У", "U"},
    {"Ф", "F"},
    {"Х", "H"},
    {"Ц", "C"},
    {"Ч", "Ch"},
    {"Ш", "Sh"},
    {"Щ", "Shch"},
    {"Ь", ""},
    {"Ъ", ""},
    {"Ы", "Y"},
    {"Э", "E"},
    {"Ю", "Yu"},
    {"Я", "Ya"},

};

const size_t SIZE_TRANSLITERATION_TABLE = sizeof(transliterationTable) / sizeof(transliterationTable[0]);

static size_t utf8Decode(const char* str, uint32_t* codePoint);
static bool isAlphaCodepoint(uint32_t c);
static bool isSpecialVarSymCodepoint(uint32_t c);

size_t myStrLen(const char* start, char endStr){
    assert(start);

    size_t curSymInd = 0;
    while(start[curSymInd] != endStr) curSymInd++;
    
    return curSymInd;
}

char* myStrCpy(char* dest, const char* src){
    assert(dest);
    assert(src);

    poisonMemory(dest, myStrLen(dest) + 1);

    int i = 0;

    while(src[i] != '\0'){
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';

    return dest;
}

char* myStrDup(const char *str){
    assert(str);

    char* newStr = (char*)calloc(myStrLen(str) + 1, sizeof(char)); 
    assert(newStr);
    
    myStrCpy(newStr, str);

    return newStr;
}

char* myFGets(char* str, int count, FILE* stream){
    assert(str);
    assert(stream);
    if(count < 1)return NULL;

    int i = 0;
    while(i < count) {
        str[i] = (char)fgetc(stream);
        LPRINTF("Код текущего символа: %d\n", str[i]);
        if(str[i] == '\n'){
            break;
        }

        i++;
    }
    str[i] = '\0';

    return str;
}

bool isEqualStrings(const char* str1, const char* str2){
    // LPRINTF("hash answer %str, длина %lu: %ld, hash %str, длина %lu: %lu\n", str1, myStrLen(str1) + 1, hash(str1, myStrLen(str1) + 1), str2, myStrLen(str2) + 1, hash(str2, myStrLen(str2) + 1));
    return hash(str1, myStrLen(str1) + 1) == hash(str2, myStrLen(str2) + 1);
}

size_t countStrings(char* buf, size_t fileSize, char endStr){
    assert(buf);

    size_t nStrings = 1;

    size_t curSym = 0;
    while(curSym < fileSize){
        if(buf[curSym] == endStr){
            nStrings++;
        }
        curSym++;
    }

    return nStrings;
}

void clearBuffer(){
    while(getchar() != '\n'){
        continue;
    }
}

void skipSpaces(char** buffer){
    assert(buffer);

    LPRINTF("entered skipSpaces func");

    while(isspace(**buffer)){
        (*buffer)++;
    }
}

bool isYes(char* answer){
    assert(answer);

    return isEqualStrings(answer, "да");
}

bool isNo(char* answer){
    assert(answer);

    return isEqualStrings(answer, "нет");
}

char* transliterate(char* str){
    assert(str);

    char* resultStr = (char*) calloc(4 * utf8StrLen(str), sizeof(char));
    assert(resultStr);

    char* curStrPos = str;
    while(*curStrPos != '\0'){
        bool found = false;
        for(size_t curLetterInd = 0; curLetterInd < SIZE_TRANSLITERATION_TABLE; curLetterInd++){
            if(!strncmp(curStrPos, transliterationTable[curLetterInd].russian, myStrLen(transliterationTable->russian))){

                strcat(resultStr, transliterationTable[curLetterInd].translit);
                found = true;
                LPRINTF("curStrPos: %s ", curStrPos);
            }
        }
        if(!found){
            size_t symLen = getUtf8CharLength(*curStrPos);
            strncat(resultStr, curStrPos, symLen);
        }   

        utf8Shift(1, &curStrPos);
    }

    return resultStr;
}

void utf8Shift(int amountSymShift, char** pos){
    assert(pos);

    for(size_t symSkipped = 0; symSkipped < (size_t) amountSymShift; symSkipped++){
        *pos += getUtf8CharLength(**pos);
    }
}

size_t utf8StrLen(char* str){
    assert(str);

    LPRINTF("started len");

    size_t len = 0;

    char* curPos = str;
    while(*curPos != '\0'){
        LPRINTF("count len: %lu, curStrPos = %s", len, curPos);

        len += getUtf8CharLength(*curPos);

        utf8Shift(1, &curPos);
    }

    LPRINTF("ended len");

    return len;
}

int getUtf8CharLength(char c){
    if     ((c & 0x80) == 0x00) return 1;
    else if((c & 0xE0) == 0xC0) return 2;
    else if((c & 0xF0) == 0xE0) return 3;
    else if((c & 0xF8) == 0xF0) return 4;
    return -1;
}

void skipWhitespace(char** bufferPos){
    while(isspace(**bufferPos)){
        utf8Shift(1, bufferPos); 
    }
}

bool isApprovedVariableSym(const char* str){
    assert(str);

    if(isalphaUtf8(str) || isSpecialVarSymUtf8(str)){
        return true;
    }

    return false;
}

bool isalphaUtf8(const char* str){
    assert(str);

    uint32_t codePoint;
    size_t len = utf8Decode(str, &codePoint);

    if(len == 0) {
        return false;
    }

    return isAlphaCodepoint(codePoint);
}

bool isSpecialVarSymUtf8(const char* str){
    assert(str);

    uint32_t codePoint;
    size_t len = utf8Decode(str, &codePoint);

    if(len == 0) {
        return false;
    }

    return isSpecialVarSymCodepoint(codePoint);
}

static size_t utf8Decode(const char* str, uint32_t* codePoint){
    assert(str);

    unsigned char byte0 = str[0];

    if(byte0 < 0x80) {
        *codePoint = byte0;
        return 1;
    }

    if((byte0 & 0xE0) == 0xC0 &&
        (str[1] & 0xC0) == 0x80) {

        *codePoint = ((byte0 & 0x1F) << 6) |
              (str[1] & 0x3F);
        return 2;
    }

    if((byte0 & 0xF0) == 0xE0 &&
        (str[1] & 0xC0) == 0x80 &&
        (str[2] & 0xC0) == 0x80) {

        *codePoint = ((byte0 & 0x0F) << 12) |
              ((str[1] & 0x3F) << 6) |
              (str[2] & 0x3F);
        return 3;
    }

    if((byte0 & 0xF8) == 0xF0 &&
        (str[1] & 0xC0) == 0x80 &&
        (str[2] & 0xC0) == 0x80 &&
        (str[3] & 0xC0) == 0x80) {

        *codePoint = ((byte0 & 0x07) << 18) |
              ((str[1] & 0x3F) << 12) |
              ((str[2] & 0x3F) << 6) |
              (str[3] & 0x3F);
        return 4;
    }

    return 0; 
}

static bool isAlphaCodepoint(uint32_t c){
    
    if((c >= 0x41 && c <= 0x5A) ||  
        (c >= 0x61 && c <= 0x7A))    
        return true;

    
    if((c >= 0x410 && c <= 0x42F) || 
        (c >= 0x430 && c <= 0x44F) || 
        (c == 0x401) ||               
        (c == 0x451))                 
        return true;

    return false;
}

static bool isSpecialVarSymCodepoint(uint32_t c){
    if(c == 0x2D){
        return true;
    }    

    return false;
}