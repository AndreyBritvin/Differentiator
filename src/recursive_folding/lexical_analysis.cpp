#include "lexical_analysis.h"
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

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

node_t* fill_node(char * buffer, size_t* position, my_tree_t* tree, node_t* parent)
{
    assert(buffer);
    assert(position);
    assert(tree);

    (*position)++;

    size_t len_of_expr = 0;
    tree_val_t expression = 0;
    char funcname[MAX_STRING_SIZE] = {};

    // printf("Type = %d ", sscanf(buffer + *position, "%[^()]%ln", &funcname, &len_of_expr));
    // printf("Get_funcname = %d\n", get_func_num(funcname));

    op_type_t var_type = NUM;
    if (sscanf(buffer + *position, "%lf%ln", &expression, &len_of_expr) > 0)
    {
        var_type = NUM;
    }
    else if (sscanf(buffer + *position, "%[^()]%ln", &funcname, &len_of_expr) >= 1
             && get_func_num(funcname) != UNKNOWN)
    {
        var_type = OP;
        expression = (tree_val_t) get_func_num(funcname);
    }
    else if (sscanf(buffer + *position, "%c%ln", &funcname, &len_of_expr))
    {
        var_type = VAR;
        expression = (tree_val_t) funcname[0];
    }
    else
    {
        fprintf(stderr, "No sscanf dont work\n");
    }

    *position += len_of_expr;

    tree->size++;
    node_t* node_to_return = new_node(tree, var_type, expression, NULL, NULL);
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

int get_func_num(char* input)
{
    // printf("%s\n", input);
    // printf("All_ops size = %d\n", sizeof(all_ops));

    for (size_t i = 0; i < sizeof(all_ops) / sizeof(operation); i++)
    {
        if (!strcmp(all_ops[i].text, input))
        {
            return i;
        }
    }

    return UNKNOWN;
}
