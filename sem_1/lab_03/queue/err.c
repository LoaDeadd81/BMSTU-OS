#include "err.h"

int err_handl(int rc)
{
    switch (rc)
    {
        case MEM_ERROR:
            printf("Memory error\n");
            break;
        case OVERFLOW_ERROR:
            printf("Queue overflowed\n");
            break;
        case DATA_ERROR:
            printf("Wrong data\n");
            break;
        case EMPTY_QUEUE_POP_ERROR:
            printf("Delete element from empty queue\n");
            break;
        default:
            printf("Unknown error\n");
            break;
    }
    return rc;
}
