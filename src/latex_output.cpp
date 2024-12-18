#include "differenciator.h"
#include "latex_output.h"
#include "utils.h"
#include <assert.h>
#include <string.h>
#include <time.h>

FILE* LATEX_FILE = NULL;
static char** all_jokes = NULL;
static size_t lines_num = 0;

#define LATEX(...) if (LATEX_FILE != NULL) fprintf(LATEX_FILE, __VA_ARGS__);

//TODO: FIX brackets
#define  INFIX_OUT(text, L, R)  fprintf(output, "(");                       \
                                latex_node(tree, L, output, is_graph_mode, recurs_level + 1); \
                                fprintf(output, text);                      \
                                latex_node(tree, R, output, is_graph_mode, recurs_level + 1); \
                                fprintf(output, ")");

#define  INFIX_OUT2(text, L, R)  fprintf(output, "");                       \
                                latex_node(tree, L, output, is_graph_mode, recurs_level + 1); \
                                fprintf(output, text);                      \
                                latex_node(tree, R, output, is_graph_mode, recurs_level + 1); \
                                fprintf(output, "");
                                // fprintf(output, ")");
                                // fprintf(output, "(");

#define  POWER_OUT(text, L, R)  fprintf(output, "(");                       \
                                latex_node(tree, L, output, is_graph_mode, recurs_level + 1); \
                                fprintf(output, ")");                       \
                                if (is_graph_mode == GRAPH_MODE)            \
                                fprintf(output, "**");                      \
                                else                                        \
                                fprintf(output, text);                      \
                                if (is_graph_mode == GRAPH_MODE)            \
                                fprintf(output, "(");                       \
                                else fprintf(output, "{");                  \
                                latex_node(tree, R, output, is_graph_mode, recurs_level + 1); \
                                if (is_graph_mode == GRAPH_MODE)            \
                                fprintf(output, ")");                       \
                                else fprintf(output, "}");

#define PREFIX_OUT(text, L, R)  if (is_graph_mode == GRAPH_MODE)                \
                                {                                               \
                                    fprintf(output, "(");                       \
                                    latex_node(tree, L, output, is_graph_mode, recurs_level + 1); \
                                    fprintf(output, ")");                       \
                                    fprintf(output, "/");                       \
                                    fprintf(output, "(");                       \
                                    latex_node(tree, R, output, is_graph_mode, recurs_level + 1); \
                                    fprintf(output, ")");                       \
                                }                                               \
                                else                                            \
                                {                                               \
                                    fprintf(output, text);                      \
                                    fprintf(output, "{");                       \
                                    latex_node(tree, L, output, is_graph_mode, recurs_level + 1); \
                                    fprintf(output, "}{");                      \
                                    latex_node(tree, R, output, is_graph_mode, recurs_level + 1); \
                                    fprintf(output, "}");                       \
                                }

#define UNAR_OUT(text, L)       if (is_graph_mode != GRAPH_MODE) fprintf(output, "\\");         \
                                fprintf(output, text);                                          \
                                fprintf(output, "(");                                           \
                                latex_node(tree, L, output, is_graph_mode, recurs_level + 1);   \
                                fprintf(output, ")");

#define TYPE_NAME all_ops[(int) node->data].text
#define LEFT      node->left
#define RIGHT     node->right

