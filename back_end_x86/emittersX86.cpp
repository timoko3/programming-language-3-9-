#include "emitters.h"


typedef static void (*emitter_t) (treeNode_t*, codeGenContext*);

struct emitRule{
    ASTnodeType type;
    emitter_t   emitter;
};

static emitRule emittersTable[] = {
    {NUMBER,        emitNumber},
    {ADD,           emitAdd   },
    {SUB,           emitSub   },
    {MUL,           emitMul   },
    {DIVIDE,        emitDiv   },
    {HLT,           emitHlt   },
    {RETURN,        emitRet   },
    {GT,            emitGt    },           
    {LT,            emitLt    },
    {GE,            emitGe    },
    {LE,            emitLe    },
    {EQUAL,         emitEqual },
    {NOT_EQUAL,     emitNEqual},
    {SQRT,          emitSqrt  },
    {IF,            emitPlug  },
    {WHILE,         emitPlug  },
    {IN,            emitPlug  },
    {OUT,           emitPlug  },
    {ASSIGN,        emitPlug  },
    {END_STATEMENT, emitPlug  },
    {MAIN,          emitPlug  },
    {POPM,          emitPopm  },
    {DRAW,          emitDraw  }
};
