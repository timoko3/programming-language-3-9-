#include "emitBinaryCommands.h"

#include "core/DSL.h"

#include "labels.h"

#include <assert.h>
#include <ctype.h>
#include <malloc.h>

const uint8_t REX_PREFIX = 0x40;

const uint8_t RAX_CODE   = 0x0;
const uint8_t RBX_CODE   = 0x3;
const uint8_t RCX_CODE   = 0x1;
const uint8_t RDX_CODE   = 0x2;
const uint8_t RSP_CODE   = 0x4;
const uint8_t RBP_CODE   = 0x5;
const uint8_t RSI_CODE   = 0x6;
const uint8_t RDI_CODE   = 0x7;

const uint8_t R8_CODE    = 0x0;
const uint8_t R9_CODE    = 0x1;
const uint8_t R10_CODE   = 0x2;
const uint8_t R11_CODE   = 0x3;
const uint8_t R12_CODE   = 0x4;
const uint8_t R13_CODE   = 0x5;
const uint8_t R14_CODE   = 0x6;
const uint8_t R15_CODE   = 0x7;

const uint8_t RM64_TO_R64_MOV_CODE   = 0x8b;
const uint8_t R64_TO_RM64_MOV_CODE   = 0x89;
const uint8_t IMM32_TO_RM64_MOV_CODE = 0xc7;

const uint8_t PUSH_RM64_CODE         = 0xFF;
const uint8_t PUSH_RM64_MOD_RM_REG_C = 0x6;
const uint8_t PUSH_IMM32_CODE        = 0x68;

const uint8_t POP_RM64_CODE          = 0x8F;
const uint8_t POP_RM64_MOD_RM_REG_C  = 0x0;

const uint8_t CALL_REL32_CODE        = 0xE8;

const uint8_t SYSCALL_FIRST_BYTE_CODE  = 0x0f;
const uint8_t SYSCALL_SECOND_BYTE_CODE = 0x05;

const uint8_t BREAK_POINT_GDB_CODE  = 0xCC;

const uint8_t REX_W_BIT             = 3;
const uint8_t REX_R_BIT             = 2;
const uint8_t REX_X_BIT             = 1;
const uint8_t REX_B_BIT             = 0;

const uint8_t MOD_RM_MOD_OFFSET     = 6;
const uint8_t MOD_RM_REG_MOD        = 3;
const uint8_t MOD_RM_MEM_DISP8_MOD  = 1;
const uint8_t MOD_RM_MEM_MOD        = 0;

const uint8_t SIB_SCALE_OFFSET      = 6;
const uint8_t SIB_INDEX_OFFSET      = 3;

static argInst_t instrGetArg(codeGenContext* context, instrArg_t* arg, const char* strArg, int argNum);

static void emitREX(codeGenContext* context, instructionInfo* instrInfo);
static void emitModRm(codeGenContext* context, instructionInfo* instrInfo);
static void emitSIB(codeGenContext* context, instructionInfo* instrInfo);
static uint8_t emitRegCode(genPurposeRegs reg);

void prepareInstructionEndcodeInfo(codeGenContext* context, instr_t instrType, size_t amountArgs, const char* strArg1, const char* strArg2){
    assert(context);

    instructionInfo* curInstrInfo = (instructionInfo*) calloc(1, sizeof(instructionInfo));
    assert(curInstrInfo);

    INSTRUCTION_INFO_TYPE(curInstrInfo)        = instrType;                    
    INSTRUCTION_INFO_AMOUNT_ARGS(curInstrInfo) = amountArgs; 
    INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(curInstrInfo)[0])) = NONE_ARG;
    INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(curInstrInfo)[1])) = NONE_ARG;
    
    const char* strArgs[2] = {strArg1, strArg2};
    for(size_t i = 0; i < amountArgs; i++){
        instrGetArg(context, &INSTRUCTION_INFO_ARGS(curInstrInfo)[i], strArgs[i], i + 1);
    }

    switch (instrType){
        case MOV_I:
            emitMov(context, curInstrInfo);
            break;
        case PUSH_I:
            emitPush(context, curInstrInfo);
            break;
        case POP_I:
            emitPop(context, curInstrInfo);
            break;
        case CALL_I:
            emitCall(context, curInstrInfo);
            break;
        case SYSCALL_I:
            emitSyscall(context, curInstrInfo);
            break;
        default:
            printf("Данная инструкция пока не поддерживается\n");
            break;
    }

    free(curInstrInfo);

    _CONTEXT_CURRENT_INSTRUCTION(context)++;
}

