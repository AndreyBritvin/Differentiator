#include "diff_rules.h"
#include "differenciator.h"
#include "latex_output.h"
#include "math.h"

#define NUM_(nums, num_of_num) node_t* num##num_of_num = new_node(doubled_tree, NUM, nums, NULL, NULL);
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
#define SIN_(L   ) new_node(doubled_tree, OP, SIN, L, NULL)
#define COS_(L   ) new_node(doubled_tree, OP, COS, L, NULL)
#define SHN_(L   ) new_node(doubled_tree, OP, SHN, L, NULL)
#define CHS_(L   ) new_node(doubled_tree, OP, CHS, L, NULL)
#define EXP_(L, R) new_node(doubled_tree, OP, EXP, L, R)
#define LOG_(L   ) new_node(doubled_tree, OP, LOG, L, NULL)


#define      PARENT(var_name, macro, L, R) node_t* var_name = macro(L, R); R->parent = L->parent = var_name;
#define UNAR_PARENT(var_name, macro, L)    node_t* var_name = macro(L);                L->parent = var_name;

node_t* diff_add(my_tree_t* doubled_tree, node_t* node)
{
    DL; DR; //CLN(for_dump); CRN(for_dump);
    // PARENT(original_node, ADD_, left_copyfor_dump, right_copyfor_dump);

    // print_equation_begining(doubled_tree, original_node, "Сумма раскладывается так:");

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
    DL; DR; CRN(1); CRN(2); CL;//CRN(3);

    PARENT(mul_1, MUL_, left_diff, right_copy1);
    PARENT(mul_2, MUL_, right_diff, left_copy);
    PARENT(chisl, SUB_, mul_1, mul_2);
    NUM_(2, square);
    PARENT(znam_sq, EXP_, right_copy2, numsquare);

    PARENT(to_ret, DIV_, chisl, znam_sq);

    return to_ret;
}

node_t* diff_cos(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(-1, 1);
    UNAR_PARENT(sinus, SIN_, left_copy);
    PARENT(actual_sinus, MUL_, num1, sinus);
    PARENT(to_ret, MUL_, actual_sinus, left_diff);

    return to_ret;
}

node_t* diff_sin(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;

    UNAR_PARENT(cosinus, COS_, left_copy);
    PARENT(to_ret, MUL_, cosinus, left_diff);

    return to_ret;
}

node_t* diff_shn(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;

    UNAR_PARENT(cosinus, CHS_, left_copy);
    PARENT(to_ret, MUL_, cosinus, left_diff);

    return to_ret;
}

node_t* diff_chs(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;

    UNAR_PARENT(cosinus, SHN_, left_copy);
    PARENT(to_ret, MUL_, cosinus, left_diff);

    return to_ret;
}

node_t* diff_tan(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(2, power)
    UNAR_PARENT(secans, COS_, left_copy);
    PARENT(secans_square, EXP_, secans, numpower);
    PARENT(to_ret, DIV_, left_diff, secans_square);

    return to_ret;
}

node_t* diff_tgh(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(2, power)
    UNAR_PARENT(secans, CHS_, left_copy);
    PARENT(secans_square, EXP_, secans, numpower);
    PARENT(to_ret, DIV_, left_diff, secans_square);

    return to_ret;
}

node_t* diff_ctg(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(2, power);
    NUM_(-1, negative);
    UNAR_PARENT(secans, SIN_, left_copy);
    PARENT(secans_square, EXP_, secans, numpower);
    PARENT(almost_answer, DIV_, left_diff, secans_square);
    PARENT(to_ret, MUL_, numnegative, almost_answer);

    return to_ret;
}

node_t* diff_cth(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(2, power);
    NUM_(-1, negative);
    UNAR_PARENT(secans, SHN_, left_copy);
    PARENT(secans_square, EXP_, secans, numpower);
    PARENT(almost_answer, DIV_, left_diff, secans_square);
    PARENT(to_ret, MUL_, numnegative, almost_answer);

    return to_ret;
}

node_t* diff_arcsin(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(2, power);
    NUM_(0.5, sqrt_val);
    NUM_(1, one);
    PARENT(arg_square, EXP_, left_copy, numpower);
    PARENT(underroot, SUB_, numone, arg_square);
    PARENT(root, EXP_, underroot, numsqrt_val);
    PARENT(to_ret, DIV_, left_diff, root);

    return to_ret;
}

