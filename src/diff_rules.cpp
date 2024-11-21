#include "diff_rules.h"
#include "differenciator.h"

#define DL node_t* left_diff  = differenciate(doubled_tree, node->left)
#define DR node_t* right_diff = differenciate(doubled_tree, node->right)
#define CL node_t* left_copy  = copy_subtree(doubled_tree, node->left)
#define CR node_t* right_copy = copy_subtree(doubled_tree, node->right)
#define CLN(No) node_t* left_copy##No  = copy_subtree(doubled_tree, node->left)
#define CRN(No) node_t* right_copy##No = copy_subtree(doubled_tree, node->right)

#define ADD_(L, R) new_node(doubled_tree, OP, ADD, L, R)
#define SUB_(L, R) new_node(doubled_tree, OP, SUB, L, R)
#define MUL_(L, R) new_node(doubled_tree, OP, MUL, L, R)
#define DIV_(L, R) new_node(doubled_tree, OP, DIV, L, R)

#define PARENT(var_name, macro, L, R) node_t* var_name = macro(L, R); R->parent = L->parent = var_name;

node_t* diff_add(my_tree_t* doubled_tree, node_t* node)
{
    DL; DR;
    PARENT(to_ret, ADD_, left_diff, right_diff);

    return to_ret;
}

node_t* diff_sub(my_tree_t* doubled_tree, node_t* node)
{
    DL; DR;
    PARENT(to_ret, SUB_, left_diff, right_diff);

    return to_ret;
}

node_t* diff_mul(my_tree_t* doubled_tree, node_t* node)
{
    DL; DR; CL; CR;
    PARENT(mul_1, MUL_, left_diff, right_copy);
    PARENT(mul_2, MUL_, left_copy, right_diff);
    PARENT(to_ret, ADD_, mul_1, mul_2);

    return to_ret;
}

node_t* diff_div(my_tree_t* doubled_tree, node_t* node)
{
    DL; DR; CRN(1); CRN(2); CRN(3); CL;

    PARENT(mul_1, MUL_, left_diff, right_copy1);
    PARENT(mul_2, MUL_, right_diff, left_copy);
    PARENT(chisl, SUB_, mul_1, mul_2);
    PARENT(znam_sq, MUL_, right_copy2, right_copy3);

    PARENT(to_ret, DIV_, chisl, znam_sq);

    return to_ret;
}

node_t* diff_cos(my_tree_t* doubled_tree, node_t* node)
{
    // PARENT(sinus,  SIN_, );
    // PARENT(to_ret, SUB_, NULL, (void*)NULL);
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_sin(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_sh(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_ch(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_tg(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_th(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_ctg(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_cth(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_arcsin(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_arccos(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_arctg(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_arcctg(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_exp(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

node_t* diff_pow(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    return to_ret;
}

