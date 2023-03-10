#ifndef ERR_H
#define ERR_H

#include <stdio.h>

#define NO_ERRORS 0

#define MEM_ERROR 1
#define OVERFLOW_ERROR 2
#define DATA_ERROR 3
#define EMPTY_QUEUE_POP_ERROR 4

int err_handl(int rc);

#endif 
