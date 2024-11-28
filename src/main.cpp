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
    enable_latex_output("tree_dump/latex.tex", "expressions/differenciate_obvious.txt");
    print_introduction();

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

    reduce_equation(&expr_tree);
    reduce_equation(&diff_tree);

    printf("Latex of differrinciate equation:\n");
    print_equation(&diff_tree, expr_tree.root, diff_tree.root);
    latex_node(&diff_tree, diff_tree.root, stdout, FORMULA_MODE);
    printf("\n");

    printf("Latex of first equation:\n");
    latex_node(&expr_tree, expr_tree.root, stdout, FORMULA_MODE);
    printf("\n");
    paste_graph(&expr_tree, expr_tree.root);

    my_tree_t taylor_tree = get_taylor_series(&expr_tree, 0.5, 2);
    TREE_DUMP(&taylor_tree, taylor_tree.root, "This is taylor tree");
    paste_taylor(&taylor_tree, taylor_tree.root);

    print_ending();

    tree_dtor(&taylor_tree);
    tree_dtor(&expr_tree);
    tree_dtor(&diff_tree);
    free(buffer);

    disable_latex_output();
    disable_logging();

    return OK;
}
