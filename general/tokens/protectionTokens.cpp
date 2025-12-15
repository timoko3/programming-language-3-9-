
#include "protectionTokens.h"

#include "../general/debug.h"
#include "../general/file.h"

#include <assert.h>

void dumpTokenSequence(tokensSequence_t* tokenSequence){
    assert(tokenSequence);

    fileDescription logFile = {
        LOG_FILE_NAME,
        "ab+"
    };    

    FILE* logFilePtr = myOpenFile(&logFile);
    if(!logFilePtr) return;

    for(size_t curTokenInd = 0; curTokenInd < tokenSequence->size; curTokenInd++){
        if(tokenSequence->data[curTokenInd].type != NUMBER){
            fprintf(logFilePtr, "token with index %lu, data: %s, type: %s, addr: %p\n", curTokenInd, *tokenStrData(&tokenSequence->data[curTokenInd]), tokenTypeToStr(tokenSequence->data[curTokenInd].type), &tokenSequence->data[curTokenInd]);
        }
        else if(tokenSequence->data[curTokenInd].type == NUMBER){
            fprintf(logFilePtr, "token with index %lu, data: %d, type: %s, addr: %p\n", curTokenInd, *tokenNumData(&tokenSequence->data[curTokenInd]), tokenTypeToStr(tokenSequence->data[curTokenInd].type), &tokenSequence->data[curTokenInd]);
        }
    }
    
    fclose(logFilePtr);
}

const char* tokenTypeToStr(tokenType type){
    switch (type) {
        case MAIN:          return "MAIN";
        case START:         return "START";
        case CALL_FUNC:     return "CALL_FUNC";
        case INIT_VARIABLE: return "INIT_VARIABLE";
        case CALL_VARIABLE: return "CALL_VARIABLE";
        case NUMBER:        return "NUMBER";
        case NAME:          return "NAME";
        case ASSIGN:        return "ASSIGN";
        case EXPRESS:       return "EXPRESS";
        case BRACKL:        return "BRACKL";
        case BRACKR:        return "BRACKR";
        case ADD:           return "ADD";
        case SUB:           return "SUB";
        case MUL:           return "MUL";
        case MUL3:          return "MUL3";
        case DIVIDE:        return "DIVIDE";
        case END_STATEMENT: return "END_STATEMENT";
        case END_BLOCK:     return "END_BLOCK";
        case END_PROGRAM:   return "END_PROGRAM";
        case COMMA:         return "COMMA";
        case WHILE:         return "WHILE";
        case IF:            return "IF";
        case ELSE_IF:       return "ELSE_IF";
        case ELSE:          return "ELSE";
        case INIT_FUNC:     return "INIT_FUNC";
        case EQUAL:         return "EQUAL";
        case NOT_EQUAL:     return "NOT_EQUAL";
        case GE:            return "GE";
        case GT:            return "GT";
        case LE:            return "LE";
        case LT:            return "LT";
        case OUT:           return "OUT";
        case IN:            return "IN";
        case HLT:           return "HLT";
        default:            return "UNKNOWN";
    }
}