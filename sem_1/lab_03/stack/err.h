#ifndef ERR_H
#define ERR_H

#include <stdio.h>

#define NO_ERROR 0
#define MEM_ERROR 1
#define POP_EMPTY_STACK_ERROR 2
#define WRONG_DATA 3
#define ZERO_DIV 4
#define ARR_OVERFLOW 5

int err_hadl(int rc);

#endif 
