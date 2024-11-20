#include "differenciator.h"
#include <assert.h>
#include "utils.h"
#include <stdio.h>
#include "my_tree.h"
#include <string.h>
#include <sys/stat.h>

// func to read from file  to buffer
size_t get_file_len(const char *filename)
{
    assert(filename != NULL);

    struct stat st = {};
    stat(filename, &st);

    return (size_t) st.st_size;
}

err_code_t fill_buffer(char **buffer_to_fill, const char* filename)
{
    size_t filesize = get_file_len(filename);
    char *temp_buf = (char *) calloc(filesize + 1, sizeof(char));
    if (temp_buf == NULL)
    {
        return ERROR_CALLOC_IS_NULL;
    }
    *buffer_to_fill = temp_buf;

    FILE* SAFE_OPEN_FILE(input_file, filename, "r");

    fread(temp_buf, 1, filesize, input_file);
    fclose(input_file);

    return OK;
}

my_tree_t make_tree(char *buffer)
{
    assert(buffer);

    INIT_TREE(tree_to_fill);

    free(tree_to_fill.root);

    size_t position = 0;
    tree_to_fill.root = fill_node(buffer, &position, &tree_to_fill, NULL);
    TREE_DUMP(&tree_to_fill, tree_to_fill.root, "I am gROOT (generated this tree after reading file)");

    return tree_to_fill;
}

node_t* fill_node(char * buffer, size_t* position, my_tree_t* tree, node_t* parent)
{
    assert(buffer);
    assert(position);
    assert(tree);

    (*position)++;

    size_t len_of_expr = 0;
    tree_val_t expression = 0;

    sscanf(buffer + *position, " %lf%ln", &expression, &len_of_expr);

    *position += len_of_expr;

    tree->size++;
    node_t* node_to_return = new_node(tree, 0, 0, NULL, NULL); // TODO: potential error via 0
    node_to_return->parent = parent;
    if (*position == 1)
    {
        tree->root = node_to_return;
    }

    bool is_left = true;

    while (buffer[*position] != ')')
    {
        // TREE_DUMP(tree, node_to_return, "Working with this\n Input text = %s", expression);
        // printf("Current char is %c position %zu\n", buffer[*position], *position);
        if (buffer[*position] == '(' && is_left)
        {
            is_left = false;
            node_to_return->left = fill_node(buffer, position, tree, node_to_return);
        }

        else if (buffer[*position] == '(' && !is_left)
        {
            node_to_return->right = fill_node(buffer, position, tree, node_to_return);
        }

        else
        {
            fprintf(stderr, "Some error happened in brackets\n");
        }

        (*position)++;
    }

    return node_to_return;
}

// bool check_is_operator(input)
// {
//
// }
