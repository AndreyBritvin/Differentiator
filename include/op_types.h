#ifndef OP_TYPES_H_
#define OP_TYPES_H_

enum node_type
{
    NUM,
    VAR,
    OP,
};

enum op_type
{
    ADD =  0,
    SUB =  1,
    MUL =  2,
    DIV =  3,
    SIN =  4,
    COS =  5,
    EXP =  6,
    TAN =  7,
    SHN =  8,
    CHS =  9,
    CTH = 10,
    TGH = 11,
    CTG = 12,
    UNKNOWN
};

struct operation
{
    op_type      type;
    const char*  text;
};

static operation all_ops[] = {//!!! order should be equal to enum!!!
    {ADD, "+"},
    {SUB, "-"},
    {MUL, "*"},
    {DIV, "/"},
    {SIN, "sin"},
    {COS, "cos"},
    {EXP, "^"},
    {TAN, "tan"},
    {SHN, "sinh"},
    {CHS, "cosh"},
    {CTH, "coth"},
    {TGH, "tanh"},
    {CTG, "cot"},
};

#endif // OP_TYPES_H_
