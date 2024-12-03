#ifndef LEXICAL_ANALYSIS_H_
#define LEXICAL_ANALYSIS_H_

#include "op_types.h"
#include "differenciator.h"

#define MAXIMUM_LEXEMS_COUNT 100

size_t get_file_len(const char *filename);
err_code_t fill_buffer(char **buffer_to_fill, const char* filename);
my_tree_t make_tree(char *buffer);
node_t* fill_node(char * buffer, size_t* position, my_tree_t* tree, node_t* parent);

struct tokens
{
    node_type  type;
    tree_val_t value;
    size_t     line;
    size_t     column;
};

#endif // LEXICAL_ANALYSIS_H_
