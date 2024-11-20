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
    {DIV, "/"}
};

#endif // OP_TYPES_H_
