#include "differenciator.h"
#include "my_tree.h"
#include "my_log.h"
#include "utils.h"
#include <stdio.h>
#include <locale.h>
#include "op_types.h"

int main(const int argc, const char** argv)
{
    enable_logging("tree_dump/differenciator.html");

    char *buffer = 0;
    fill_buffer(&buffer, "expressions/expr_1.txt");
    // printf("%s\n", buffer);

    // my_tree_t expr_tree = make_tree(buffer);

    // my_tree_t test_tree = {};
    INIT_TREE(test_tree);
    free(test_tree.root);

    node_t* node_5 = new_node(&test_tree, NUM, 6  , NULL, NULL);
    node_t* node_6 = new_node(&test_tree, VAR, 'x', NULL, NULL);

    node_t* node_3 = new_node(&test_tree, NUM, 30, NULL, NULL);
    node_t* node_4 = new_node(&test_tree, NUM, 12, NULL, NULL);

    node_t* node_1 = new_node(&test_tree, OP, ADD, node_3, node_4);
    node_t* node_2 = new_node(&test_tree, OP, SUB, node_5, node_6);
    test_tree.root = new_node(&test_tree, OP, DIV, node_1, node_2);
    // test_tree.size = 7;
    node_5->parent = node_6->parent = node_2;
    node_3->parent = node_4->parent = node_1;
    node_1->parent = node_2->parent = test_tree.root;

    TREE_DUMP(&test_tree, test_tree.root, "this is tree");

    printf("evaluated value is %lg\n", evaluate_tree(test_tree.root));

    INIT_TREE(diff_tree);
    free(diff_tree.root);
    diff_tree.root = differenciate(&diff_tree, test_tree.root);
    TREE_DUMP(&diff_tree, diff_tree.root, "this is tree has size = %zu", diff_tree.size);
    latex_node(&test_tree, test_tree.root, stdout);
    tree_dtor(&test_tree);
    tree_dtor(&diff_tree);

    free(buffer);
    disable_logging();

    return OK;
}
