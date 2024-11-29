#include "differenciator.h"
#include <assert.h>
#include "utils.h"
#include <stdio.h>
#include "my_tree.h"
#include <string.h>
#include <sys/stat.h>
#include "simplificator.h"

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
    char funcname[MAX_STRING_SIZE] = {};

    // printf("Type = %d ", sscanf(buffer + *position, "%[^()]%ln", &funcname, &len_of_expr));
    // printf("Get_funcname = %d\n", get_func_num(funcname));

    op_type_t var_type = NUM;
    if (sscanf(buffer + *position, "%lf%ln", &expression, &len_of_expr) > 0)
    {
        var_type = NUM;
    }
    else if (sscanf(buffer + *position, "%[^()]%ln", &funcname, &len_of_expr) >= 1
             && get_func_num(funcname) != UNKNOWN)
    {
        var_type = OP;
        expression = (tree_val_t) get_func_num(funcname);
    }
    else if (sscanf(buffer + *position, "%c%ln", &funcname, &len_of_expr))
    {
        var_type = VAR;
        expression = (tree_val_t) funcname[0];
    }
    else
    {
        fprintf(stderr, "No sscanf dont work\n");
    }

    *position += len_of_expr;

    tree->size++;
    node_t* node_to_return = new_node(tree, var_type, expression, NULL, NULL);
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

int get_func_num(char* input)
{
    // printf("%s\n", input);
    // printf("All_ops size = %d\n", sizeof(all_ops));

    for (size_t i = 0; i < sizeof(all_ops) / sizeof(operation); i++)
    {
        if (!strcmp(all_ops[i].text, input))
        {
            return i;
        }
    }

    return UNKNOWN;
}

#define LEFT node->left
#define RIGHT node->right
#define ADD_(L, R) evaluate_tree(L) + evaluate_tree(R);
#define SUB_(L, R) evaluate_tree(L) - evaluate_tree(R);
#define DIV_(L, R) evaluate_tree(L) / evaluate_tree(R);
#define MUL_(L, R) evaluate_tree(L) * evaluate_tree(R);

#define EXP_(L, R) pow(evaluate_tree(L), evaluate_tree(R));
#define LOG_(L   ) log(evaluate_tree(L));

#define SIN_(L   ) sin(evaluate_tree(L));
#define COS_(L   ) cos(evaluate_tree(L));
#define TAN_(L   ) tan(evaluate_tree(L));
#define CTG_(L   ) 1 / tan(evaluate_tree(L));

#define SHN_(L   ) sinh(evaluate_tree(L));
#define CHS_(L   ) cosh(evaluate_tree(L));
#define TGH_(L   ) tanh(evaluate_tree(L));
#define CTH_(L   ) tanh(1 / evaluate_tree(L));

#define ARCSIN_(L   ) asin(evaluate_tree(L));
#define ARCCOS_(L   ) acos(evaluate_tree(L));
#define ARCTAN_(L   ) atan(evaluate_tree(L));
#define ARCCTG_(L   ) M_PI / 2 - atan(evaluate_tree(L));

#define ARCSHN_(L   ) asinh(evaluate_tree(L));
#define ARCCHS_(L   ) acosh(evaluate_tree(L));
#define ARCTGH_(L   ) atanh(evaluate_tree(L));
#define ARCCTH_(L   ) atanh(1 / evaluate_tree(L));

tree_val_t evaluate_tree(node_t* node)
{
    if (node->type == NUM)     return node->data;
    if (node->type == VAR)     return Global_X;
    if (node->type == SUBTREE) return evaluate_tree(node->left);

    switch ((int) node->data)
    {
        case ADD: return ADD_(LEFT, RIGHT);
        case SUB: return SUB_(LEFT, RIGHT);
        case DIV: return DIV_(LEFT, RIGHT);
        case MUL: return MUL_(LEFT, RIGHT);

        case SIN: return SIN_(LEFT);
        case COS: return COS_(LEFT);
        case TAN: return TAN_(LEFT);
        case CTG: return CTG_(LEFT);

        case SHN: return SHN_(LEFT);
        case CHS: return CHS_(LEFT);
        case TGH: return TGH_(LEFT);
        case CTH: return CTH_(LEFT);

        case EXP: return EXP_(LEFT, RIGHT);
        case LOG: return LOG_(LEFT);

        case ARCSIN: return ARCSIN_(LEFT);
        case ARCCOS: return ARCCOS_(LEFT);
        case ARCTAN: return ARCTAN_(LEFT);
        case ARCCTG: return ARCCTG_(LEFT);

        case ARCSHN: return ARCSHN_(LEFT);
        case ARCCHS: return ARCCHS_(LEFT);
        case ARCTGH: return ARCTGH_(LEFT);
        case ARCCTH: return ARCCTH_(LEFT);


        default: fprintf(stderr, "Unknown operation in evaluate\n"); return 0;
    }
}
#undef ADD_
#undef SUB_
#undef DIV_
#undef MUL_
#undef EXP_
#undef LEFT
#undef RIGHT

#define DO_DIFF(func)                                   \
{                                                       \
    node_t* result = diff_##func(doubled_tree, node);   \
    print_equation(doubled_tree, node, result);         \
    return result;                                      \
}

