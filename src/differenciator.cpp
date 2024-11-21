#include "differenciator.h"
#include <assert.h>
#include "utils.h"
#include <stdio.h>
#include "my_tree.h"
#include <string.h>
#include <sys/stat.h>

// func to read from file  to buffer
size_t get_file_len(const char *filename)
{
    assert(filename != NULL);

    struct stat st = {};
    stat(filename, &st);

    return (size_t) st.st_size;
}

err_code_t fill_buffer(char **buffer_to_fill, const char* filename)
{
    size_t filesize = get_file_len(filename);
    char *temp_buf = (char *) calloc(filesize + 1, sizeof(char));
    if (temp_buf == NULL)
    {
        return ERROR_CALLOC_IS_NULL;
    }
    *buffer_to_fill = temp_buf;

    FILE* SAFE_OPEN_FILE(input_file, filename, "r");

    fread(temp_buf, 1, filesize, input_file);
    fclose(input_file);

    return OK;
}

my_tree_t make_tree(char *buffer)
{
    assert(buffer);

    INIT_TREE(tree_to_fill);

    free(tree_to_fill.root);

    size_t position = 0;
    tree_to_fill.root = fill_node(buffer, &position, &tree_to_fill, NULL);
    TREE_DUMP(&tree_to_fill, tree_to_fill.root, "I am gROOT (generated this tree after reading file)");

    return tree_to_fill;
}

node_t* fill_node(char * buffer, size_t* position, my_tree_t* tree, node_t* parent)
{
    assert(buffer);
    assert(position);
    assert(tree);

    (*position)++;

    size_t len_of_expr = 0;
    tree_val_t expression = 0;

    sscanf(buffer + *position, " %lf%ln", &expression, &len_of_expr);

    *position += len_of_expr;

    tree->size++;
    node_t* node_to_return = new_node(tree, 0, 0, NULL, NULL); // TODO: potential error via 0
    node_to_return->parent = parent;
    if (*position == 1)
    {
        tree->root = node_to_return;
    }

    bool is_left = true;

    while (buffer[*position] != ')')
    {
        // TREE_DUMP(tree, node_to_return, "Working with this\n Input text = %s", expression);
        // printf("Current char is %c position %zu\n", buffer[*position], *position);
        if (buffer[*position] == '(' && is_left)
        {
            is_left = false;
            node_to_return->left = fill_node(buffer, position, tree, node_to_return);
        }

        else if (buffer[*position] == '(' && !is_left)
        {
            node_to_return->right = fill_node(buffer, position, tree, node_to_return);
        }

        else
        {
            fprintf(stderr, "Some error happened in brackets\n");
        }

        (*position)++;
    }

    return node_to_return;
}

#define LEFT node->left
#define RIGHT node->right
#define ADD_(L, R) evaluate_tree(L) + evaluate_tree(R);
#define SUB_(L, R) evaluate_tree(L) - evaluate_tree(R);
#define DIV_(L, R) evaluate_tree(L) / evaluate_tree(R);
#define MUL_(L, R) evaluate_tree(L) * evaluate_tree(R);

tree_val_t evaluate_tree(node_t* node)
{
    if (node->type == NUM) return node->data;
    if (node->type == VAR) return Global_X;

    switch ((int) node->data)
    {
        case ADD: return ADD_(LEFT, RIGHT);
        case SUB: return SUB_(LEFT, RIGHT);
        case DIV: return DIV_(LEFT, RIGHT);
        case MUL: return MUL_(LEFT, RIGHT);
    }
}
#undef ADD_
#undef SUB_
#undef DIV_
#undef MUL_
#undef LEFT
#undef RIGHT

node_t* differenciate(my_tree_t* doubled_tree, node_t* node)
{
    if (node->type == NUM) return new_node(doubled_tree, NUM, 0, NULL, NULL);
    if (node->type == VAR) return new_node(doubled_tree, NUM, 1, NULL, NULL);
    if (node->type == OP)
    {
        switch ((int) node->data)
        {
            case ADD: return diff_add(doubled_tree, node);
            case SUB: return diff_sub(doubled_tree, node);
            case MUL: return diff_mul(doubled_tree, node);
            case DIV: return diff_div(doubled_tree, node);

            default : fprintf(stderr, "Unknown operator in differenciate\n"); return NULL;
        }
    }
}

node_t* copy_subtree(my_tree_t* main_tree, node_t* node)
{
    node_t* copy_node = new_node(main_tree, node->type, node->data, NULL, NULL);

    node_t* left_sub  = NULL;
    node_t* right_sub = NULL;

    if (node->left  != NULL)
    {
        left_sub  = copy_subtree(main_tree, node->left);
        left_sub->parent = copy_node;
    }
    if (node->right != NULL)
    {
        right_sub = copy_subtree(main_tree, node->right);
        right_sub->parent = copy_node;
    }

    copy_node->left  = left_sub;
    copy_node->right = right_sub;

    return copy_node;
}

err_code_t latex_output(my_tree_t* tree, const char* filename)
{
    FILE* SAFE_OPEN_FILE(output_file, filename, "a");

    latex_node(tree, tree->root, output_file);

    fclose(output_file);

    return OK;
}

#define  INFIX_OUT(text, L, R)  fprintf(output, "(");\
                                latex_node(tree, L, output);\
                                fprintf(output, ")");\
                                fprintf(output, text);\
                                fprintf(output, "(");\
                                latex_node(tree, R, output);\
                                fprintf(output, ")");

#define PREFIX_OUT(text, L, R)  fprintf(output, text);      \
                                fprintf(output, "{");       \
                                latex_node(tree, L, output);\
                                fprintf(output, "}{");      \
                                latex_node(tree, R, output);\
                                fprintf(output, "}");

err_code_t latex_node(my_tree_t* tree, node_t* node, FILE* output)
{
    switch (node->type)
    {
        case NUM: fprintf(output, "%lg", node->data); break;
        case VAR: fprintf(output, "%c",  (char) node->data); break;
        case OP:
        {
            switch ((int) node->data)
            {
                case ADD:  INFIX_OUT(all_ops[(int) node->data].text, node->left, node->right); break;
                case SUB:  INFIX_OUT(all_ops[(int) node->data].text, node->left, node->right); break;
                case MUL:  INFIX_OUT(all_ops[(int) node->data].text, node->left, node->right); break;
                case DIV: PREFIX_OUT("\\frac", node->left, node->right); break;
                default: fprintf(stderr, "unknow operator in latex node");break;
            }
            break;
        }
        default:
            fprintf(stderr, "unknown operation in latex_node"); break;
    }

    return OK;
}
// bool check_is_operator(input)
// {
//
// }