static argInst_t instrGetArg(codeGenContext* context, instrArg_t* arg, const char* strArg, int argNum){
    assert(context);
    assert(arg);
    assert(strArg);

    regTableElem_t* refReg = regTableElemCtor(NONE, (char*) strArg, ANY, 0);

    regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findNameRegRule, refReg);

    regTableElemDtor(refReg);             
    
    char strPart[64] = "";
    char op = '+';
    int n = 0;
    int parsedVarsAmount = 0; 
    if(foundReg){
        INSTRUCTION_ARG_TYPE(arg) = R64;
        INSTRUCTION_ARG_VALUE_REG(arg) = foundReg;
    }
    else if((parsedVarsAmount = sscanf(strArg, "[%[a-z0-9] %c %d]", strPart, &op, &n) )> 0){
        regTableElem_t* refReg = regTableElemCtor(NONE, strPart, ANY, 0);
        regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findNameRegRule, refReg);
        regTableElemDtor(refReg);             

        INSTRUCTION_ARG_TYPE(arg)        = MEM64;
        INSTRUCTION_ARG_VALUE_REG(arg)   = foundReg;
        INSTRUCTION_ARG_IS_MEM_CASE(arg) = 1;

        printf("parsedVarsAmount = %d\n", parsedVarsAmount);
        switch (parsedVarsAmount){
            case 1:
                break;
            case 2:
            case 3:
                if(op == '-') n *= -1; 
                INSTRUCTION_ARG_MEM_SHIFT(arg) = n;
                break;
            default:
                printf("Недопустимая адресация\n");
                break;
        }
    }
    else if(sscanf(strArg, "%d", &n) == 1){
        INSTRUCTION_ARG_TYPE(arg)      = IMM32;
        INSTRUCTION_ARG_VALUE_NUM(arg) = n;
    }
    else if(sscanf(strArg, "%[a-z]%d", strPart, &n) == 2){
        INSTRUCTION_ARG_TYPE(arg) = LABEL;

        // label_t* refLabel = createLabel(_CONTEXT_LABELS_TABLE(context), strPart, n, 0);
        // if(!labelFind(_CONTEXT_LABELS_TABLE(context), labelCmp, refLabel)) createLabel(_CONTEXT_LABELS_TABLE(context), strPart, n, 1);
    }
    else{
        printf("Недопустимой аргумент для данной инструкции\n");
    } 

    return INSTRUCTION_ARG_TYPE(arg);
}

void emitMov(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);

    BP;

    if(!INSTRUCTION_ARG_IS_MEM_CASE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) &&
       !INSTRUCTION_ARG_IS_MEM_CASE((&INSTRUCTION_INFO_ARGS(instrInfo)[1])) &&
       INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) == R64){
        INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) = RM64;
    }

    instrArg_t arg1 = INSTRUCTION_INFO_ARGS(instrInfo)[0];
    instrArg_t arg2 = INSTRUCTION_INFO_ARGS(instrInfo)[1];

    switch(INSTRUCTION_ARG_TYPE((&arg1))){
        case R64:
            switch (INSTRUCTION_ARG_TYPE((&arg2))){
                case MEM64:
                    emitREX(context, instrInfo);
                    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), RM64_TO_R64_MOV_CODE);
                    emitModRm(context, instrInfo);
                    break;
                default:
                    break;
            }
            break;
        case MEM64:
        case RM64:
            switch (INSTRUCTION_ARG_TYPE((&arg2))){
                case R64:
                    emitREX(context, instrInfo);
                    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), R64_TO_RM64_MOV_CODE);
                    emitModRm(context, instrInfo);
                    break;
                case IMM32:
                    emitREX(context, instrInfo);
                    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), IMM32_TO_RM64_MOV_CODE);
                    emitModRm(context, instrInfo);
                    writeU32LeBuf(_CONTEXT_ELF_CODE_BUFFER(context), (uint32_t) INSTRUCTION_ARG_VALUE_NUM((&arg2)));
                    break;
                default: break;
            }
            break;
        default: break;
    }
    
}

