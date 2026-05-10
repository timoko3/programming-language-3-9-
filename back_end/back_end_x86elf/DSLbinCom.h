#define BP writeU8Buf(_CONTEXT_ELF_CODE_BUFFER(context), BREAK_POINT_GDB_CODE)

#define INSTRUCTION_INFO_TYPE(insInfo)        insInfo->instrType

#define INSTRUCTION_INFO_AMOUNT_ARGS(insInfo) insInfo->amountArgs
#define INSTRUCTION_INFO_ARGS(insInfo)        insInfo->args

#define INSTRUCTION_ARG_TYPE(arg)       arg->argType

#define INSTRUCTION_R64_TYPE_USED(arg)  arg->r64TypeUsed

#define INSTRUCTION_ARG_VALUE_REG(arg)  arg->argValue.reg
#define INSTRUCTION_ARG_VALUE_NUM(arg)  arg->argValue.number

#define INSTRUCTION_ARG_IS_MEM_CASE(arg)  arg->isMemCase
#define INSTRUCTION_ARG_MEM_SHIFT(arg)    arg->memShift

#define _MOV(arg1, arg2)    prepareInstructionEndcodeInfo(context, MOV, 2, arg1, arg2)