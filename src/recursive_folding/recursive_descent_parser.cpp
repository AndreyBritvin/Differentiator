#include "recursive_descent_parser.h"
#include <stdio.h>
#include <math.h>
#include "my_tree.h"
#include <assert.h>

// const char* input = "25*10*(3*(25-10*2)+1)-15$";
// const char* input = "25+10-10*3$";
// int pos = 0;

#define SYNTAX_ERROR(...) fprintf(stderr, __VA_ARGS__);

my_tree_t make_tree(char *buffer)
{
    assert(buffer);
    tokens* programm_tokens = (tokens*) calloc(MAXIMUM_LEXEMS_COUNT, sizeof(tokens));

    size_t tokens_num = lexical_analysis(programm_tokens, buffer);

    for (size_t i = 0; i < tokens_num; i++)
    {
        if (programm_tokens[i].type == OP)
        {
            printf("index = %02zu, type = OP , name = %s\n", i,
                                                     all_ops[(int) programm_tokens[i].value].text);
        }
        else if (programm_tokens[i].type == NUM)
        {
            printf("index = %02zu, type = NUM, name = %lg\n", i,
                        programm_tokens[i].value);
        }
        else if (programm_tokens[i].type == VAR)
        {
            printf("index = %02zu, type = VAR, name = %s\n", i,
                        *(char**) &programm_tokens[i].value);
        }
        else if (programm_tokens[i].type == END)
        {
            printf("index = %02zu, type = OP , name = %c\n", i,
                        (char) programm_tokens[i].value);
        }
    }

    my_tree_t tree_to_fill = get_grammatic(buffer);
    TREE_DUMP(&tree_to_fill, tree_to_fill.root, "I am gROOT (generated this tree after reading file)");

    for (size_t i = 0; i < tokens_num; i++)
    {
        if (programm_tokens[i].type == VAR)
        {
            free(*(char**) &programm_tokens[i].value);
        }
    }
    free(programm_tokens);
    return tree_to_fill;
}

my_tree_t get_grammatic(char* input)
{
    INIT_TREE(tree_to_ret);
    free(tree_to_ret.root);
    size_t pos = 0;
    tree_to_ret.root = get_expression(&tree_to_ret, input, &pos);
    if (input[pos] != '$') SYNTAX_ERROR("At pos = %d expected $, but %c instead\n", pos, input[pos]);

    return tree_to_ret;
}

node_t* get_exp(my_tree_t* tree, char* input, size_t* pos)
{
    node_t* val = get_primary(tree, input, pos);
    while (input[*pos] == '^')
    {
        (*pos)++;
        node_t* val_2 = get_primary(tree, input, pos);
        val = new_node(tree, OP, EXP, val, val_2);
        val->left->parent = val->right->parent = val;
    }

    return val;
}

node_t* get_mul_div(my_tree_t* tree, char* input, size_t* pos)
{
    node_t* val = get_exp(tree, input, pos);
    while (input[*pos] == '*' || input[*pos] == '/')
    {
        int operation = input[*pos];
        (*pos)++;
        node_t* val_2 = get_exp(tree, input, pos);
        if (operation == '*')
        {
            val = new_node(tree, OP, MUL, val, val_2);
            val->left->parent = val->right->parent = val;
        }
        if (operation == '/')
        {
            val = new_node(tree, OP, DIV, val, val_2);
            val->left->parent = val->right->parent = val;
        }
    }

    return val;
}

node_t* get_primary(my_tree_t* tree, char* input, size_t* pos)
{
    if (input[*pos] == '(')
    {
        (*pos)++;
        node_t* val = get_expression(tree, input, pos);
        if (input[*pos] != ')') SYNTAX_ERROR("At pos = %d expected ), but %c instead\n", pos, input[*pos]);
        (*pos)++;
        return val;
    }
    node_t* to_ret = get_variable(tree, input, pos);
    if (to_ret != NULL) return to_ret;
    to_ret = get_number(tree, input, pos);

    return to_ret;
}

node_t* get_expression(my_tree_t* tree, char* input, size_t* pos)
{
    node_t* val = get_mul_div(tree, input, pos);
    while (input[*pos] == '+' || input[*pos] == '-')
    {
        int operation = input[*pos];
        (*pos)++;
        node_t* val_2 = get_mul_div(tree, input, pos);
        if (operation == '+')
        {
            val = new_node(tree, OP, ADD, val, val_2);
            val->left->parent = val->right->parent = val;
        }
        if (operation == '-')
        {
            val = new_node(tree, OP, SUB, val, val_2);
            val->left->parent = val->right->parent = val;
        }
    }

    return val;
}

node_t* get_number(my_tree_t* tree, char* input, size_t* pos)
{
    int val = 0;
    while ('0' <= input[*pos] && input[*pos] <= '9')
    {
        val = val * 10 + input[*pos] - '0';
        (*pos)++;
    }

    return new_node(tree, NUM, val, NULL, NULL);
}

node_t* get_variable(my_tree_t* tree, char* input, size_t* pos)
{
    if (input[*pos] == 'x')
    {
        (*pos)++;
        return new_node(tree, VAR, 'x', NULL, NULL);
    }

    return NULL;
}