void emitPush(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    BP;

    if(!INSTRUCTION_ARG_IS_MEM_CASE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) &&
        INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) == R64){
        INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) = RM64;
    }

    instrArg_t arg1 = INSTRUCTION_INFO_ARGS(instrInfo)[0];

    emitREX(context, instrInfo);

    switch(INSTRUCTION_ARG_TYPE((&arg1))){
        case MEM64:
        case RM64:
            writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), PUSH_RM64_CODE);
            emitModRm(context, instrInfo);
            break;
        case IMM32:
            writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), PUSH_IMM32_CODE);
            writeU32LeBuf(_CONTEXT_ELF_CODE_BUFFER(context), (uint32_t) INSTRUCTION_ARG_VALUE_NUM((&arg1)));
            break;
        default: break;
    }
}

void emitPop(codeGenContext* context, instructionInfo* instrInfo){
    BP;

    if(!INSTRUCTION_ARG_IS_MEM_CASE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) &&
        INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) == R64){
        INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) = RM64;
    }

    instrArg_t arg1 = INSTRUCTION_INFO_ARGS(instrInfo)[0];

    emitREX(context, instrInfo);

    switch(INSTRUCTION_ARG_TYPE((&arg1))){
        case MEM64:
        case RM64:
            writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), POP_RM64_CODE);
            emitModRm(context, instrInfo);
            break;
        default: break;
    }
}

void emitCall(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    BP;

    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), CALL_REL32_CODE);
    writeU32LeBuf(_CONTEXT_ELF_CODE_BUFFER(context), 0x0);    
}

void emitSyscall(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    BP;

    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), SYSCALL_FIRST_BYTE_CODE);
    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), SYSCALL_SECOND_BYTE_CODE);
}

