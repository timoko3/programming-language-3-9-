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

const uint8_t XMM0_CODE  = 0x0;


enum modRmCase_t{
    MR_CASE_R,
    MR_CASE_0,
    MR_CASE_5,
    MR_CASE_6,
    MR_CASE_7,
    MR_CASE_NO
};

enum opEn_t{
    M_OP_EN,
    I_OP_EN,
    D_OP_EN,

    MR_OP_EN,
    RM_OP_EN,

    MI_OP_EN,

    A_OP_EN,

    NO_OP_EN
};

struct instrEncodeRule_t{
    instr_t     type;
    instrMode_t mode;
    modRmCase_t modRmCase;
    opEn_t      opEn;
    size_t      amountArgs;
    size_t      opcodeAmountBytes;
    uint8_t     opcodeBytes[3];
};

instrEncodeRule_t instructionsEncodeRules[]{
    {PUSH_I, RM64MODE,     MR_CASE_6,  M_OP_EN,  1, 1, {0xFF}},
    {PUSH_I, MEM64MODE,    MR_CASE_6,  M_OP_EN,  1, 1, {0xFF}},
    {PUSH_I, IMM32MODE,    MR_CASE_NO, I_OP_EN,  1, 1, {0x68}},

    {POP_I,  RM64MODE,     MR_CASE_0,  M_OP_EN,  1, 1, {0x8F}},
    {POP_I,  MEM64MODE,    MR_CASE_0,  M_OP_EN,  1, 1, {0x8F}},

    {CALL_I, LABELMODE,    MR_CASE_NO, D_OP_EN,  1, 1, {0xE8}},
    {RET_I,  NOMODE,       MR_CASE_NO, NO_OP_EN, 0, 1, {0xC3}},

    {JMP_I,  LABELMODE,    MR_CASE_NO, NO_OP_EN, 1, 1, {0xE9}},

    {JG_I,   LABELMODE,    MR_CASE_NO, NO_OP_EN, 1, 2, {0x0F, 0x8F}},
    {JGE_I,  LABELMODE,    MR_CASE_NO, NO_OP_EN, 1, 2, {0x0f, 0x8D}},
    {JL_I,   LABELMODE,    MR_CASE_NO, NO_OP_EN, 1, 2, {0x0f, 0x8C}},
    {JLE_I,  LABELMODE,    MR_CASE_NO, NO_OP_EN, 1, 2, {0x0f, 0x8E}},
    {JE_I,   LABELMODE,    MR_CASE_NO, NO_OP_EN, 1, 2, {0x0f, 0x84}},
    {JNE_I,  LABELMODE,    MR_CASE_NO, NO_OP_EN, 1, 2, {0x0f, 0x85}},

    {SYSCALL_I, NOMODE,    MR_CASE_NO, NO_OP_EN, 0, 2, {0x0f, 0x05}},

    {MOV_I,   R64TORM64,   MR_CASE_R,  MR_OP_EN, 2, 1, {0x89}},
    {MOV_I,   R64TOMEM64,  MR_CASE_R,  MR_OP_EN, 2, 1, {0x89}},

    {MOV_I,   MEM64TOR64,  MR_CASE_R,  RM_OP_EN, 2, 1, {0x8b}},

    {MOV_I,   IMM32TORM64, MR_CASE_0,  MI_OP_EN, 2, 1, {0xc7}},

    {ADD_I,   R64TORM64,   MR_CASE_R,  MR_OP_EN, 2, 1, {0x01}},
    {ADD_I,   R64TOMEM64,  MR_CASE_R,  MR_OP_EN, 2, 1, {0x01}},
    {ADD_I,   IMM32TORM64, MR_CASE_0,  MI_OP_EN, 2, 1, {0x81}},

    {SUB_I,   R64TORM64,   MR_CASE_R,  MR_OP_EN, 2, 1, {0x29}},
    {SUB_I,   R64TOMEM64,  MR_CASE_R,  MR_OP_EN, 2, 1, {0x29}},
    {SUB_I,   IMM32TORM64, MR_CASE_5,  MI_OP_EN, 2, 1, {0x81}},

    {IMUL_I,  RM64TOR64,   MR_CASE_R,  RM_OP_EN, 2, 2, {0x0f, 0xaf}},

    {IDIV_I,  RM64MODE,    MR_CASE_7,  M_OP_EN,  1, 1, {0xf7}},

    {CMP_I,   R64TORM64,   MR_CASE_R,  MR_OP_EN, 2, 1, {0x39}},
    {CMP_I,   IMM32TORM64, MR_CASE_7,  MI_OP_EN, 2, 1, {0x81}},

    {CVTSI2SS_I,  RM64TOXMM, MR_CASE_R, A_OP_EN,  2, 2, {0x0F, 0x2A}},
    {CVTTSS2SI_I, XMMTOR64,  MR_CASE_R, A_OP_EN,  2, 2, {0x0F, 0x2C}},
    {SQRTSS_I,    XMMTOXMM,  MR_CASE_R, A_OP_EN,  2, 2, {0x0F, 0x51}},

    {CQO_I,       NOMODE,  MR_CASE_NO, NO_OP_EN,  0, 1, {0x99}}
};

