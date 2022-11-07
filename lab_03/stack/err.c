#include "err.h"

int err_hadl(int rc)
{
    switch (rc)
    {
        case MEM_ERROR:
            printf("Memory error\n");
            break;
        case POP_EMPTY_STACK_ERROR:
            printf("Del from empty stack\n");
            break;
        case WRONG_DATA:
            printf("Wrong data\n");
            break;
        case ZERO_DIV:
            printf("Division by zero\n");
            break;
        case NO_ERROR:
            break;
        case ARR_OVERFLOW:
            printf("Overflow arr stack\n");
            break;
        default:
            printf("Unknown error\n");
            break;
    }
    return rc;
}
