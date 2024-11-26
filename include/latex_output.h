#ifndef LATEX_OUTPUT_H_
#define LATEX_OUTPUT_H_
#include "my_tree.h"
#include "utils.h"
#include <stdio.h>

err_code_t latex_node(my_tree_t* tree, node_t* node, FILE* output);
err_code_t enable_latex_output(const char* filename, const char* filejokes);
err_code_t disable_latex_output();
err_code_t print_equation(my_tree_t* tree, node_t* node_before, node_t* node_after);
err_code_t print_introduction();
err_code_t print_ending();
err_code_t free_all_jokes();
err_code_t fill_jokes(const char* filename);
size_t get_lines_num(char* buffer, size_t all_len);
err_code_t paste_graph(my_tree_t* tree, node_t* node);

#endif // LATEX_OUTPUT_H_
