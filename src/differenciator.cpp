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
#define LOG_(L, R) log(evaluate_tree(R)) / log (evaluate_tree(L));

#define SIN_(L   ) sin(evaluate_tree(L));
#define COS_(L   ) cos(evaluate_tree(L));
#define TAN_(L   ) tan(evaluate_tree(L));
#define CTG_(L   ) 1 / tan(evaluate_tree(L));

#define SHN_(L   ) sinh(evaluate_tree(L));
#define CHS_(L   ) cosh(evaluate_tree(L));
#define TGH_(L   ) tanh(evaluate_tree(L));
#define CTH_(L   ) 1 / tanh(evaluate_tree(L));

#define ARCSIN_(L   ) asin(evaluate_tree(L));
#define ARCCOS_(L   ) acos(evaluate_tree(L));
#define ARCTAN_(L   ) atan(evaluate_tree(L));
#define ARCCTG_(L   ) M_PI / 2 - atan(evaluate_tree(L));

#define ARCSHN_(L   ) asinh(evaluate_tree(L));
#define ARCCHS_(L   ) acosh(evaluate_tree(L));
#define ARCTGH_(L   ) atanh(evaluate_tree(L));
#define ARCCTH_(L   ) 1 / atanh(evaluate_tree(L));

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

        case SIN: return SIN_(LEFT);
        case COS: return COS_(LEFT);
        case TAN: return TAN_(LEFT);
        case CTG: return CTG_(LEFT);

        case SHN: return SHN_(LEFT);
        case CHS: return CHS_(LEFT);
        case TGH: return TGH_(LEFT);
        case CTH: return CTH_(LEFT);

        case EXP: return EXP_(LEFT, RIGHT);
        case LOG: return LOG_(LEFT, RIGHT);

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

            case SIN: return diff_sin(doubled_tree, node);
            case COS: return diff_cos(doubled_tree, node);
            // case TAN: return diff_sin(doubled_tree, node);
            // case CTG: return diff_sin(doubled_tree, node);

            case SHN: return diff_shn(doubled_tree, node);
            case CHS: return diff_chs(doubled_tree, node);
            // case TGH: return diff_sin(doubled_tree, node);
            // case CTH: return diff_sin(doubled_tree, node);

            case EXP: return diff_exp(doubled_tree, node);
            // case LOG: return diff_log(doubled_tree, node);

            // case ARCSIN: return diff_arcsin(doubled_tree, node);
            // case ARCCOS: return diff_arccos(doubled_tree, node);
            // case ARCTAN: return diff_arctan(doubled_tree, node);
            // case ARCCTG: return diff_arcctg(doubled_tree, node);

            // case ARCSHN: return diff_arcshn(doubled_tree, node);
            // case ARCCHS: return diff_arcchs(doubled_tree, node);
            // case ARCTGH: return diff_arctgh(doubled_tree, node);
            // case ARCCTH: return diff_arccth(doubled_tree, node);

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
                                fprintf(output, text);\
                                latex_node(tree, R, output);\
                                fprintf(output, ")");
                                // fprintf(output, ")");
                                // fprintf(output, "(");

#define PREFIX_OUT(text, L, R)  fprintf(output, text);      \
                                fprintf(output, "{");       \
                                latex_node(tree, L, output);\
                                fprintf(output, "}{");      \
                                latex_node(tree, R, output);\
                                fprintf(output, "}");

#define UNAR_OUT(text, L)       fprintf(output, "\\");       \
                                fprintf(output, text);      \
                                fprintf(output, "{(");       \
                                latex_node(tree, L, output);\
                                fprintf(output, ")}");

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

                case SIN:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case COS:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case TAN:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case CTG:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;

                case SHN:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case CHS:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case TGH:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case CTH:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;

                case EXP:  INFIX_OUT(all_ops[(int) node->data].text, node->left, node->right); break;
                case LOG:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;

                case ARCSIN:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case ARCCOS:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case ARCCTG:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case ARCTAN:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;

                case ARCSHN:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case ARCCHS:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case ARCTGH:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;
                case ARCCTH:   UNAR_OUT(all_ops[(int) node->data].text, node->left); break;

                default: fprintf(stderr, "unknow operator in latex node");break;
            }
            break;
        }
        default:
            fprintf(stderr, "unknown operation in latex_node"); break;
    }

    return OK;
}

err_code_t reduce_equation(my_tree_t* to_reduce)
{
    bool is_changed = true;
    while (is_changed)
    {
        is_changed = false;
        equivalent(to_reduce, to_reduce->root, &is_changed);
        TREE_DUMP(to_reduce, to_reduce->root, "Now this is reduced tree");
    }

    return OK;
}

node_t* equivalent(my_tree_t* tree, node_t* node, bool* is_changed)
{
    node_t* to_ret = NULL;

    if (subtree_var_count(tree, node) == ZERO_VAR && node->type == OP)
    {
        TREE_DUMP(tree, node, "we are here calculating this");
        *is_changed = true;
        tree_val_t new_value = evaluate_tree(node);
        to_ret = new_node(tree, NUM, new_value, NULL, NULL);
        to_ret->parent = node->parent;
        if (node == tree->root) tree->root = to_ret;
        node_dtor(node);

        return to_ret;
    }

    if (node->type == OP && (int) node->data == MUL
        && ( node->right->type == NUM && is_double_equal(node->right->data, 0)
          || node->left->type  == NUM && is_double_equal(node->left->data,  0)))
    {
        *is_changed = true;
        TREE_DUMP(tree, node, "gonna changing this to null this");
        to_ret = new_node(tree, NUM, 0, NULL, NULL);
        to_ret->parent = node->parent;
        node_dtor(node);

        return to_ret;
    }

    if (node->type == OP && (int) node->data == MUL && (is_double_equal(node->right->data, 1)
                                                     || is_double_equal(node->left->data,  1)))
    {
        *is_changed = true;
        if (is_double_equal(node->left->data,  1)) to_ret = copy_subtree(tree, node->right);
        if (is_double_equal(node->right->data, 1)) to_ret = copy_subtree(tree, node->left);

        to_ret->parent = node->parent;
        node_dtor(node);

        return to_ret;
    }

    if (node->right != NULL) node->right = equivalent(tree, node->right, is_changed);
    if (node->left  != NULL) node->left  = equivalent(tree, node->left , is_changed);

    return node;
}

// bool check_is_operator(input)
// {
//
// }
