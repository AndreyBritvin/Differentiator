#ifndef DIFF_RULES_H_
#define DIFF_RULES_H_

#include "my_log.h"
#include "utils.h"
#include "op_types.h"
#include "my_tree.h"

node_t* diff_add(my_tree_t* doubled_tree, node_t* node);
node_t* diff_sub(my_tree_t* doubled_tree, node_t* node);
node_t* diff_mul(my_tree_t* doubled_tree, node_t* node);
node_t* diff_div(my_tree_t* doubled_tree, node_t* node);
node_t* diff_cos(my_tree_t* doubled_tree, node_t* node);
node_t* diff_sin(my_tree_t* doubled_tree, node_t* node);
node_t* diff_sh(my_tree_t* doubled_tree, node_t* node);
node_t* diff_ch(my_tree_t* doubled_tree, node_t* node);
node_t* diff_tg(my_tree_t* doubled_tree, node_t* node);
node_t* diff_th(my_tree_t* doubled_tree, node_t* node);
node_t* diff_ctg(my_tree_t* doubled_tree, node_t* node);
node_t* diff_cth(my_tree_t* doubled_tree, node_t* node);
node_t* diff_arcsin(my_tree_t* doubled_tree, node_t* node);
node_t* diff_arccos(my_tree_t* doubled_tree, node_t* node);
node_t* diff_arctg(my_tree_t* doubled_tree, node_t* node);
node_t* diff_arcctg(my_tree_t* doubled_tree, node_t* node);
node_t* diff_exp(my_tree_t* doubled_tree, node_t* node);
node_t* diff_pow(my_tree_t* doubled_tree, node_t* node);

#endif // DIFF_RULES_H_