const size_t ENCODE_RULES_TABLE_SIZE = sizeof(instructionsEncodeRules) / sizeof(instrEncodeRule_t);

// const uint8_t RM64_TO_R64_MOV_CODE   = 0x8b;
// const uint8_t R64_TO_RM64_MOV_CODE   = 0x89;
// const uint8_t IMM32_TO_RM64_MOV_CODE = 0xc7;

// const uint8_t PUSH_RM64_CODE         = 0xFF;
const uint8_t PUSH_RM64_MOD_RM_REG_C = 0x6;
// const uint8_t PUSH_IMM32_CODE        = 0x68;

// const uint8_t POP_RM64_CODE          = 0x8F;
const uint8_t POP_RM64_MOD_RM_REG_C  = 0x0;

// const uint8_t CALL_REL32_CODE        = 0xE8;

// const uint8_t SYSCALL_FIRST_BYTE_CODE  = 0x0f;
// const uint8_t SYSCALL_SECOND_BYTE_CODE = 0x05;

const uint8_t BREAK_POINT_GDB_CODE  = 0xCC;

const uint8_t MANDATORY_BYTE_CODE   = 0xF3; 

const uint8_t R64_TO_RM64_ADD_CODE   = 0x01;
const uint8_t IMM32_TO_RM64_ADD_CODE = 0x01;

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

static void chooseArgsMode(codeGenContext* context, instructionInfo* instrInfo);

static void emitInstr(codeGenContext* context, instructionInfo* instrInfo);

static void emitMandatory(codeGenContext* context, instructionInfo* instrInfo);
static void emitREX(codeGenContext* context, instructionInfo* instrInfo);
static void emitModRm(codeGenContext* context, instructionInfo* instrInfo, modRmCase_t modRmType, opEn_t opEnType);
static void emitSIB(codeGenContext* context, instructionInfo* instrInfo);
static uint8_t emitRegCode(genPurposeRegs reg);

static instrEncodeRule_t* findEncodeRule(instr_t type, instrMode_t mode);

static void emitR64toRM64(codeGenContext* context, instructionInfo* instrInfo);
static void emitImm32toRM64(codeGenContext* context, instructionInfo* instrInfo);
static void emitRM64(codeGenContext* context, instructionInfo* instrInfo);
static void emitImm32(codeGenContext* context, instructionInfo* instrInfo);

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

    chooseArgsMode(context, curInstrInfo);

    printf("curMode = %d\n", INSTRUCTION_INFO_MODE(curInstrInfo));

    emitInstr(context, curInstrInfo);

    free(curInstrInfo);

    _CONTEXT_CURRENT_INSTRUCTION(context)++;
}

