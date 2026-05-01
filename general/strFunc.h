#ifndef STR_FUNC_H
#define STR_FUNC_H

#include <stddef.h>
#include <stdio.h>

const size_t         MAX_ANSWER_SIZE = 256;

struct string {
    char* ptr;
    size_t len;
};

struct buffer_t{
    int* ptr;
    size_t amountNodes;
};

const int UPPER_SYM_MIN        = (int) 'A'; 
const int UPPER_SYM_MAX        = 'Z'; 
const int UPPER_TO_LOWER_SHIFT = 'A' - 'a'; 
const int SIZE_OF_END_FILE     = 1; 

const char END_STR             = '\n';

size_t countStrings(char* buf, size_t fileSize, char endStr);
bool isEqualStrings(const char* str1, const char* str2);
size_t myStrLen(const char* start, char endStr ='\0');
char* myStrCpy(char* dest, const char* src);
char* myStrDup(const char *str);
char* myFGets(char* str, int count, FILE* stream);
void clearBuffer();
void skipSpaces(char** buffer);

bool isYes(char* answer);
bool isNo(char* answer);

size_t utf8StrLen(char* str);
void utf8Shift(int amountSymShift, char** pos);
size_t getUtf8CharLength(char c);
void skipWhitespace(char** bufferPos);
bool isApprovedVariableSym(const char* str);
bool isSpecialVarSymUtf8(const char* str);
bool isalphaUtf8(const char* str);

char* transliterate(char* str);

int compareStrings(void* a, void* b);
void* copyString(void* a, void* b);

#endif /* STR_FUNC_H */