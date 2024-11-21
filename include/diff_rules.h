#ifndef DIFF_RULES_H_
#define DIFF_RULES_H_

#include "my_log.h"
#include "utils.h"
#include "op_types.h"
#include "my_tree.h"

enum vars_count
{
    ZERO_VAR,
    ONE_VAR,
};

node_t* diff_add(my_tree_t* doubled_tree, node_t* node);
node_t* diff_sub(my_tree_t* doubled_tree, node_t* node);
node_t* diff_mul(my_tree_t* doubled_tree, node_t* node);
node_t* diff_div(my_tree_t* doubled_tree, node_t* node);
node_t* diff_cos(my_tree_t* doubled_tree, node_t* node);
node_t* diff_sin(my_tree_t* doubled_tree, node_t* node);
node_t* diff_shn(my_tree_t* doubled_tree, node_t* node);
node_t* diff_chs(my_tree_t* doubled_tree, node_t* node);
node_t* diff_tan(my_tree_t* doubled_tree, node_t* node);
node_t* diff_tgh(my_tree_t* doubled_tree, node_t* node);
node_t* diff_ctg(my_tree_t* doubled_tree, node_t* node);
node_t* diff_cth(my_tree_t* doubled_tree, node_t* node);
node_t* diff_arcsin(my_tree_t* doubled_tree, node_t* node);
node_t* diff_arccos(my_tree_t* doubled_tree, node_t* node);
node_t* diff_arctg(my_tree_t* doubled_tree, node_t* node);
node_t* diff_arcctg(my_tree_t* doubled_tree, node_t* node);
node_t* diff_exp(my_tree_t* doubled_tree, node_t* node);
node_t* diff_pow(my_tree_t* doubled_tree, node_t* node);
node_t* diff_pow_exp(my_tree_t* doubled_tree, node_t* node);
node_t* diff_exponential(my_tree_t* doubled_tree, node_t* node);

int subtree_var_count(my_tree_t* tree, node_t* node);

#endif // DIFF_RULES_H_