node_t* differenciate(my_tree_t* doubled_tree, node_t* node)
{
    assert(doubled_tree);
    assert(node);

    switch (node->type)
    {
        case NUM:
        {
            node_t* result = new_node(doubled_tree, NUM, 0, NULL, NULL);
            print_equation(doubled_tree, node, result);
            return result;
        }
        case VAR:
        {
            node_t* result = new_node(doubled_tree, NUM, 1, NULL, NULL);
            print_equation(doubled_tree, node, result);
            return result;
        }
        case OP:
        {
            switch ((int) node->data)
            {
                case ADD: DO_DIFF(add);
                case SUB: DO_DIFF(sub);
                case MUL: DO_DIFF(mul);
                case DIV: DO_DIFF(div);

                case SIN: DO_DIFF(sin);
                case COS: DO_DIFF(cos);
                case TAN: DO_DIFF(tan);
                case CTG: DO_DIFF(ctg);

                case SHN: DO_DIFF(shn);
                case CHS: DO_DIFF(chs);
                case TGH: DO_DIFF(tgh);
                case CTH: DO_DIFF(cth);

                case EXP: DO_DIFF(exp);
                case LOG: DO_DIFF(log);

                case ARCSIN: DO_DIFF(arcsin);
                case ARCCOS: DO_DIFF(arccos);
                case ARCTAN: DO_DIFF(arctan);
                case ARCCTG: DO_DIFF(arcctg);

                case ARCSHN: DO_DIFF(arcshn);
                case ARCCHS: DO_DIFF(arcchs);
                case ARCTGH: DO_DIFF(arctgh);
                case ARCCTH: DO_DIFF(arccth);

                default : fprintf(stderr, "Unknown operator in differenciate\n"); return NULL;
            }
        }
        case SUBTREE:
        {
            differenciate(doubled_tree, node->left);
        }
    }
}

my_tree_t get_taylor_series(my_tree_t* expr_tree, tree_val_t x0, size_t amount)
{
    INIT_TREE(taylor_tree);
    free(taylor_tree.root);
    taylor_tree.root = NULL;
    Global_X = x0;
    INIT_TREE(diff_subtree);
    free(diff_subtree.root);
    diff_subtree.root = copy_subtree(expr_tree, expr_tree->root);

    for (size_t i = 0; i < amount; i++)
    {
        INIT_TREE(diff_tree);
        free(diff_tree.root);
        diff_tree.root = differenciate(&diff_tree, diff_subtree.root);
        reduce_equation(&diff_tree);

        tree_val_t deriative_n = evaluate_tree(diff_tree.root);
        tree_val_t coef_n = deriative_n / factorial(i + 1);
        printf("i = %zu, n = %lg\n", i, coef_n);
        add_taylor_coeff(&taylor_tree, x0, i, coef_n);
        // TREE_DUMP(&diff_subtree, diff_subtree.root, "This is diff_subtree at i = %zu", i);
        // TREE_DUMP(&diff_tree, diff_tree.root, "This is diff_tree at i = %zu", i);

        diff_subtree.size = 100;
        tree_dtor(&diff_subtree);
        diff_subtree.root = copy_subtree(&diff_tree, diff_tree.root);
        diff_subtree.size = 100;

        tree_dtor(&diff_tree);
    }
    TREE_DUMP(&taylor_tree, taylor_tree.root, "before reducing");

    reduce_equation(&taylor_tree);
    tree_dtor(&diff_subtree);

    return taylor_tree;
}

#define NUM_(nums, num_of_num) node_t* num_##num_of_num = new_node(taylor_tree, NUM, nums, NULL, NULL);
#define VAR_(var) node_t* var_node = new_node(taylor_tree, VAR, var, NULL, NULL);
#define ADD_(L, R) new_node(taylor_tree, OP, ADD, L, R)
#define SUB_(L, R) new_node(taylor_tree, OP, SUB, L, R)
#define MUL_(L, R) new_node(taylor_tree, OP, MUL, L, R)
#define EXP_(L, R) new_node(taylor_tree, OP, EXP, L, R)
#define PARENT(var_name, macro, L, R) node_t* var_name = macro(L, R); R->parent = L->parent = var_name;

err_code_t add_taylor_coeff(my_tree_t* taylor_tree, tree_val_t x0, size_t power, tree_val_t coef)
{
    NUM_(x0, x1);
    NUM_(power, power1);
    NUM_(coef, coef);
    NUM_(0, empty);
    VAR_('x');
    PARENT(minus1, SUB_, var_node, num_x1);
    PARENT(power1, EXP_, minus1, num_power1);
    PARENT(mul1,   MUL_, power1, num_coef);
    PARENT(add1, ADD_, mul1, num_empty);
    if (taylor_tree->root != NULL)
    {
        node_t* zero_node = find_zero_node(taylor_tree, taylor_tree->root);
        printf("coef = %lg, node = %p\n", coef, zero_node);
        zero_node->parent->right = add1;
        add1->parent = zero_node->parent;
        free(zero_node);
    }
    else
    {
        taylor_tree->root = add1;
    }
    // TREE_DUMP(taylor_tree, add1, "added this");

    return OK;
}

node_t* find_zero_node(my_tree_t* taylor_tree, node_t* curr_node)
{
    if (curr_node->type == NUM) return curr_node;

    return find_zero_node(taylor_tree, curr_node->right);
}

unsigned long long factorial(size_t num)
{
    unsigned long long fact = 1;

    for (size_t i = 1; i < num; i++)
    {
        fact *= i;
    }

    return fact;
}
