#ifndef RECURSIVE_DESCNET_PARSER_H_
#define RECURSIVE_DESCNET_PARSER_H_

#include "stdlib.h"

int get_grammatic(char* input);
int get_number(char* input, size_t* pos);
int get_expression(char* input, size_t* pos);
int get_mul_div(char* input, size_t* pos);
int get_exp(char* input, size_t* pos);
int get_primary(char* input, size_t* pos);

#endif // RECURSIVE_DESCNET_PARSER_H_
