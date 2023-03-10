#include <stdio.h>
#include <string.h>
#include <time.h>

#include "queues.h"
#include "sl_list.h"
#include "queues_io.h"

#define BUFF_SIZE 4
#define ITER 100


int work_with_queue()
{
    int rc = NO_ERRORS;
    int c = 1;
    queue_arr queueArr;
    memset(&queueArr, 0, sizeof(queueArr));
    queue_list queueList;
    memset(&queueList, 0, sizeof(queueList));
    sl_list_free *listFree = NULL;
    while (c != 0)
    {

        printf("\n1-Add element to queue-arr\n2-Remove element from queue-arr\n3-Print queue-arr\n"
               "4-Add element to queue-list\n5-Remove element from queue-list\n6-Print queue-list\n"
               "7-Print free memory\n0-Exit\nChoice:");
        if ((scanf("%d", &c) != 1))
        {
            char buff[BUFF_SIZE];
            scanf("%s", buff);
            printf("\nWrong choice");
            continue;
        }
        if (c < 0 || c > 7)
        {
            printf("\nWrong choice\n");
            continue;
        }
        switch (c)
        {
            case 1:
            {
                int cur = 0;
                printf("Input number:");
                if ((scanf("%d", &cur) != 1))
                {
                    rc = DATA_ERROR;
                    break;
                }
                rc = queue_arr_push(&queueArr, cur);
                break;
            }
            case 2:
            {
                int cur = 0;
                rc = queue_arr_pop(&queueArr, &cur);
                if (!rc)
                    printf("Val: %d\n", cur);
                break;
            }
            case 3:
            {
                print_arr(&queueArr);
                break;
            }
            case 4:
            {
                int cur = 0;
                printf("Input number:");
                if ((scanf("%d", &cur) != 1))
                {
                    rc = DATA_ERROR;
                    break;
                }
                rc = queue_list_push(&queueList, cur);
                break;
            }
            case 5:
            {
                int cur = 0;
                rc = queue_list_pop(&queueList, &cur, &listFree);
                if (!rc)
                    printf("Val: %d\n", cur);
                break;
            }
            case 6:
            {
                print_list(&queueList);
                break;
            }
            case 7:
                print_list_free(listFree);
                break;
            default:
                break;
        }
        if (rc)
        {
            err_handl(rc);
            rc = 0;
        }
    }
    queue_list_free(&queueList);
    free_list_free(listFree);
    return rc;
}

int sim()
{
    int c = 1, rc = 0;
    while (c != 0)
    {
        printf("1-Simulating with arr-queue\n2-Simulating with list-queue\n0-Exit\nChoice:");
        if ((scanf("%d", &c) != 1))
        {
            char buff[BUFF_SIZE];
            scanf("%s", buff);
            printf("\nWrong choice");
            continue;
        }
        if (c < 0 || c > 2)
        {
            printf("\nWrong choice\n");
            continue;
        }
        switch (c)
        {
            case 1:
            {
                rc = sim_arr_strap();
                break;
            }
            case 2:
            {
                rc = sim_list_strap();
                break;
            }
            default:
                break;
        }
        if (rc)
        {
            err_handl(rc);
            rc = 0;
        }
    }
    return rc;
}


int menu()
{
    int rc = NO_ERRORS;
    printf("The program simulates a queuing system with 2 alternating\n"
           "priority queues and one server. Also provides the ability\n"
           "to work with queues of different types\n");
    int c = 1;
    while (c != 0)
    {
        while (c != 0)
        {
            printf("1-Work with queue\n2-Simulation\n3-Measure\n0-Exit\nChoice:");
            if ((scanf("%d", &c) != 1))
            {
                char buff[BUFF_SIZE];
                scanf("%s", buff);
                printf("\nWrong choice");
                continue;
            }
            if (c < 0 || c > 3)
            {
                printf("\nWrong choice\n");
                continue;
            }
            switch (c)
            {
                case 1:
                {
                    rc = work_with_queue();
                    break;
                }
                case 2:
                {
                    rc = sim();
                    break;
                }
                case 3:
                {
                    int n = 0;
                    while (1)
                    {
                        printf("Input num of elements:");
                        if((scanf("%d", &n) == 1) && n > 0 && n <= ARR_SIZE)
                            break;
                        printf("Wrong data\n");
                    }
                    rc = measure(n, ITER);
                    break;
                }
                default:
                    break;
            }
        }
    }
    return rc;
}

int main()
{
    setbuf(stdout, NULL);
    int rc = menu();
    return rc;
}