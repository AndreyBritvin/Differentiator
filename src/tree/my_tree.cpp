#include "my_tree.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "my_log.h"

node_t* new_node(my_tree_t* tree, op_type_t type, tree_val_t data, node_t* left_node, node_t* right_node)
{
    assert(tree);

    node_t* node = (node_t*) calloc(1, sizeof(node_t));
    node->type  =       type;
    node->data  =       data;
    node->left  =  left_node;
    node->right = right_node;

    tree->size += 1;

    return node;
}

err_code_t tree_ctor(my_tree_t* tree)
{
    tree->size = 1;
    tree->root = new_node(tree, 0, 0, NULL, NULL);

    return OK;
}

err_code_t tree_dtor(my_tree_t* tree)
{
    CHECK_TREE(tree);
    node_dtor(tree->root);

    return OK;
}

err_code_t node_dtor(node_t* node)
{
    assert(node);

    if (node->left  != NULL) node_dtor(node->left);
    if (node->right != NULL) node_dtor(node->right);

    free(node);

    return OK;
}

err_code_t print_tree(my_tree_t* tree)
{
    CHECK_TREE(tree);

    print_node(tree->root);

    return OK;
}

err_code_t print_node(node_t* tree)
{
    if (tree == NULL) return ERROR_TREE_IS_NULL;

    printf("(");
    if (tree->left  != NULL) print_node(tree->left);
    printf("%lg", tree->data);
    if (tree->right != NULL) print_node(tree->right);
    printf(")");

    return OK;
}

err_code_t verificator(my_tree_t* tree, node_t* node, size_t recurs_level)
{
    assert(tree);

    // size_t index_to_append = 0;
    // printf("Tree size is %zd, curr recurs is %zd\n", tree->size, recurs_level);
    if (recurs_level > tree->size)
    {
        return ERROR_TREE_LOOPED;
    }

    if (node->left  != NULL && node->left ->parent != node) return ERROR_PARENT_DONT_MATCH;
    if (node->right != NULL && node->right->parent != node) return ERROR_PARENT_DONT_MATCH;

    err_code_t err_code = OK;
    if (node->left  != NULL)                   err_code = verificator(tree, node->left,  recurs_level + 1);
    if (node->right != NULL && err_code == OK) err_code = verificator(tree, node->right, recurs_level + 1);

    return err_code;
}

node_t* copy_subtree(my_tree_t* main_tree, node_t* node)
{
    assert(main_tree);
    assert(node);

    node_t* copy_node = new_node(main_tree, node->type, node->data, NULL, NULL);

    node_t* left_sub  = NULL;
    node_t* right_sub = NULL;

    if (node->left  != NULL)
    {
        left_sub  = copy_subtree(main_tree, node->left);
        left_sub->parent = copy_node;
    }
    if (node->right != NULL)
    {
        right_sub = copy_subtree(main_tree, node->right);
        right_sub->parent = copy_node;
    }

    copy_node->left  = left_sub;
    copy_node->right = right_sub;

    return copy_node;
}

int subtree_var_count(my_tree_t* tree, node_t* node)
{
    if (node->type == VAR) return ONE_VAR;
    if (node->left  != NULL && subtree_var_count(tree, node->left)  == ONE_VAR) return ONE_VAR;
    if (node->right != NULL && subtree_var_count(tree, node->right) == ONE_VAR) return ONE_VAR;

    return ZERO_VAR;
}
