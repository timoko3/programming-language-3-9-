#include "emitBinaryCommands.h"

#include "DSLbinCom.h"

#include "core/DSL.h"


#include <assert.h>

const uint8_t REX_PREFIX = 0x40;

const uint8_t RAX_CODE   = 0x0;
const uint8_t RBX_CODE   = 0x3;
const uint8_t RCX_CODE   = 0x1;
const uint8_t RDX_CODE   = 0x2;
const uint8_t RSP_CODE   = 0x4;
const uint8_t RBP_CODE   = 0x5;
const uint8_t RSI_CODE   = 0x6;
const uint8_t RDI_CODE   = 0x7;

const uint8_t R64_TO_R64_MOV_CODE = 0x8b;

const uint8_t BREAK_POINT_GDB_CODE = 0xCC;

static void emitREX(codeGenContext* context, REX_t rexType);
static void emitModRm(codeGenContext* context, argInst_t arg1, argInst_t arg2, genPurposeRegs reg1, genPurposeRegs reg2);
static uint8_t emitRegCode(genPurposeRegs reg);

void emitMov(codeGenContext* context, argInst_t arg1, argInst_t arg2, genPurposeRegs reg1, genPurposeRegs reg2){
    assert(context);

    BP;

    emitREX(context, REX_W);

    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), R64_TO_R64_MOV_CODE);

    emitModRm(context, arg1, arg2, reg1, reg2);

    BP;
}

static void emitREX(codeGenContext* context, REX_t rexType){
    assert(context);

    uint8_t rexByte = 0 | REX_PREFIX;

    switch (rexType){
        case REX_W:
            rexByte |= (1 << 3);
            break;
    
        default:
            break;
    }

    printf("rexByte = %02x", rexByte);

    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), rexByte);
}

static void emitModRm(codeGenContext* context, argInst_t arg1, argInst_t arg2, genPurposeRegs reg1, genPurposeRegs reg2){

    uint8_t modRmByte = 0;

    if((arg1 == R64) && (arg2 == R64)){
        modRmByte |= (0x03 << 6);

        modRmByte |= (emitRegCode(reg1) << 3);

        modRmByte |= (emitRegCode(reg2));
    }

    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), modRmByte);
}

static uint8_t emitRegCode(genPurposeRegs reg){
    uint8_t curRegCode = 0;

    switch (reg){
        case RAX:
            curRegCode = RAX_CODE;
            break;
        case RBX:
            curRegCode = RBX_CODE;
            break;
        case RCX:
            curRegCode = RCX_CODE;
            break;
        case RDX:
            curRegCode = RDX_CODE;
            break;
        case RSP:
            curRegCode = RSP_CODE;
            break;
        case RBP:
            curRegCode = RBP_CODE;
            break;
        case RSI:
            curRegCode = RSI_CODE;
            break;
        case RDI:
            curRegCode = RDI_CODE;
            break;
        default:
            break;
    }

    return curRegCode;
}