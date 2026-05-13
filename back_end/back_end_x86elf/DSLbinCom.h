#include "core/DSL.h"

#define BP writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), BREAK_POINT_GDB_CODE)

#define INSTRUCTION_INFO_TYPE(insInfo)        insInfo->instrType
#define INSTRUCTION_INFO_MODE(insInfo)        insInfo->instrMode

#define INSTRUCTION_INFO_AMOUNT_ARGS(insInfo) insInfo->amountArgs
#define INSTRUCTION_INFO_ARGS(insInfo)        insInfo->args

#define INSTRUCTION_ARG_TYPE(arg)       arg->argType

#define INSTRUCTION_R64_TYPE_USED(arg)  arg->r64TypeUsed

#define INSTRUCTION_ARG_VALUE_REG(arg)  arg->argValue.reg
#define INSTRUCTION_ARG_VALUE_NUM(arg)  arg->argValue.number

#define INSTRUCTION_ARG_IS_MEM_CASE(arg)  arg->isMemCase
#define INSTRUCTION_ARG_MEM_SHIFT(arg)    arg->memShift

#define INSTRUCTION_ENCODE_RULE(instructionsEncodeRules, numRule)        instructionsEncodeRules[numRule]
#define INSTRUCTION_ENCODE_RULE_TYPE(instructionsEncodeRule)   instructionsEncodeRule->type
#define INSTRUCTION_ENCODE_RULE_MODE(instructionsEncodeRule)   instructionsEncodeRule->mode
#define INSTRUCTION_ENCODE_RULE_BYTES(instructionsEncodeRule) instructionsEncodeRule->opcodeBytes
#define INSTRUCTION_ENCODE_RULE_AMOUNT_BYTES(instructionsEncodeRule) instructionsEncodeRule->opcodeAmountBytes
#define INSTRUCTION_ENCODE_RULE_MOD_RM_CASE(instructionsEncodeRule) instructionsEncodeRule->modRmCase
#define INSTRUCTION_ENCODE_RULE_OP_EN(instructionsEncodeRule) instructionsEncodeRule->opEn

#define _MOV(arg1, arg2)    prepareInstructionEndcodeInfo(context, MOV_I, 2, arg1, arg2)

#define _PUSH(arg1)         prepareInstructionEndcodeInfo(context, PUSH_I, 1, arg1, "")
#define _POP(arg1)          prepareInstructionEndcodeInfo(context, POP_I, 1, arg1, "")

#define _CALL(arg1)         prepareInstructionEndcodeInfo(context, CALL_I, 1, arg1, "")
#define _RET()              prepareInstructionEndcodeInfo(context, RET_I, 0, "", "")
#define _SYSCALL()          prepareInstructionEndcodeInfo(context, SYSCALL_I, 0, "", "")

#define _ADD(arg1, arg2)    prepareInstructionEndcodeInfo(context, ADD_I, 2, arg1, arg2)
#define _SUB(arg1, arg2)    prepareInstructionEndcodeInfo(context, SUB_I, 2, arg1, arg2)
#define _IMUL(arg1, arg2)   prepareInstructionEndcodeInfo(context, IMUL_I, 2, arg1, arg2)
#define _IDIV(arg1)         prepareInstructionEndcodeInfo(context, IDIV_I, 1, arg1, "")

#define _CMP(arg1, arg2)    prepareInstructionEndcodeInfo(context, CMP_I, 2, arg1, arg2)

#define _JMP(arg1)         prepareInstructionEndcodeInfo(context, JMP_I, 1, arg1, "")
#define _JG(arg1)          prepareInstructionEndcodeInfo(context, JG_I, 1, arg1, "")
#define _JGE(arg1)         prepareInstructionEndcodeInfo(context, JGE_I, 1, arg1, "")
#define _JL(arg1)          prepareInstructionEndcodeInfo(context, JL_I, 1, arg1, "")
#define _JLE(arg1)         prepareInstructionEndcodeInfo(context, JLE_I, 1, arg1, "")
#define _JE(arg1)          prepareInstructionEndcodeInfo(context, JE_I, 1, arg1, "")
#define _JNE(arg1)         prepareInstructionEndcodeInfo(context, JNE_I, 1, arg1, "")

#define _CVTSI2SS(arg1, arg2)   prepareInstructionEndcodeInfo(context, CVTSI2SS_I, 2, arg1, arg2)
#define _CVTTSS2SI(arg1, arg2)  prepareInstructionEndcodeInfo(context, CVTTSS2SI_I, 2, arg1, arg2)
#define _SQRTSS(arg1, arg2)     prepareInstructionEndcodeInfo(context, SQRTSS_I, 2, arg1, arg2)

#define _CQO()                  prepareInstructionEndcodeInfo(context, CQO_I, 0, "", "")

#define _LABEL(arg1)        fixUpSetLabelOffset(_CONTEXT_ELF_FIX_UP_LIST(context), arg1, BIN_BUFFER_SIZE(_CONTEXT_ELF_CODE_BUFFER(context)))
#define _LABEL_USE(arg1)    fixUpListAddPatchOffset(_CONTEXT_ELF_FIX_UP_LIST(context), arg1, BIN_BUFFER_SIZE(_CONTEXT_ELF_CODE_BUFFER(context)))