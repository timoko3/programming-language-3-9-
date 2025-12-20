#ifndef CMD_OPCODES_H
#define CMD_OPCODES_H

#include <stddef.h> 

#include "processor/cmd_spu.h"

enum cmdOpcodes{
    PUSH = 1,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    ABS,
    IN,
    OUT,
    HLT,
    JMP,
    JB,
    JBE,
    JA,
    JAE,
    JE,
    JNE,
    CALL,
    RET,
    PUSHREG = 32 + PUSH,
    POPREG,
    PUSHM,
    POPM,
    PUSHMA,
    POPMA,
    DRAW,
    LT,
    LE,
    GT,
    GE,
    EQUAL,
    NEQUAL
};

enum param_t{
    NO_PARAM,
    LABEL_PARAM,
    REG_PARAM,
    NUMBER_PARAM
};

struct command_t{
    cmdOpcodes            code; 
    handler_t             handler;   
    int                   nArgs; 
    param_t               param;
    const char*           name;
    unsigned long         hash;
};

static command_t commands[]{
    {ADD,      add,        0, NO_PARAM,     "ADD"    },
    {SUB,      sub,        0, NO_PARAM,     "SUB"    },
    {MUL,      mul,        0, NO_PARAM,     "MUL"    },
    {DIV,      myDiv,      0, NO_PARAM,     "DIV"    },
    {SQRT,     mySqrt,     0, NO_PARAM,     "SQRT"   },
    {ABS,      myAbs,      0, NO_PARAM,     "ABS"    },

    {LT,       lt,         0, NO_PARAM,     "LT"     },
    {LE,       le,         0, NO_PARAM,     "LE"     },
    {GT,       gt,         0, NO_PARAM,     "GT"     },
    {GE,       ge,         0, NO_PARAM,     "GE"     },
    {EQUAL,    eq,         0, NO_PARAM,     "EQ"     },
    {NEQUAL,   neq,        0, NO_PARAM,     "NEQ"    },
    
    
    {JMP,      jmp,        1, LABEL_PARAM,  "JMP"    },
    {JB,       jb,         1, LABEL_PARAM,  "JB"     },
    {JBE,      jbe,        1, LABEL_PARAM,  "JBE"    },
    {JA,       ja ,        1, LABEL_PARAM,  "JA"     },
    {JAE,      jae,        1, LABEL_PARAM,  "JAE"    },
    {JE,       je ,        1, LABEL_PARAM,  "JE"     },
    {JNE,      jne,        1, LABEL_PARAM,  "JNE"    },

    {PUSH,     push,       1, NUMBER_PARAM, "PUSH"   },
    {PUSHREG,  pushreg,    1, REG_PARAM,    "PUSHREG"},
    {POPREG,   popreg,     1, REG_PARAM,    "POPREG" },
    {PUSHM,    pushm,      1, REG_PARAM,    "PUSHM"  },
    {POPM,     popm,       1, REG_PARAM,    "POPM"   },

    {PUSHMA,   pushmAddr,  1, NUMBER_PARAM, "PUSHMA" },
    {POPMA,    popmAddr,   1, NUMBER_PARAM, "POPMA"  },
    
    {IN,       in,         0, NO_PARAM,     "IN"     },
    {OUT,      out,        0, NO_PARAM,     "OUT"    },
    {HLT,      hlt,        0, NO_PARAM,     "HLT"    },
    {CALL,     callFunc,   1, LABEL_PARAM,  "CALL"   },
    {RET,      retFunc,    0, NO_PARAM,     "RET"    },

    {DRAW,     draw,       0, NO_PARAM,     "DRAW"   }
};

const size_t N_REGISTERS   = 10;
const size_t PREAMBLE_SIZE = 2;
const int SIGNATURE        = 999;
const int VERSION          = 1;

#endif /* CMD_OPCODES_H */