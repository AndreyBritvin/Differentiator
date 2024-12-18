#ifndef AKINATOR_H_
#define AKINATOR_H_
#include "utils.h"
#include "op_types.h"
#include "diff_rules.h"
#include "latex_output.h"
#include "my_tree.h"
#include "recursive_descent_parser.h"

const int MAX_STRING_SIZE = 128;

static tree_val_t Global_X = 3;

tree_val_t evaluate_tree(node_t* node);
node_t* differenciate(my_tree_t* doubled_tree, node_t* node);

int get_func_num(char* input);

my_tree_t get_taylor_series(my_tree_t* expr_tree, tree_val_t x0, size_t amount);
err_code_t add_taylor_coeff(my_tree_t* taylor_tree, tree_val_t x0, size_t power, tree_val_t coef);
node_t* find_zero_node(my_tree_t* taylor_tree, node_t* curr_node);

unsigned long long factorial(size_t number);

#endif // AKINATOR_H_