static argInst_t instrGetArg(codeGenContext* context, instrArg_t* arg, const char* strArg, int argNum){
    assert(context);
    assert(arg);
    assert(strArg);

    printf("strArg: %s\n", strArg);

    regTableElem_t* refReg = regTableElemCtor(NONE, (char*) strArg, ANY, 0);

    regTableElem_t* foundReg = regTableFind(_CONTEXT_REG_TABLE(context), findNameRegRule, refReg);

    regTableElemDtor(refReg);             
    
    char strPart[64] = "";
    char op = '+';
    int n = 0;
    int parsedVarsAmount = 0; 
    if(foundReg){
        if(REG_TABLE_ELEM_USE_SCENERY(foundReg) != VEC_REG) INSTRUCTION_ARG_TYPE(arg) = R64;
        else INSTRUCTION_ARG_TYPE(arg) = XMM;

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
                printf("INSTRUCTION_ARG_MEM_SHIFT = %d\n", INSTRUCTION_ARG_MEM_SHIFT(arg));
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
    else if(sscanf(strArg, "%[a-zA-Z]%d", strPart, &n) == 2){
        INSTRUCTION_ARG_TYPE(arg) = LABEL;

        // label_t* refLabel = createLabel(_CONTEXT_LABELS_TABLE(context), strPart, n, 0);
        // if(!labelFind(_CONTEXT_LABELS_TABLE(context), labelCmp, refLabel)) createLabel(_CONTEXT_LABELS_TABLE(context), strPart, n, 1);
    }
    else{
        printf("Недопустимой аргумент для данной инструкции\n");
    } 

    return INSTRUCTION_ARG_TYPE(arg);
}

static void chooseArgsMode(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    if(!INSTRUCTION_ARG_IS_MEM_CASE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) &&
       !INSTRUCTION_ARG_IS_MEM_CASE((&INSTRUCTION_INFO_ARGS(instrInfo)[1])) &&
       INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) == R64){
        if(INSTRUCTION_INFO_TYPE(instrInfo) != IMUL_I) INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) = RM64;
    }

    instrArg_t arg1 = INSTRUCTION_INFO_ARGS(instrInfo)[0];
    instrArg_t arg2 = INSTRUCTION_INFO_ARGS(instrInfo)[1];    
    
    switch(INSTRUCTION_ARG_TYPE((&arg1))){
        case R64:
            switch (INSTRUCTION_ARG_TYPE((&arg2))){
                case MEM64:
                    INSTRUCTION_INFO_MODE(instrInfo) = MEM64TOR64;
                    break;
                case R64:
                    INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[1])) = RM64;
                    INSTRUCTION_INFO_MODE(instrInfo) = RM64TOR64;
                    break;
                case NONE_ARG:
                    INSTRUCTION_INFO_MODE(instrInfo) = RM64MODE;
                    break;
                default:
                    break;
            }
            break;
        case MEM64:
            switch (INSTRUCTION_ARG_TYPE((&arg2))){
                case R64:
                    INSTRUCTION_INFO_MODE(instrInfo) = R64TOMEM64;
                    break;
                case IMM32:
                    INSTRUCTION_INFO_MODE(instrInfo) = IMM32TOMEM64;
                    break;
                case NONE_ARG:
                    INSTRUCTION_INFO_MODE(instrInfo) = MEM64MODE;
                    break;
                default: break;
            }
            break;
        case RM64:
            switch (INSTRUCTION_ARG_TYPE((&arg2))){
                case R64:
                    INSTRUCTION_INFO_MODE(instrInfo) = R64TORM64;
                    break;
                case IMM32:
                    INSTRUCTION_INFO_MODE(instrInfo) = IMM32TORM64;
                    break;
                case XMM:
                    INSTRUCTION_INFO_MODE(instrInfo) = XMMTOR64;
                    INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[0])) = R64;
                    break;
                case NONE_ARG:
                    INSTRUCTION_INFO_MODE(instrInfo) = RM64MODE;
                    break;
                default: break;
            }
            break;
        case XMM:
            switch (INSTRUCTION_ARG_TYPE((&arg2))){
                case R64:
                    INSTRUCTION_INFO_MODE(instrInfo) = RM64TOXMM;
                    INSTRUCTION_ARG_TYPE((&INSTRUCTION_INFO_ARGS(instrInfo)[1])) = RM64;
                    break;
                case XMM:
                    INSTRUCTION_INFO_MODE(instrInfo) = XMMTOXMM;
                default: break;
            }
            break;
        case IMM32:
            INSTRUCTION_INFO_MODE(instrInfo) = IMM32MODE;
            break;
        case LABEL:
            INSTRUCTION_INFO_MODE(instrInfo) = LABELMODE;
            break; 
        case NONE_ARG:
            INSTRUCTION_INFO_MODE(instrInfo) = NOMODE;
            break; 
        default: break;
    }
}

static void emitInstr(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);
    
    instrEncodeRule_t* instrEncodeRule = findEncodeRule(INSTRUCTION_INFO_TYPE(instrInfo), INSTRUCTION_INFO_MODE(instrInfo));
    assert(instrEncodeRule);

    // BP;

    emitMandatory(context, instrInfo);

    emitREX(context, instrInfo);

    for(size_t i = 0; i < INSTRUCTION_ENCODE_RULE_AMOUNT_BYTES(instrEncodeRule); i++){
        writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), INSTRUCTION_ENCODE_RULE_BYTES(instrEncodeRule)[i]);
    }
    
    emitModRm(context, instrInfo, INSTRUCTION_ENCODE_RULE_MOD_RM_CASE(instrEncodeRule), INSTRUCTION_ENCODE_RULE_OP_EN(instrEncodeRule));

    switch (INSTRUCTION_INFO_MODE(instrInfo)){
        case RM64TOR64:
        case MEM64TOR64:
        case R64TOMEM64:
        case R64TORM64:
        case RM64MODE:
        case MEM64MODE:
            break;
        case IMM32TORM64:
        case IMM32TOMEM64:
            emitImm32toRM64(context, instrInfo);
            break;
        case IMM32MODE:
            emitImm32(context, instrInfo);
            break;
        case LABELMODE:
            writeU32LeBuf(_CONTEXT_ELF_CODE_BUFFER(context), 0x0);
        case NOMODE:
            break;
        default:
            break;
    }
}

static void emitImm32(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    writeU32LeBuf(_CONTEXT_ELF_CODE_BUFFER(context), (uint32_t) INSTRUCTION_ARG_VALUE_NUM((&INSTRUCTION_INFO_ARGS(instrInfo)[0])));
}

