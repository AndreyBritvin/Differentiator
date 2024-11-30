#ifndef LATEX_OUTPUT_H_
#define LATEX_OUTPUT_H_
#include "my_tree.h"
#include "utils.h"
#include <stdio.h>

enum latex_output_mode
{
    RECURSION_BEGIN = 0,
    GRAPH_MODE,
    FORMULA_MODE,
    DIFF_MODE,
    DIFF,
    COPY,
};

node_t** latex_node(my_tree_t* tree, node_t* node, FILE* output, latex_output_mode is_graph_mode, size_t recurs_level);
err_code_t enable_latex_output(const char* filename, const char* filejokes);
err_code_t disable_latex_output();
err_code_t print_equation(my_tree_t* tree, node_t* node_before, node_t* node_after);
err_code_t print_introduction();
err_code_t print_ending();
err_code_t free_all_jokes();
err_code_t fill_jokes(const char* filename);
size_t get_lines_num(char* buffer, size_t all_len);
err_code_t paste_graph(my_tree_t* tree, node_t* node);
err_code_t paste_taylor(my_tree_t* tree, node_t* node);

err_code_t print_equation_begining(my_tree_t* tree, node_t* node_before, const char* text);
err_code_t print_equation_ending(my_tree_t* tree, node_t* node_before, const char* text, latex_output_mode is_diff);
err_code_t paste_two_graphs(my_tree_t* tree_1, my_tree_t* tree_2, tree_val_t x0);

err_code_t print_subtrees(my_tree_t* tree, FILE* output, node_t** subtrees, latex_output_mode is_diff);

#endif // LATEX_OUTPUT_H_