node_t** latex_node(my_tree_t* tree, node_t* node, FILE* output,
                    latex_output_mode is_graph_mode, size_t recurs_level)
{
    assert(tree);
    assert(output);
    assert(node);
    static node_t** subtrees_indexes = NULL;
    static size_t subtrees_count = 0;

    if (is_graph_mode != GRAPH_MODE && recurs_level == RECURSION_BEGIN)
    {
        subtrees_count = 0;
        subtrees_indexes = (node_t**) calloc(MAX_SUBTREES_COUNT, sizeof(node_t*));
        // TREE_DUMP(tree, node, "This is latex_node. Before creation nodes");
        generate_subtrees(tree, node, RECURSION_BEGIN);
        // printf("node addr = %p\n", node);
        // TREE_DUMP(tree, node, "This is latex_node. After creation nodes");
    }

    switch (node->type)
    {
        case NUM: fprintf(output, "%lg", node->data); break;
        case VAR: fprintf(output, "%c",  (char) node->data); break;
        case OP:
        {
            switch ((int) node->data)
            {
                case ADD:  INFIX_OUT(TYPE_NAME, LEFT, RIGHT); break;
                case SUB:  INFIX_OUT(TYPE_NAME, LEFT, RIGHT); break;
                case MUL:  INFIX_OUT2(TYPE_NAME, LEFT, RIGHT); break;
                case DIV: PREFIX_OUT("\\frac", LEFT, RIGHT); break;

                case SIN:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case COS:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case TAN:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case CTG:   UNAR_OUT(TYPE_NAME, LEFT); break;

                case SHN:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case CHS:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case TGH:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case CTH:   UNAR_OUT(TYPE_NAME, LEFT); break;

                case EXP:  POWER_OUT(TYPE_NAME, LEFT, RIGHT); break;
                case LOG:   UNAR_OUT(TYPE_NAME, LEFT); break;

                case ARCSIN:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case ARCCOS:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case ARCCTG:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case ARCTAN:   UNAR_OUT(TYPE_NAME, LEFT); break;

                case ARCSHN:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case ARCCHS:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case ARCTGH:   UNAR_OUT(TYPE_NAME, LEFT); break;
                case ARCCTH:   UNAR_OUT(TYPE_NAME, LEFT); break;

                default: fprintf(stderr, "unknow operator in latex node");break;
            }
            break;
        }
        case SUBTREE:
        {
            fprintf(output, "%s", (char*) &node->data);
            // printf("We are in subtree with name = %s\n", (char*) &node->data);
            subtrees_indexes[subtrees_count++] = node;
            if (subtrees_count >= MAX_SUBTREES_COUNT - 1)
            {
                assert("You should change MAX_SUBTREES_COUNT in latex_output.h" == NULL);
            }
            // latex_node(tree, node->left, output, is_graph_mode, recurs_level + 1);
            break;
        }
        default:
            fprintf(stderr, "unknown operation in latex_node"); break;
    }

    if (is_graph_mode != GRAPH_MODE && recurs_level == RECURSION_BEGIN)
    {
        print_subtrees(tree, output, subtrees_indexes, is_graph_mode);
        remove_subtrees(tree, node);
    }

    return subtrees_indexes;
}

err_code_t print_subtrees(my_tree_t* tree, FILE* output, node_t** subtrees, latex_output_mode is_diff)
{
    if (is_diff == DIFF_MODE) fprintf(output, ")'");
    fprintf(output, "\\]");
    for (size_t i = 0; i < MAX_SUBTREES_COUNT; i++)
    {
        if (subtrees[i] == NULL) continue;
        fprintf(output, "Где");
        break;
    }

    for (size_t i = 0; i < MAX_SUBTREES_COUNT; i++)
    {
        if (subtrees[i] == NULL) continue;
        fprintf(output, "\\[%s = ", (char*) &(subtrees[i]->data));
        latex_node(tree, subtrees[i]->left, output, FORMULA_MODE, RECURSION_BEGIN + 1);
        fprintf(output, "\\]\n");
    }
    free(subtrees);

    return OK;
}

size_t get_lines_num(char* buffer, size_t all_len)
{
    size_t lines_count = 0;
    for (size_t i = 0; i < all_len; i++)
    {
        if (buffer[i] == '\n')
        {
            lines_count += 1;
        }
    }

    return lines_count;
}

