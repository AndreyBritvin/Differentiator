#ifndef AKINATOR_H_
#define AKINATOR_H_
#include "utils.h"
#include "op_types.h"
#include "my_tree.h"

const int MAX_STRING_SIZE = 128;

static tree_val_t Global_X = 3;

size_t get_file_len(const char *filename);
err_code_t fill_buffer(char **buffer_to_fill, const char* filename);
my_tree_t make_tree(char *buffer);
node_t* fill_node(char * buffer, size_t* position, my_tree_t* tree, node_t* parent);


#endif // AKINATOR_H_