static void emitImm32toRM64(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    writeU32LeBuf(_CONTEXT_ELF_CODE_BUFFER(context), (uint32_t) INSTRUCTION_ARG_VALUE_NUM((&INSTRUCTION_INFO_ARGS(instrInfo)[1])));
}

static void emitMandatory(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);    

    if(INSTRUCTION_INFO_MODE(instrInfo) == RM64TOXMM || 
       INSTRUCTION_INFO_MODE(instrInfo) == XMMTOR64  ||
       INSTRUCTION_INFO_MODE(instrInfo) == XMMTOXMM){
        writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), MANDATORY_BYTE_CODE);
    }
}

static void emitREX(codeGenContext* context, instructionInfo* instrInfo){
    assert(context);
    assert(instrInfo);

    uint8_t rexByte = 0 | REX_PREFIX;

    // if(INSTRUCTION_INFO_TYPE(instrInfo) == MOV_I)

    rexByte |= (1 << REX_W_BIT);   // set REX.W

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

static void emitModRm(codeGenContext* context, instructionInfo* instrInfo, modRmCase_t modRmType, opEn_t opEnType){
    assert(context);
    assert(instrInfo);

    if(modRmType == MR_CASE_NO) return;

    uint8_t modRmByte = 0;

    instrArg_t arg1 = INSTRUCTION_INFO_ARGS(instrInfo)[0];
    instrArg_t arg2 = INSTRUCTION_INFO_ARGS(instrInfo)[1];

    uint8_t firstThreeBits = 0;
    uint8_t secondThreeBits = 0;

    switch (INSTRUCTION_INFO_MODE(instrInfo)){
        case RM64MODE:
        case IMM32TORM64:
        case R64TORM64:
        case RM64TOR64:
        case RM64TOXMM:
        case XMMTOR64:
        case XMMTOXMM:
            modRmByte |= (MOD_RM_REG_MOD << MOD_RM_MOD_OFFSET);
            break;

        case MEM64MODE:
        case R64TOMEM64:
        case IMM32TOMEM64:
        case MEM64TOR64:
            if(INSTRUCTION_ARG_MEM_SHIFT((&arg1)) != 0)      modRmByte |= (MOD_RM_MEM_DISP8_MOD << MOD_RM_MOD_OFFSET);
            else if(INSTRUCTION_ARG_MEM_SHIFT((&arg2)) != 0) modRmByte |= (MOD_RM_MEM_DISP8_MOD << MOD_RM_MOD_OFFSET);
            else                                             modRmByte |= (MOD_RM_MEM_MOD       << MOD_RM_MOD_OFFSET);
            break;
            

        default:
            break;
    }

    if(modRmType == MR_CASE_R){
        switch(opEnType){
            case RM_OP_EN:
                firstThreeBits |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg2)))));
                secondThreeBits |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))) << 3);
                break;
            case MR_OP_EN:
                firstThreeBits |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))));
                secondThreeBits |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg2)))) << 3);
                break;
            case MI_OP_EN:
                firstThreeBits |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))));
                break;
            case A_OP_EN:
                firstThreeBits |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg2)))));
                secondThreeBits |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))) << 3);
                break;
            
            default:
                break;
        }
    }
    else{
        firstThreeBits |= (emitRegCode(REG_TABLE_ELEM_REG(INSTRUCTION_ARG_VALUE_REG((&arg1)))));   

        uint8_t specialCaseVal = 0;
        switch(modRmType){
            case MR_CASE_0:  
                specialCaseVal = 0x0;
                break;
            case MR_CASE_5:
                specialCaseVal = 0x5;
                break;
            case MR_CASE_6:
                specialCaseVal = 0x6;
                break;
            case MR_CASE_7:
                specialCaseVal = 0x7;
                break;
            default:
                printf("не специальный случай modRm\n");
                break;
        }
        secondThreeBits |= (specialCaseVal << 3);
    }

    modRmByte |= firstThreeBits;
    modRmByte |= secondThreeBits;

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
        case XMM0:
            curRegCode = XMM0_CODE;
            break;
        
        default:
            break;
    }

    return curRegCode;
}

static instrEncodeRule_t* findEncodeRule(instr_t type, instrMode_t mode){

    for(size_t i = 0; i < ENCODE_RULES_TABLE_SIZE; i++){
        if(INSTRUCTION_ENCODE_RULE_MODE((&INSTRUCTION_ENCODE_RULE(instructionsEncodeRules, i))) == mode && 
           INSTRUCTION_ENCODE_RULE_TYPE((&INSTRUCTION_ENCODE_RULE(instructionsEncodeRules, i))) == type)

           return &INSTRUCTION_ENCODE_RULE(instructionsEncodeRules, i);
    }

    return NULL;
}