node_t* diff_arccos(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(2, power);
    NUM_(0.5, sqrt_val);
    NUM_(1, one);
    PARENT(arg_square, EXP_, left_copy, numpower);
    PARENT(underroot, SUB_, numone, arg_square);
    PARENT(root, EXP_, underroot, numsqrt_val);
    PARENT(division, DIV_, left_diff, root);

    NUM_(-1, negative);
    PARENT(to_ret, MUL_, numnegative, division);

    return to_ret;
}

node_t* diff_arctan(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(1, one);
    NUM_(2, power);
    PARENT(fsquare, EXP_, left_copy, numpower);
    PARENT(znam, ADD_, numone, fsquare);
    PARENT(to_ret, DIV_, left_diff, znam);

    return to_ret;
}

node_t* diff_arcctg(my_tree_t* doubled_tree, node_t* node)
{
    node_t* left_diff = diff_arctan(doubled_tree, node);
    NUM_(-1, minus_one);
    PARENT(to_ret, MUL_, numminus_one, left_diff);

    return to_ret;
}


node_t* diff_arcshn(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(2, power);
    NUM_(0.5, sqrt_val);
    NUM_(1, one);
    PARENT(arg_square, EXP_, left_copy, numpower);
    PARENT(underroot, ADD_, numone, arg_square);
    PARENT(root, EXP_, underroot, numsqrt_val);
    PARENT(to_ret, DIV_, left_diff, root);

    return to_ret;
}

node_t* diff_arcchs(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(2, power);
    NUM_(0.5, sqrt_val);
    NUM_(1, one);
    PARENT(arg_square, EXP_, left_copy, numpower);
    PARENT(underroot, SUB_, arg_square, numone);
    PARENT(root, EXP_, underroot, numsqrt_val);
    PARENT(to_ret, DIV_, left_diff, root);

    return to_ret;
}

node_t* diff_arctgh(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(1, one);
    NUM_(2, power);
    PARENT(fsquare, EXP_, left_copy, numpower);
    PARENT(znam, SUB_, numone, fsquare);
    PARENT(to_ret, DIV_, left_diff, znam);

    return to_ret;
}

node_t* diff_arccth(my_tree_t* doubled_tree, node_t* node)
{
    return diff_arctgh(doubled_tree, node);
}


node_t* diff_exp(my_tree_t* doubled_tree, node_t* node)
{
    node_t* to_ret = NULL;

    int func_left  = subtree_var_count(doubled_tree, node->left);
    int func_right = subtree_var_count(doubled_tree, node->right);
    if (func_left == ZERO_VAR && func_right == ONE_VAR)
    {
        to_ret = diff_exponential(doubled_tree, node);
    }
    if (func_left == ONE_VAR && func_right == ONE_VAR)
    {
        to_ret = diff_pow_exp(doubled_tree, node);
    }
    if (func_left == ONE_VAR && func_right == ZERO_VAR)
    {
        to_ret = diff_pow(doubled_tree, node);
    }
    if (func_left == ZERO_VAR && func_right == ZERO_VAR)
    {
        NUM_(0, 1);
        to_ret = num1;
    }

    return to_ret;
}

node_t* diff_pow(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;
    NUM_(node->right->data - 1, _right);
    NUM_(node->right->data, _left);
    PARENT(another_pow, EXP_, left_copy, num_right);
    PARENT(pow_diff, MUL_, num_left, another_pow);
    PARENT(to_ret, MUL_, pow_diff, left_diff);

    return to_ret;
}

node_t* diff_pow_exp(my_tree_t* doubled_tree, node_t* node)
{
    DL; DR; CRN(1); CRN(2); CLN(1); CLN(2); CLN(3);
    PARENT(original, EXP_, left_copy1, right_copy1);

    UNAR_PARENT(logarithm, LOG_, left_copy2);
    PARENT(second_add, MUL_, logarithm, right_diff);

    PARENT(division, DIV_, right_copy2, left_copy3);
    PARENT(first_add, MUL_, division, left_diff);

    PARENT(second_diff, ADD_, first_add, second_add);
    PARENT(to_ret, MUL_, original, second_diff);

    return to_ret;
}

node_t* diff_exponential(my_tree_t* doubled_tree, node_t* node)
{
    DR; CR;
    NUM_(node->left->data, _orig);
    NUM_(log(node->left->data), _ln);
    PARENT(orig_pow, EXP_, num_orig, right_copy);
    PARENT(ln_mul, MUL_, orig_pow, num_ln);
    PARENT(to_ret, MUL_, ln_mul, right_diff);

    return to_ret;
}

node_t* diff_log(my_tree_t* doubled_tree, node_t* node)
{
    DL; CL;

    PARENT(to_ret, DIV_, left_diff, left_copy);

    return to_ret;
}
