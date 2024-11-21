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
    printf("%s\n", buffer);

    my_tree_t expr_tree = make_tree(buffer);
    TREE_DUMP(&expr_tree, expr_tree.root, "This is tree from chitalka");
    // my_tree_t test_tree = {};

    printf("evaluated value is %lg\n", evaluate_tree(expr_tree.root));

    INIT_TREE(diff_tree);
    free(diff_tree.root);
    diff_tree.root = differenciate(&diff_tree, expr_tree.root);
    TREE_DUMP(&diff_tree, diff_tree.root, "this is tree has size = %zu", diff_tree.size);
    latex_node(&diff_tree, diff_tree.root, stdout);

    tree_dtor(&expr_tree);
    tree_dtor(&diff_tree);
    printf("\n");
    free(buffer);
    disable_logging();

    return OK;
}
