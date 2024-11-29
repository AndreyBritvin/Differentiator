#include "simplificator.h"
#include <assert.h>
#include "differenciator.h"

err_code_t reduce_equation(my_tree_t* to_reduce)
{
    assert(to_reduce);

    bool is_changed = true;
    while (is_changed)
    {
        is_changed = false;
        TREE_DUMP(to_reduce, to_reduce->root, "Before reducing in reduce function. Size = %zu", to_reduce->size);
        to_reduce->root = simplify_tree(to_reduce, to_reduce->root, &is_changed);
        TREE_DUMP(to_reduce, to_reduce->root, "Now this is reduced tree");
        break;
    }

    return OK;
}

node_t* simplify_tree(my_tree_t* tree, node_t* node, bool* is_changed)
{
    assert(tree);
    assert(node);
    assert(is_changed);

    if (node->right != NULL) node->right = simplify_tree(tree, node->right, is_changed);
    if (node->left  != NULL) node->left  = simplify_tree(tree, node->left , is_changed);

    if (subtree_var_count(tree, node) == ZERO_VAR && node->type == OP)
    {
        return constant_folding(tree, node, is_changed);
    }

    if (node->type == OP && (int) node->data == MUL
        && ( node->right->type == NUM && is_double_equal(node->right->data, 0)
          || node->left->type  == NUM && is_double_equal(node->left->data,  0)))
    {
        // TREE_DUMP(tree, node, "something will happened mul0");
        return  mul_0_folding(tree, node, is_changed);
    }

    if (node->type == OP && (int) node->data == MUL
        && ( node->right->type == NUM && is_double_equal(node->right->data, 1)
          || node->left->type  == NUM && is_double_equal(node->left->data,  1)))
    {
        // TREE_DUMP(tree, node, "something will happened mul1");
        return mul_1_folding(tree, node, is_changed, 1);
    }

    if (node->type == OP && (int) node->data == EXP
        && ( node->right->type == NUM && is_double_equal(node->right->data, 1)
          || node->left->type  == NUM && is_double_equal(node->left->data,  1)))
    {
        // TREE_DUMP(tree, node, "something will happened exp1");
        return mul_1_folding(tree, node, is_changed, 1); // not error that mul; same as * 1 == ^ 1
    }

    if (node->type == OP && (int) node->data == EXP
        && ( node->right->type == NUM && is_double_equal(node->right->data, 0)
          || node->left->type  == NUM && is_double_equal(node->left->data,  0)))
    {
        // TREE_DUMP(tree, node, "something will happened exp0");
        return pow_0_folding(tree, node, is_changed);
    }

    if (node->type == OP && ((int) node->data == ADD || (int) node->data == SUB)
        && ( node->right->type == NUM && is_double_equal(node->right->data, 0)
          || node->left->type  == NUM && is_double_equal(node->left->data,  0)))
    {
        // TREE_DUMP(tree, node, "something will happened +-0");
        return mul_1_folding(tree, node, is_changed, 0);
    }
    // 0-...
    // /1
    // 0^
    // 1^

    return node;
}

node_t* constant_folding(my_tree_t* tree, node_t* node, bool* is_changed)
{
    assert(tree);
    assert(node);
    assert(is_changed);

    node_t* to_ret = NULL;
    // TREE_DUMP(tree, node, "we are here calculating this");
    *is_changed = true;
    tree_val_t new_value = evaluate_tree(node);
    to_ret = new_node(tree, NUM, new_value, NULL, NULL);
    to_ret->parent = node->parent;

    if (node == tree->root) tree->root = to_ret;
    node_dtor(node);

    return to_ret;
}

node_t* mul_1_folding(my_tree_t* tree, node_t* node, bool* is_changed, tree_val_t num_to_cmp)
{
    assert(tree);
    assert(node);
    assert(is_changed);

    node_t* to_ret = NULL;
    *is_changed = true;
    if      (node->left->type == NUM
        &&   is_double_equal(node->left->data,  num_to_cmp)) to_ret = copy_subtree(tree, node->right);
    else if (node->right->type == NUM
        &&   is_double_equal(node->right->data, num_to_cmp)) to_ret = copy_subtree(tree, node->left);

    if (node == tree->root) tree->root = to_ret;
    to_ret->parent = node->parent;
    // TREE_DUMP(tree, node, "replacing this node with input data = %lf", num_to_cmp);
    node_dtor(node);

    return to_ret;
}

node_t* mul_0_folding(my_tree_t* tree, node_t* node, bool* is_changed)
{
    assert(tree);
    assert(node);
    assert(is_changed);

    node_t* to_ret = NULL;
    *is_changed = true;
    // TREE_DUMP(tree, node, "gonna changing this to zero this");
    to_ret = new_node(tree, NUM, 0, NULL, NULL);
    if (node == tree->root) tree->root = to_ret;

    to_ret->parent = node->parent;
    node_dtor(node);

    return to_ret;
}

node_t* pow_0_folding(my_tree_t* tree, node_t* node, bool* is_changed)
{
    assert(tree);
    assert(node);
    assert(is_changed);

    node_t* to_ret = NULL;
    *is_changed = true;
    to_ret = new_node(tree, NUM, 1, NULL, NULL);
    if (node == tree->root) tree->root = to_ret;

    to_ret->parent = node->parent;
    node_dtor(node);

    return to_ret;
}