err_code_t fill_jokes(const char* filename)
{
    size_t all_len = get_file_len(filename);
    FILE* SAFE_OPEN_FILE(jokes_file, filename, "r");
    SAFE_CALLOC(buffer, char, all_len);

    fread(buffer, all_len, sizeof(char), jokes_file);
    fseek(jokes_file, 0, SEEK_SET);

    lines_num = get_lines_num(buffer, all_len);
    all_jokes = (char**) calloc(lines_num, sizeof(char*));

    char* offset = buffer;
    for (size_t i = 0; i < lines_num; i++)
    {
        SAFE_CALLOC(line, char, MAX_STRING_SIZE);
        fgets(line, MAX_STRING_SIZE, jokes_file);
        // printf("line %zu = %s\n", i, line);
        all_jokes[i] = line;
    }
    for (size_t i = 0; i < lines_num; i++)
    {
        // printf("%s\n", all_jokes[i]);
    }

    free(buffer);

    return OK;
}

err_code_t free_all_jokes()
{
    for (size_t i = 0; i < lines_num; i++)
    {
        free(all_jokes[i]);
    }
    free(all_jokes);

    return OK;
}

err_code_t enable_latex_output(const char* filename, const char* filejokes)
{
    assert(filename);
    srand(time(NULL));
    fill_jokes(filejokes);
    SAFE_OPEN_FILE(LATEX_FILE, filename, "w");

    return OK;
}

err_code_t disable_latex_output()
{
    assert(LATEX_FILE);
    free_all_jokes();

    fclose(LATEX_FILE);

    system("pdflatex tree_dump/latex.tex");

    return OK;
}

err_code_t print_introduction()
{
    LATEX("\\documentclass{article}\n"
    "\\usepackage[T2A]{fontenc}\n"
    "\\usepackage{graphicx}\n"
    "\\graphicspath{ {./tree_dump/graphs/} }\n"
    "\\usepackage[margin=0.25in]{geometry}\n"
    "\\usepackage{pgfplots}\n"
    "\\pgfplotsset{width=10cm,compat=1.9}\n\n"
    "\\title{Взятие производной; Графики; Тейлор SWIFT}\n"
    "\\author{Andrey Britvin}\n"
    "\\date{November 2024}\n"
    "\\begin{document}\n"
    "\\maketitle\n"
    "\\section{Назад к истокам}\n"
        )

    return OK;
}

err_code_t print_ending()
{
    LATEX("\\section{The end}\n");

    LATEX("\\end{document}\n");

    return OK;
}

err_code_t print_equation(my_tree_t* tree, node_t* node_before, node_t* node_after)
{
    LATEX("%s"
          "\\[(", all_jokes[rand() % lines_num]);
    // TREE_DUMP(tree, tree->root, "This is latex_node. Before creation nodes");
    latex_node(tree, node_before, LATEX_FILE, DIFF_MODE, RECURSION_BEGIN);
    LATEX("\n Есть не что иное, как\n \\[");
    latex_node(tree, node_after, LATEX_FILE, FORMULA_MODE, RECURSION_BEGIN);
    LATEX("\n");

    return OK;
}

err_code_t print_equation_begining(my_tree_t* tree, node_t* node_before, const char* text)
{
    LATEX("%s\n", text);
    LATEX("\\[");
    latex_node(tree, node_before, LATEX_FILE, FORMULA_MODE, RECURSION_BEGIN);
    LATEX("\n");

    return OK;
}

err_code_t print_equation_ending(my_tree_t* tree, node_t* node_before, latex_output_mode is_diff)
{
    LATEX("(");
    latex_node(tree, node_before, LATEX_FILE, FORMULA_MODE, RECURSION_BEGIN);
    LATEX(")");
    if (is_diff == DIFF)
    LATEX("'");
    LATEX("\n");
    node_dtor(node_before);

    return OK;
}

