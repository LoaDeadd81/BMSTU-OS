#include <stdio.h>
#include <string.h>
#include <time.h>

#include "err.h"
#include "stack.h"
#include "calc.h"
#include "sl_list.h"

#define BUFF_SIZE 1024
#define ITER 100

int main()
{
    setbuf(stdout, NULL);
    int rc = 0, c = 1;
    printf("The program calculates arithmetic expressions of the form:\n"
           "number | sign | number | sign | number. Input without spaces and unary minus.\n");
    stack_list stackList;
    stack_arr stackArr;
    memset(&stackList, 0, sizeof(stack_list));
    memset(&stackArr, 0, sizeof(stack_arr));
    sl_list_free *free_list = NULL;
    char str_in[BUFF_SIZE + 3];
    char str_post[BUFF_SIZE + 3];
    char s[VAL_LEN + 1];
    while (1)
    {
        printf("Choice menu item\nList\n1-Push to stack\n2-Pop from stack\n"
               "3-Show stack\n4-Print free mem\n5-Pop all elements\nArr\n6-Push to stack"
               "\n7-Pop from stack\n8-Show stack\n9-Pop all elements\nOther\n10-Calculate\n0-Exit\nChoice:");
        int sc = scanf("%d", &c);
        if (sc != 1)
        {
            getchar();
            printf("\nNot number\n\n");
            continue;
        }
        printf("\n");
        if (sc == 1)
            switch (c)
            {
                case 1:
                {

                    memset(&s, 0, sizeof(char) * (VAL_LEN + 1));
                    printf("Input data:");
                    rc = scanf("%s", s);
                    if (rc != 1 || strlen(s) > VAL_LEN)
                    {
                        rc = WRONG_DATA;
                        break;
                    }
                    rc = 0;
                    rc = list_push(&stackList, s);
                    break;
                }
                case 2:
                {
                    memset(&s, 0, sizeof(char) * (VAL_LEN + 1));
                    rc = list_pop(&stackList, s, &free_list);
                    if (rc == 0)
                        printf("Value: %s\n", s);
                    break;
                }
                case 3:
                    print_stack(&stackList);
                    break;
                case 4:
                    free_print_list(free_list);
                    break;
                case 5:
                {
                    int a = 0;
                    while (a != POP_EMPTY_STACK_ERROR)
                        a = list_pop(&stackList, s, &free_list);
                    break;
                }
                case 6:
                {
                    memset(&s, 0, sizeof(char) * (VAL_LEN + 1));
                    printf("Input data:");
                    rc = scanf("%s", s);
                    if (rc != 1 || strlen(s) > VAL_LEN)
                    {
                        rc = WRONG_DATA;
                        break;
                    }
                    rc = 0;
                    rc = arr_push(&stackArr, s);
                    if (rc)
                        break;
                    break;
                }
                case 7:
                {
                    memset(&s, 0, sizeof(char) * (VAL_LEN + 1));
                    rc = arr_pop(&stackArr, s);
                    if (rc == 0)
                        printf("Value: %s\n", s);
                    break;
                }
                case 8:
                {
                    print_stack_arr(&stackArr);
                    break;
                }
                case 9:
                {
                    int a = 0;
                    while (a != POP_EMPTY_STACK_ERROR)
                        a = arr_pop(&stackArr, s);
                    break;
                }
                case 10:
                {
                    memset(str_in, 0, sizeof(char) * (BUFF_SIZE + 3));
                    memset(str_post, 0, sizeof(char) * (BUFF_SIZE + 3));
                    printf("Input arithmetic expressions:");
                    scanf("%s", str_in);
                    char buf[10];
                    memset(buf, 0, sizeof(char) * (10));
                    fgets(buf, 10, stdin);
                    for (int i = 0; i < 10; i++)
                        if (!(isspace(buf[i]) || buf[i] == '\0'))
                            rc = WRONG_DATA;
                    if (rc)
                        break;
                    if (strlen(str_in) > BUFF_SIZE)
                        return err_hadl(WRONG_DATA);
                    rc = infix_to_postfix_list(str_in, str_post);
                    if (rc)
                    {
                        if (rc == POP_EMPTY_STACK_ERROR)
                            rc = WRONG_DATA;
                        break;
                    }
//                printf("Postfix: %s\n", str_post);
                    double res = 0;
                    rc = eval_list(str_post, &res);
                    if (rc)
                    {
                        if (rc == POP_EMPTY_STACK_ERROR)
                            rc = WRONG_DATA;
                        break;
                    }
                    printf("Result: %.4lf\n", res);
                    break;
                }
//            case 7:
//            {
//                printf("Input num of elements:");
//                int n_elem = 0;
//                int rc = scanf("%d", &n_elem);
//                if (rc != 1)
//                {
//                    rc = WRONG_DATA;
//                    break;
//                }
//                rc = 0;
//                double t1 = 0, t2 = 0;
//                size_t m1 = 0, m2 = 0;
//                stack_arr stackArr;
//                stack_list stackList;
//                memset(&stackArr, 0, sizeof(stackArr));
//                memset(&stackList, 0, sizeof(stackList));
//                char buf[VAL_LEN];
//                char tok[] = "jgcahdbnaskdua";
//                clock_t begin = clock();
//                for (int i = 0; i < ITER; i++)
//                {
//                    for (int j = 0; j < n_elem; j++)
//                        arr_push(&stackArr, tok);
//                    for (int j = 0; j < n_elem; j++)
//                        arr_pop(&stackArr, buf);
//                }
//                clock_t end = clock();
//                t1 = ((double) (end - begin));
//                t1 /= ITER;
//                t1 /= CLOCKS_PER_SEC;
//                m1 = sizeof(char) * (VAL_LEN + 1) * ARR_LEN + sizeof(int);
//
//
//                for (int i = 0; i < ITER; i++)
//                {
//                    clock_t begin1 = clock();
//                    for (int j = 0; j < n_elem; j++)
//                        list_push(&stackList, tok);
//                    for (int j = 0; j < n_elem; j++)
//                        list_pop(&stackList, buf, NULL);
//                    clock_t end1 = clock();
//                    t2 += ((double) (end1 - begin1));
//                }
//                t2 /= ITER;
//                t2 /= CLOCKS_PER_SEC;
//                m2 += sizeof(sl_list_token) * n_elem;
//
//                printf("%Iu %Iu\n", sizeof(sl_list_token), sizeof(char) * (VAL_LEN + 1));
//                printf("Arr: %lf %Iu\nList: %lf %Iu\n", t1, m1, t2, m2);
//            }
                case 0:
                    break;
                default:
                    printf("Wrong choice\n");
                    break;
            }
        err_hadl(rc);
        if (rc == MEM_ERROR || c == 0)
            break;
        else
            rc = 0;
        printf("\n");
    }
    list_stack_free(&stackList);
    free_list_free(free_list);
    return rc;
}
