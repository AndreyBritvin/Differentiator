#ifndef AKINATOR_H_
#define AKINATOR_H_
#include "utils.h"
#include "op_types.h"
#include "diff_rules.h"
#include "my_tree.h"

const int MAX_STRING_SIZE = 128;

static tree_val_t Global_X = 3;

size_t get_file_len(const char *filename);
err_code_t fill_buffer(char **buffer_to_fill, const char* filename);
my_tree_t make_tree(char *buffer);
node_t* fill_node(char * buffer, size_t* position, my_tree_t* tree, node_t* parent);

tree_val_t evaluate_tree(node_t* node);
node_t* copy_subtree(my_tree_t* main_tree, node_t* node);
node_t* differenciate(my_tree_t* doubled_tree, node_t* node);

err_code_t latex_node(my_tree_t* tree, node_t* node, FILE* output);
err_code_t latex_output(my_tree_t* tree, const char* filename);

int get_func_num(char* input);

err_code_t reduce_equation(my_tree_t* to_reduce);
node_t* equivalent(my_tree_t* tree, node_t* node, bool* is_changed);

#endif // AKINATOR_H_