borders get_border()
{
    double left = 0, right = 0;
    printf("Введите левую границу:");
    while (scanf("%lg", &left) == 0)
    {
        printf("Пожалуйста, введите число\n");
    }

    printf("Введите правую границу:");
    while (scanf("%lg", &right) == 0)
    {
        printf("Пожалуйста, введите число\n");
    }
    borders to_ret = {};
    to_ret.left  = left;
    to_ret.right = right;

    return to_ret;
}

err_code_t paste_graph(my_tree_t* tree, node_t* node, borders border)
{
    LATEX("\\section{Кривульки}\n");
    static size_t image_counter = 0;
    FILE* gnuplot_pipe = popen("gnuplot -p", "w");

    if (gnuplot_pipe == NULL) return ERROR_FILE;

    fprintf(gnuplot_pipe, "set term pngcairo\n");
    fprintf(gnuplot_pipe, "set output \"tree_dump/graphs/%zu.png\"\n set grid\n", image_counter);
    fprintf(gnuplot_pipe, "plot [%lg:%lg] ", border.left, border.right);
    latex_node(tree, node, gnuplot_pipe, GRAPH_MODE, RECURSION_BEGIN);
    fprintf(gnuplot_pipe, "\n");

    LATEX("\\includegraphics[width=\\textwidth, height=0.9\\textheight,"
                 "keepaspectratio]{tree_dump/graphs/%zu.png}\n", image_counter);

    pclose(gnuplot_pipe);

    image_counter += 2;

    return OK;
}

err_code_t paste_taylor(my_tree_t* tree, node_t* node)
{
    LATEX("\\section{Кто эта ваша Taylor фиВт}\n")

    LATEX("Вот тейлорово разложение. После контрольной в самый раз\n"
          "\\[");
    latex_node(tree, node, LATEX_FILE, FORMULA_MODE, RECURSION_BEGIN);
    LATEX("\n");

    return OK;
}

borders get_border(tree_val_t x0)
{
    double delta = 0;
    printf("Введите дельту для дельта окрестности:");
    while (scanf("%lg", &delta) == 0)
    {
        printf("Пожалуйста, введите число\n");
    }
    borders to_ret = {};
    to_ret.left  = x0 - delta;
    to_ret.right = x0 + delta;

    return to_ret;
}

err_code_t paste_two_graphs(my_tree_t* tree_1, my_tree_t* tree_2, tree_val_t x0)
{
    static size_t image_counter = 1;
    borders delta_x0 = get_border(x0);
    LATEX("\\section{Кривляние тейлора в $\\delta$ - окрестности точки x0 = %lg}\n", x0);

    FILE* gnuplot_pipe = popen("gnuplot -p", "w");

    fprintf(gnuplot_pipe, "set term pngcairo\n");
    fprintf(gnuplot_pipe, "set output \"tree_dump/graphs/%zu.png\"\n set grid\n", image_counter);
    fprintf(gnuplot_pipe, "plot [%lg:%lg] ", delta_x0.left, delta_x0.right);
    latex_node(tree_1, tree_1->root, gnuplot_pipe, GRAPH_MODE, RECURSION_BEGIN);
    fprintf(gnuplot_pipe, " with lines title \"expr\", ", delta_x0.left, delta_x0.right);
    latex_node(tree_2, tree_2->root, gnuplot_pipe, GRAPH_MODE, RECURSION_BEGIN);
    fprintf(gnuplot_pipe, " with lines title \"taylor\"\n");

    pclose(gnuplot_pipe);

    LATEX("\\includegraphics[width=\\textwidth, height=0.9\\textheight,"
                 "keepaspectratio]{tree_dump/graphs/%zu.png}\n", image_counter);

    image_counter += 2;

    return OK;
}

err_code_t print_tree_value(my_tree_t* tree, tree_val_t x0)
{
    tree_val_t result = evaluate_tree(tree->root);
    LATEX("Подставляя x = %lg получим что это выражение коллапсирует в %lg\n\n", x0, result);

    return OK;
}
