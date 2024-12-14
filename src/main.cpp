#include "differenciator.h"
#include "simplificator.h"
#include "recursive_descent_parser.h"
#include "my_tree.h"
#include "my_log.h"
#include "utils.h"
#include <stdio.h>
#include <locale.h>
#include "op_types.h"

int main(const int argc, const char** argv)
{
    if (argc != 2)
    {
        printf("Sorry, enter:\n%s input_filename.txt\n", argv[0]);
        return EXIT_SUCCESS;
    }

    enable_logging("tree_dump/differenciator.html");
    enable_latex_output("tree_dump/latex.tex", "expressions/differenciate_obvious.txt");
    print_introduction();

    char *buffer = 0;
    fill_buffer(&buffer, argv[1]);
    printf("%s\n", buffer);

    my_tree_t expr_tree = make_tree(buffer);
    TREE_DUMP(&expr_tree, expr_tree.root, "This is tree from chitalka");
    // my_tree_t test_tree = {};
    print_equation_begining(&expr_tree, expr_tree.root, "Давайте продифференцируем это выражение");

    printf("evaluated value is %lg\n", evaluate_tree(expr_tree.root));

    // tree_dtor(&expr_tree);
    // return 0;

    INIT_TREE(diff_tree);
    free(diff_tree.root);
    diff_tree.root = differenciate(&diff_tree, expr_tree.root);
    TREE_DUMP(&diff_tree, diff_tree.root, "Before generating subtrees", diff_tree.size);
    generate_subtrees(&diff_tree, diff_tree.root, RECURSION_BEGIN);
    TREE_DUMP(&diff_tree, diff_tree.root, "After generating subtrees", diff_tree.size);
    remove_subtrees(&diff_tree, diff_tree.root);
    TREE_DUMP(&diff_tree, diff_tree.root, "After removing subtrees", diff_tree.size);

    reduce_equation(&expr_tree);
    reduce_equation(&diff_tree);
    print_equation_begining(&expr_tree, expr_tree.root, "Исходное выражение");

    print_equation_begining(&diff_tree, diff_tree.root, "Итоговый ответ: \n");

    borders graph_borders = get_border();
    paste_graph(&expr_tree, expr_tree.root, graph_borders);

    tree_val_t x0_for_taylor = 0;
    printf("Введите точку x0 для тейлора:");
    scanf("%lg", &x0_for_taylor);

    tree_val_t n_for_taylor = 0;
    printf("Введите степень n для тейлора:");
    scanf("%lg", &n_for_taylor);

    my_tree_t taylor_tree = get_taylor_series(&expr_tree, x0_for_taylor, n_for_taylor);
    TREE_DUMP(&taylor_tree, taylor_tree.root, "This is taylor tree");
    paste_taylor(&taylor_tree, taylor_tree.root);
    paste_two_graphs(&expr_tree, &taylor_tree, x0_for_taylor);

    print_ending();

    tree_dtor(&taylor_tree);
    tree_dtor(&expr_tree);
    tree_dtor(&diff_tree);
    free(buffer);

    disable_latex_output();
    disable_logging();

    return OK;
}
