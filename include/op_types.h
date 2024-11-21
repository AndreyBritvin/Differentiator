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
    ADD,
    SUB,
    MUL,
    DIV,
    SIN,
    COS,
    EXP,
    TAN,
    SHN,
    CHS,
    CTH,
    TGH,
    CTG,
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