static void emitREX(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    uint8_t rexByte = 0 | REX_PREFIX;

    if(INSTRUCTION_INFO_TYPE(instrInfo) == MOV_I) rexByte |= (1 << REX_W_BIT);   // set REX.W

    for(size_t i = 0; i < INSTRUCTION_INFO_AMOUNT_ARGS(instrInfo); i++){
        instrArg_t* curArg = (&INSTRUCTION_INFO_ARGS(instrInfo)[i]); 
        if(INSTRUCTION_ARG_TYPE(curArg) == R64 || 
           INSTRUCTION_ARG_TYPE(curArg) == RM64 ||
           INSTRUCTION_ARG_TYPE(curArg) == MEM64){  
            if(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG(curArg)) >= R8){
                switch (INSTRUCTION_ARG_TYPE(curArg)){
                    case R64:
                        rexByte |= (1 << REX_R_BIT);
                        break;
                    case MEM64:
                    case RM64:
                        rexByte |= (1 << REX_B_BIT);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), rexByte);
}

static void emitModRm(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    uint8_t modRmByte = 0;

    instrArg_t arg1 = INSTRUCTION_INFO_ARGS(instrInfo)[0];
    instrArg_t arg2 = INSTRUCTION_INFO_ARGS(instrInfo)[1];

    switch(INSTRUCTION_ARG_TYPE((&arg1))){
        case RM64:
            modRmByte |= (MOD_RM_REG_MOD << MOD_RM_MOD_OFFSET);
            switch(INSTRUCTION_ARG_TYPE((&arg2))){
                case R64:
                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))));
                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg2)))) << 3);
                    break;
                case IMM32:
                    
                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))));
                    break;
                case NONE_ARG:
                    if(INSTRUCTION_INFO_TYPE(instrInfo) == PUSH_I) modRmByte |= PUSH_RM64_MOD_RM_REG_C << 3;
                    if(INSTRUCTION_INFO_TYPE(instrInfo) == POP_I)  modRmByte |= POP_RM64_MOD_RM_REG_C << 3;
                    
                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))));

                default: break;
            }
            break;
        case R64:
            switch(INSTRUCTION_ARG_TYPE((&arg2))){
                case MEM64:
                    if(INSTRUCTION_ARG_MEM_SHIFT((&arg2)) != 0) modRmByte |= (MOD_RM_MEM_DISP8_MOD << MOD_RM_MOD_OFFSET);
                    else                                        modRmByte |= (MOD_RM_MEM_MOD       << MOD_RM_MOD_OFFSET);

                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))) << 3);
                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg2)))));
                    break;
                default: break;
            }
            break;
        case MEM64:
            if(INSTRUCTION_ARG_MEM_SHIFT((&arg1)) != 0) modRmByte |= (MOD_RM_MEM_DISP8_MOD << MOD_RM_MOD_OFFSET);
            else                                        modRmByte |= (MOD_RM_MEM_MOD       << MOD_RM_MOD_OFFSET);
            switch(INSTRUCTION_ARG_TYPE((&arg2))){
                case R64:
                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))));
                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg2)))) << 3);
                    break;
                case IMM32:
                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))));
                    break;
                case NONE_ARG:
                    if(INSTRUCTION_INFO_TYPE(instrInfo) == PUSH_I) modRmByte |= PUSH_RM64_MOD_RM_REG_C << 3;
                    if(INSTRUCTION_INFO_TYPE(instrInfo) == POP_I)  modRmByte |= POP_RM64_MOD_RM_REG_C << 3;
                    modRmByte |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))));
                
                default: break;
            }
            break;
    }

    writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), modRmByte);

    emitSIB(context, instrInfo);

    if(INSTRUCTION_ARG_MEM_SHIFT((&arg1)) != 0) writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), (uint8_t) INSTRUCTION_ARG_MEM_SHIFT((&arg1)));
    if(INSTRUCTION_ARG_MEM_SHIFT((&arg2)) != 0) writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), (uint8_t) INSTRUCTION_ARG_MEM_SHIFT((&arg2)));
}

static void emitSIB(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    uint8_t SIBByte = 0;

    // temp for only rsp and r12 adressation


    for(size_t i = 0; i < INSTRUCTION_INFO_AMOUNT_ARGS(instrInfo); i++){
        instrArg_t arg = INSTRUCTION_INFO_ARGS(instrInfo)[i];
        if(INSTRUCTION_ARG_TYPE((&arg)) == MEM64){
            if( (REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg))) == R12 ) ||
                (REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg))) == RSP) ){
                SIBByte |= (0 << SIB_SCALE_OFFSET);
                SIBByte |= (0x4 << SIB_INDEX_OFFSET);
                SIBByte |= 0x4;
            }
        }
    }
    
    if(SIBByte) writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), SIBByte);
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

        case R8:
            curRegCode = R8_CODE;
            break;
        case R9:
            curRegCode = R9_CODE;
            break;
        case R10:
            curRegCode = R10_CODE;
            break;
        case R11:
            curRegCode = R11_CODE;
            break;
        case R12:
            curRegCode = R12_CODE;
            break;
        case R13:
            curRegCode = R13_CODE;
            break;
        case R14:
            curRegCode = R14_CODE;
            break;
        case R15:
            curRegCode = R15_CODE;
            break;
        
        default:
            break;
    }

    return curRegCode;
}