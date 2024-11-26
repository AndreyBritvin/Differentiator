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

#define  INFIX_OUT(text, L, R)  fprintf(output, "(");                       \
                                latex_node(tree, L, output, is_graph_mode); \
                                fprintf(output, text);                      \
                                latex_node(tree, R, output, is_graph_mode); \
                                fprintf(output, ")");
                                // fprintf(output, ")");
                                // fprintf(output, "(");

#define  POWER_OUT(text, L, R)  fprintf(output, "(");                       \
                                latex_node(tree, L, output, is_graph_mode); \
                                fprintf(output, ")");                       \
                                fprintf(output, text);                      \
                                if (is_graph_mode == GRAPH_MODE)            \
                                fprintf(output, "(");                       \
                                else fprintf(output, "{");                  \
                                latex_node(tree, R, output, is_graph_mode); \
                                if (is_graph_mode == GRAPH_MODE)            \
                                fprintf(output, ")");                       \
                                else fprintf(output, "}");

#define PREFIX_OUT(text, L, R)  if (is_graph_mode == GRAPH_MODE)                \
                                {                                               \
                                    fprintf(output, "(");                       \
                                    latex_node(tree, L, output, is_graph_mode); \
                                    fprintf(output, ")");                       \
                                    fprintf(output, "/");                       \
                                    fprintf(output, "(");                       \
                                    latex_node(tree, R, output, is_graph_mode); \
                                    fprintf(output, ")");                       \
                                }                                               \
                                else                                            \
                                {                                               \
                                    fprintf(output, text);                      \
                                    fprintf(output, "{");                       \
                                    latex_node(tree, L, output, is_graph_mode); \
                                    fprintf(output, "}{");                      \
                                    latex_node(tree, R, output, is_graph_mode); \
                                    fprintf(output, "}");                       \
                                }

#define UNAR_OUT(text, L)       if (is_graph_mode != GRAPH_MODE) fprintf(output, "\\");         \
                                fprintf(output, text);                                          \
                                fprintf(output, "(");                                           \
                                latex_node(tree, L, output, is_graph_mode);                     \
                                fprintf(output, ")");

#define TYPE_NAME all_ops[(int) node->data].text
#define LEFT      node->left
#define RIGHT     node->right

err_code_t latex_node(my_tree_t* tree, node_t* node, FILE* output, bool is_graph_mode)
{
    assert(tree);
    assert(output);
    assert(node);

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
                case MUL:  INFIX_OUT(TYPE_NAME, LEFT, RIGHT); break;
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
        default:
            fprintf(stderr, "unknown operation in latex_node"); break;
    }

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
        printf("line %zu = %s\n", i, line);
        all_jokes[i] = line;
    }
    for (size_t i = 0; i < lines_num; i++)
    {
        printf("%s\n", all_jokes[i]);
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

    return OK;
}

err_code_t print_introduction()
{
    LATEX("\\documentclass{article}\n"
    "\\usepackage[T2A]{fontenc}\n"
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
    latex_node(tree, node_before, LATEX_FILE, FORMULA_MODE);
    LATEX(")' = ");
    latex_node(tree, node_after, LATEX_FILE, FORMULA_MODE);
    LATEX("\\]\n");

    return OK;
}

err_code_t print_equation_begining(my_tree_t* tree, node_t* node_before, const char* text)
{
    LATEX("%s\n", text);
    LATEX("\\[(");
    latex_node(tree, node_before, LATEX_FILE, FORMULA_MODE);
    LATEX(")'\n");

    return OK;
}

err_code_t print_equation_ending(my_tree_t* tree, node_t* node_before, latex_output_mode is_diff)
{
    LATEX("(");
    latex_node(tree, node_before, LATEX_FILE, FORMULA_MODE);
    LATEX(")");
    if (is_diff = DIFF)
    LATEX("'");
    LATEX("\n");
    node_dtor(node_before);

    return OK;
}



err_code_t paste_graph(my_tree_t* tree, node_t* node)
{
    LATEX("\\section{Кривульки}\n");

    LATEX(
    "\\begin{tikzpicture}\n"
    "\\begin{axis}["
    "xmin=-11,xmax=11,\n"
    "ymin=-11,ymax=11,\n"
    "grid=both,\n"
    "grid style={line width=.1pt, draw=gray!10},\n"
    "major grid style={line width=.2pt,draw=gray!50},\n"
    "axis lines=middle,\n"
    "minor tick num=4,\n"
    "axis line style={latex-latex},\n"
    "ticklabel style={font=\\tiny,fill=white},\n"
    "xlabel style={at={(ticklabel* cs:1)},anchor=north west},\n"
    "ylabel style={at={(ticklabel* cs:1)},anchor=south west}\n"
    "]\n"
    "\\addplot[color=red, samples=1000]{"
        );
    latex_node(tree, node, LATEX_FILE, GRAPH_MODE);
    LATEX(
    "};\n"
    "\\end{axis}\n"
    "\\end{tikzpicture}\n"
        );

    return OK;
}

err_code_t paste_taylor(my_tree_t* tree, node_t* node)
{
    LATEX("\\section{Кто эта ваша Taylor фиВт}\n")

    LATEX("Вот тейлорово разложение. После контрольной в самый раз\n"
          "\\[");
    latex_node(tree, node, LATEX_FILE, FORMULA_MODE);
    LATEX("\\]\n");

    return OK;
}
