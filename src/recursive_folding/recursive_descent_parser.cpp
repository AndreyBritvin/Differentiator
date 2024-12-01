#include "recursive_descent_parser.h"
#include <stdio.h>
#include <math.h>

// const char* input = "25*10*(3*(25-10*2)+1)-15$";
// const char* input = "25+10-10*3$";
// int pos = 0;

#define SYNTAX_ERROR(...) fprintf(stderr, __VA_ARGS__);

int get_grammatic(char* input)
{
    size_t pos = 0;
    int val = get_expression(input, &pos);
    if (input[pos] != '$') SYNTAX_ERROR("At pos = %d expected $, but %c instead\n", pos, input[pos]);

    return val;
}

int get_exp(char* input, size_t* pos)
{
    int val = get_primary(input, pos);
    while (input[*pos] == '^')
    {
        int operation = input[*pos];
        (*pos)++;
        int val_2 = get_primary(input, pos);
        val = pow(val, val_2);
    }

    return val;
}

int get_mul_div(char* input, size_t* pos)
{
    int val = get_exp(input, pos);
    while (input[*pos] == '*' || input[*pos] == '/')
    {
        int operation = input[*pos];
        (*pos)++;
        int val_2 = get_exp(input, pos);
        if (operation == '*') val *= val_2;
        if (operation == '/') val /= val_2;
    }

    return val;
}

int get_primary(char* input, size_t* pos)
{
    if (input[*pos] == '(')
    {
        (*pos)++;
        int val = get_expression(input, pos);
        if (input[*pos] != ')') SYNTAX_ERROR("At pos = %d expected ), but %c instead\n", pos, input[*pos]);
        (*pos)++;
        return val;
    }

    return get_number(input, pos);
}

int get_expression(char* input, size_t* pos)
{
    int val = get_mul_div(input, pos);
    while (input[*pos] == '+' || input[*pos] == '-')
    {
        int operation = input[*pos];
        (*pos)++;
        int val_2 = get_mul_div(input, pos);
        if (operation == '+') val += val_2;
        if (operation == '-') val -= val_2;
    }

    return val;
}

int get_number(char* input, size_t* pos)
{
    int val = 0;
    while ('0' <= input[*pos] && input[*pos] <= '9')
    {
        val = val * 10 + input[*pos] - '0';
        (*pos)++;
    }

    return val;
}
