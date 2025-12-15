#include "treeExpImp.h"

#include "../core/DSL.h"
#include "../file.h"
#include "../debug.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

static treeNode_t* readNode(char** curBufferPos);
static char*       getNodeValue(char** curBufferPos);

static void skipSpaceAndCloseBracket(char** curBufferPos);
static void SyntaxError(const char* file, int line, const char* func);
static void expect(char** curBufferPos, char expected, const char* file, int line, const char* func);

#define SYNTAX_ERROR SyntaxError(__FILE__, __LINE__, __FUNCTION__)
#define EXPECT(curBufferPos, expected) expect(curBufferPos, expected, __FILE__, __LINE__, __FUNCTION__)

void treeWrite(tree_t* tree, const char* treeFileName){
    assert(tree);

    fileDescription treeFile{
        treeFileName,
        "wb"
    };

    FILE* treeFilePtr = myOpenFile(&treeFile);
    assert(treeFilePtr);

    printPreOrder(tree->root, treeFilePtr);

    fclose(treeFilePtr);
}

void treeRead(tree_t* tree, const char* treeFileName){
    assert(tree);
    assert(treeFileName);

    data_t treeData;
    parseStringsFile(&treeData, treeFileName);

    char* curBufferPos = treeData.buffer;

    tree->root = readNode(&curBufferPos);

    free(treeData.buffer);
    free(treeData.strings);
}

void printPreOrder(const treeNode_t* node, FILE* stream){
    assert(node);
    assert(stream);

    fprintf(stream, "(");
    fprintf(stream, " %s ", _NODE_WRITE_FILE(node));  /// можно менять
    
    if(node->left){
        printPreOrder(node->left, stream);
    }
    else{
        fprintf(stream, "nil ");
    }

    if(node->right){
        printPreOrder(node->right, stream);
    }
    else{
        fprintf(stream, "nil ");
    }

    fprintf(stream, ")");
}

static treeNode_t* readNode(char** curBufferPos){
    assert(curBufferPos);

    LPRINTF("entered readNode curBufferPos = %s", *curBufferPos);

    skipSpaceAndCloseBracket(curBufferPos);

    treeNode_t* newNode = NULL;
    if(**curBufferPos == '('){
        (*curBufferPos)++;

        skipSpaceAndCloseBracket(curBufferPos);

        LPRINTF("скобочка\n");

        char* nodeValue = getNodeValue(curBufferPos);

        newNode = createNewNodeStr(nodeValue, NULL, NULL);

        free(nodeValue);
    }
    else{
        if(strncmp(*curBufferPos, "nil", 3) == 0){
            LPRINTF("readNode case nil");
            *curBufferPos += 3;
            return NULL;
        }
    }

    _L(newNode)  = readNode(curBufferPos);

    _R(newNode)  = readNode(curBufferPos);

    return newNode;
}

static char* getNodeValue(char** curBufferPos){
    assert(curBufferPos);

    LPRINTF("entered getNodeValue curBufferPos = %s", *curBufferPos);

    char* nodeValue = (char*) calloc(MAX_NODE_VALUE_SIZE, sizeof(char));

    size_t bufferPosShift = 0;
    sscanf(*curBufferPos, "%s%n", nodeValue, &bufferPosShift);

    LPRINTF("getNodeValue nodeValue = %s, len nodeValue = %lu", nodeValue, bufferPosShift);

    *curBufferPos += bufferPosShift;

    return nodeValue;
}

static void skipSpaceAndCloseBracket(char** curBufferPos){
    assert(curBufferPos);

    while(true){
        if((**curBufferPos == ')') || (**curBufferPos == ' ')){
            (*curBufferPos)++;
        }
        else{
            break;
        }
    }
}  

static void expect(char** curBufferPos, char expected, const char* file, int line, const char* func){
    assert(curBufferPos);
    assert(file);
    assert(func);

    if (**curBufferPos != expected){
        SYNTAX_ERROR;
    }
    (*curBufferPos)++;
}

static void SyntaxError(const char* file, int line, const char* func){
    assert(file);
    assert(func);

    printf("Синтаксическая ошибка!! %s:%d:%s", file, line, func);
}