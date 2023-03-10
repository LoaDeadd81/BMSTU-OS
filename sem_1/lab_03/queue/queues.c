#include "queues.h"

void queue_list_free(queue_list *queue)
{
    free_list(queue->begin);
}

int queue_arr_push(queue_arr *queue, int data)
{
    queue->pin = queue->pin % ARR_SIZE;
    if (queue->pin == queue->pout && queue->is_not_empty)
        return OVERFLOW_ERROR;
    queue->arr[queue->pin] = data;
    queue->is_not_empty = 1;
    queue->pin++;
    return NO_ERRORS;
}

int queue_arr_pop(queue_arr *queue, int *res)
{
    queue->pout = queue->pout % ARR_SIZE;
    if (queue->pin % ARR_SIZE == queue->pout && queue->is_not_empty == 0)
        return EMPTY_QUEUE_POP_ERROR;
    if (queue->pout + 1 == queue->pin)
        queue->is_not_empty = 0;
    *res = queue->arr[queue->pout];
    queue->arr[queue->pout] = 0;
    queue->pout++;
    return NO_ERRORS;
}

int queue_list_push(queue_list *queue, int data)
{
    sl_list *cur = node_init(data);
    if (!cur)
        return MEM_ERROR;
    if (queue->begin == NULL)
        queue->begin = cur;
    queue->end = push_back(queue->end, cur);
    return NO_ERRORS;
}

int queue_list_pop(queue_list *queue, int *res, sl_list_free **listFree)
{
    if (queue->begin == NULL)
        return EMPTY_QUEUE_POP_ERROR;
    queue->begin = pop_front(queue->begin, res, listFree);
    return NO_ERRORS;
}

double rnd_double(double *bor)
{
    return (bor[1] - bor[0]) * ((double) rand() / (double) RAND_MAX) + bor[0];
}

double mn(double a, double b, double c)
{
    if (b < c)
    {
        if (a < b)
            return a;
        return b;
    }
    return c;
}

double err(double fact, double exp)
{
    return fabs(fact - exp) / exp;
}

int param_input(int *num, double *bor1, double *bor2, double *bor3, double *bor4)
{
    int rc = NO_ERRORS;
    printf("Input num of elements:");
    if ((scanf("%d", num) != 1) || *num < 1)
        return DATA_ERROR;
    printf("Enter the arrival interval for the first queue:");
    if ((scanf("%lf", &bor1[0]) != 1) || bor1[0] < 0 || (scanf("%lf", &bor1[1]) != 1) || bor1[1] < 0 ||
        bor1[0] > bor1[1] || fabs(bor1[0] - bor1[1]) < EPS)
        return DATA_ERROR;
    printf("Enter the arrival interval for the second queue:");
    if ((scanf("%lf", &bor2[0]) != 1) || bor2[0] < 0 || (scanf("%lf", &bor2[1]) != 1) || bor2[1] < 0 ||
        bor2[0] > bor2[1] || fabs(bor2[0] - bor2[1]) < EPS)
        return DATA_ERROR;
    printf("Enter the processing interval for elements of the first type:");
    if ((scanf("%lf", &bor3[0]) != 1) || bor3[0] < 0 || (scanf("%lf", &bor3[1]) != 1) || bor3[1] < 0 ||
        bor3[0] > bor3[1] || fabs(bor3[0] - bor3[1]) < EPS)
        return DATA_ERROR;
    printf("Enter the processing interval for elements of the second type:");
    if ((scanf("%lf", &bor4[0]) != 1) || bor4[0] < 0 || (scanf("%lf", &bor4[1]) != 1) || bor4[1] < 0 ||
        bor4[0] > bor4[1] || fabs(bor4[0] - bor4[1]) < EPS)
        return DATA_ERROR;
    return rc;
}

int simulation_arr(int num, double *bor1, double *bor2, double *bor3, double *bor4)
{
    int rc = NO_ERRORS, n1_q = -1, n2_q = -1, n1_in = 0, n2_in = 0, n1_out = 0, n2_out = 0, oa_type = 0, q1_sr = 0, q2_sr = 0;
    double t1_in = 0, t2_in = 0, t_oa = 0, t_do = 0, t_all = 0, t_chill = 0;
    queue_arr q1, q2;
    memset(&q1, 0, sizeof(q1));
    memset(&q2, 0, sizeof(q2));
    double tt1 = 0, tt2 = 0, tt3 = 0, tt4 = 0;
    while (n1_out < num)
    {
        t_all += t_do;
        t1_in -= t_do;
        t2_in -= t_do;
        t_oa -= t_do;
        if (n1_q >= 0)
            q1_sr += n1_q;
        if (n2_q >= 0)
            q2_sr += n2_q;
        if (t1_in < EPS)
        {
            rc = queue_arr_push(&q1, n1_q);
            if (rc)
                break;
            n1_q++;
            n1_in++;
            t1_in = rnd_double(bor1);
            tt1 += t1_in;
        }
        if (t2_in < EPS)
        {
            rc = queue_arr_push(&q2, n2_q);
            if (rc)
                break;
            n2_q++;
            n2_in++;
            t2_in = rnd_double(bor2);
            tt2 += t2_in;
        }
        if (t_oa < EPS)
        {
            int tmp = 0;
            if (oa_type == 0)
            {
                if (n1_q > 0)
                {
                    rc = queue_arr_pop(&q1, &tmp);
                    if (rc)
                        break;
                    n1_q--;
                    t_oa = rnd_double(bor3);
                    oa_type = 1;
                }
                else if (n2_q > 0)
                {
                    rc = queue_arr_pop(&q2, &tmp);
                    if (rc)
                        break;
                    n2_q--;
                    t_oa = rnd_double(bor4);
                    oa_type = 2;
                }
                else
                {
                    t_do = mn(t1_in, t2_in, t1_in + t2_in);
                    t_all += t_do;
                    t_chill += t_do;
                    continue;
                }
            }
            else
            {

                if (n1_q < 1 && n2_q < 1)
                {
                    t_do = mn(t1_in, t2_in, t1_in + t2_in);
                    t_all += t_do;
                    t_chill += t_do;
                    continue;
                }
                if (oa_type == 1)
                {
                    n1_out++;
                    if (n1_q > 0)
                    {
                        rc = queue_arr_pop(&q1, &tmp);
                        if (rc)
                            break;
                        n1_q--;
                        t_oa = rnd_double(bor3);
                        oa_type = 1;
                        tt3 += t_oa;
                    }
                    else
                    {
                        rc = queue_arr_pop(&q2, &tmp);
                        if (rc)
                            break;
                        n2_q--;
                        t_oa = rnd_double(bor4);
                        oa_type = 2;
                        tt4 += t_oa;
                    }
                }
                else
                {
                    n2_out++;
                    if (n2_q > 0)
                    {
                        rc = queue_arr_pop(&q2, &tmp);
                        if (rc)
                            break;
                        n2_q--;
                        t_oa = rnd_double(bor4);
                        oa_type = 2;
                        tt4 += t_oa;
                    }
                    else
                    {
                        rc = queue_arr_pop(&q1, &tmp);
                        if (rc)
                            break;
                        n1_q--;
                        t_oa = rnd_double(bor3);
                        oa_type = 1;
                        tt3 += t_oa;
                    }
                }
            }
        }
        t_do = mn(t1_in, t2_in, t_oa);
        if ((n2_out + n1_out) % 100 == 0 && (n1_out != 0 || n2_out != 0))
        {
            printf("Elements processed: %d\n", n2_out + n1_out);
            printf("Length of the first queue: %d\n", n1_q);
            printf("Average length of the first queue: %.4lf\n", (double) q1_sr / 100.0);
            printf("Length of the second queue: %d\n", n2_q);
            printf("Average length of the second queue: %.4lf\n\n", (double) q2_sr / 100.0);
            q1_sr = 0;
            q2_sr = 0;
        }
    }

    if (rc == 0)
    {
        double exp1_do, exp1_come;
        double exp2_do, exp2_come;

        exp1_come = (bor1[0] + bor1[1]) / 2 * n1_out;
        exp1_do = (bor3[0] + bor3[1]) / 2 * n1_out;

        exp2_come = (bor2[0] + bor2[1]) / 2 * n2_out;
        exp2_do = (bor4[0] + bor4[1]) / 2 * n2_out;


        printf("Total running time: %.4lf\n", t_all);
        printf("Total downtime: %.4lf\n\n", t_chill);

        printf("Included items of the first type: %d\n", n1_in);
        printf("Processed elements of the first type: %d\n\n", n1_out);

        printf("Included items of the second type: %d\n", n2_in);
        printf("Processed elements of the second type: %d\n\n", n2_out);

        printf("Arrival time in first queue\nFact: %.4lf\nExpected: %.4lf\nError : %.2lf\n", tt1, exp1_come,
               err(tt1, exp1_come));
        printf("Processing time for items from the first queue\nFact: %.4lf\nExpected: %.4lf\nError : %.2lf\n", tt3,
               exp1_do,
               err(tt3, exp1_do));

        printf("Arrival time in second queue\nFact: %.4lf\nExpected: %.4lf\nError : %.2lf\n", tt2, exp2_come,
               err(tt2, exp2_come));
        printf("Processing time for items from the second queue\nFact: %.4lf\nExpected: %.4lf\nError : %.2lf\n", tt4,
               exp2_do,
               err(tt4, exp2_do));;
    }
    return rc;
}


int sim_arr_strap()
{
    int num = 1000, rc = 1;
    double bor1[] = {1, 5}, bor2[] = {0, 3}, bor3[] = {0, 4}, bor4[] = {0, 1};
    int c = 0;
    while (1)
    {
        printf("1-Change simulation parameters\n2-Simulate\nChoice:");
        if ((scanf("%d", &c) == 1) && c > 0 && c < 3)
            break;
        printf("Wrong choice\n");
    }
    if (c == 1)
        while (rc != 0)
        {
            rc = param_input(&num, bor1, bor2, bor3, bor4);
            printf("Wrong data\n");
        }
    rc = simulation_arr(num, bor1, bor2, bor3, bor4);
    return rc;
}

int simulation_list(int num, double *bor1, double *bor2, double *bor3, double *bor4, sl_list_free **listFree)
{
    int rc = NO_ERRORS, n1_q = -1, n2_q = -1, n1_in = 0, n2_in = 0, n1_out = 0, n2_out = 0, oa_type = 0, q1_sr = 0, q2_sr = 0;
    double t1_in = 0, t2_in = 0, t_oa = 0, t_do = 0, t_all = 0, t_chill = 0;
    queue_list q1, q2;
    memset(&q1, 0, sizeof(q1));
    memset(&q2, 0, sizeof(q2));
    double tt1 = 0, tt2 = 0, tt3 = 0, tt4 = 0;
    while (n1_out < num)
    {
        t_all += t_do;
        t1_in -= t_do;
        t2_in -= t_do;
        t_oa -= t_do;
        if (n1_q >= 0)
            q1_sr += n1_q;
        if (n2_q >= 0)
            q2_sr += n2_q;
        if (t1_in < EPS)
        {
            rc = queue_list_push(&q1, n1_q);
            if (rc)
                break;
            n1_q++;
            n1_in++;
            t1_in = rnd_double(bor1);
            tt1 += t1_in;
        }
        if (t2_in < EPS)
        {
            rc = queue_list_push(&q2, n2_q);
            if (rc)
                break;
            n2_q++;
            n2_in++;
            t2_in = rnd_double(bor2);
            tt2 += t2_in;
        }
        if (t_oa < EPS)
        {
            int tmp = 0;
            if (oa_type == 0)
            {
                if (n1_q > 0)
                {
                    rc = queue_list_pop(&q1, &tmp, listFree);
                    if (rc)
                        break;
                    n1_q--;
                    t_oa = rnd_double(bor3);
                    oa_type = 1;
                }
                else if (n2_q > 0)
                {
                    rc = queue_list_pop(&q2, &tmp, listFree);
                    if (rc)
                        break;
                    n2_q--;
                    t_oa = rnd_double(bor4);
                    oa_type = 2;
                }
                else
                {
                    t_do = mn(t1_in, t2_in, t1_in + t2_in);
                    t_all += t_do;
                    t_chill += t_do;
                    continue;
                }
            }
            else
            {

                if (n1_q < 1 && n2_q < 1)
                {
                    t_do = mn(t1_in, t2_in, t1_in + t2_in);
                    t_all += t_do;
                    t_chill += t_do;
                    continue;
                }
                if (oa_type == 1)
                {
                    n1_out++;
                    if (n1_q > 0)
                    {
                        rc = queue_list_pop(&q1, &tmp, listFree);
                        if (rc)
                            break;
                        n1_q--;
                        t_oa = rnd_double(bor3);
                        oa_type = 1;
                        tt3 += t_oa;
                    }
                    else
                    {
                        rc = queue_list_pop(&q2, &tmp, listFree);
                        if (rc)
                            break;
                        n2_q--;
                        t_oa = rnd_double(bor4);
                        oa_type = 2;
                        tt4 += t_oa;
                    }
                }
                else
                {
                    n2_out++;
                    if (n2_q > 0)
                    {
                        rc = queue_list_pop(&q2, &tmp, listFree);
                        if (rc)
                            break;
                        n2_q--;
                        t_oa = rnd_double(bor4);
                        oa_type = 2;
                        tt4 += t_oa;
                    }
                    else
                    {
                        rc = queue_list_pop(&q1, &tmp, listFree);
                        if (rc)
                            break;
                        n1_q--;
                        t_oa = rnd_double(bor3);
                        oa_type = 1;
                        tt3 += t_oa;
                    }
                }
            }
        }
        t_do = mn(t1_in, t2_in, t_oa);

        if ((n2_out + n1_out) % 100 == 0 && (n1_out != 0 || n2_out != 0))
        {
            printf("Elements processed: %d\n", n2_out + n1_out);
            printf("Length of the first queue: %d\n", n1_q);
            printf("Average length of the first queue: %.4lf\n", (double) q1_sr / 100.0);
            printf("Length of the second queue: %d\n", n2_q);
            printf("Average length of the second queue: %.4lf\n\n", (double) q2_sr / 100.0);
            q1_sr = q2_sr = 0;
        }
    }

    if (rc == 0)
    {
        double exp1_do, exp1_come;
        double exp2_do, exp2_come;

        exp1_come = (bor1[0] + bor1[1]) / 2 * n1_out;
        exp1_do = (bor3[0] + bor3[1]) / 2 * n1_out;

        exp2_come = (bor2[0] + bor2[1]) / 2 * n2_out;
        exp2_do = (bor4[0] + bor4[1]) / 2 * n2_out;


        printf("Total running time: %.4lf\n", t_all);
        printf("Total downtime: %.4lf\n\n", t_chill);

        printf("Included items of the first type: %d\n", n1_in);
        printf("Processed elements of the first type: %d\n\n", n1_out);

        printf("Included items of the second type: %d\n", n2_in);
        printf("Processed elements of the second type: %d\n\n", n2_out);

        printf("Arrival time in first queue\nFact: %.4lf\nExpected: %.4lf\nError : %.2lf\n", tt1, exp1_come,
               err(tt1, exp1_come));
        printf("Processing time for items from the first queue\nFact: %.4lf\nExpected: %.4lf\nError : %.2lf\n", tt3,
               exp1_do,
               err(tt3, exp1_do));

        printf("Arrival time in second queue\nFact: %.4lf\nExpected: %.4lf\nError : %.2lf\n", tt2, exp2_come,
               err(tt2, exp2_come));
        printf("Processing time for items from the second queue\nFact: %.4lf\nExpected: %.4lf\nError : %.2lf\n", tt4,
               exp2_do,
               err(tt4, exp2_do));
    }
    return rc;
}


int sim_list_strap()
{
    int num = 1000, rc = 1;
    double bor1[] = {1, 5}, bor2[] = {0, 3}, bor3[] = {0, 4}, bor4[] = {0, 1};
    int c = 0;
    while (1)
    {
        printf("1-Change simulation parameters\n2-Simulate\nChoice:");
        if ((scanf("%d", &c) == 1) && c > 0 && c < 3)
            break;
        printf("Wrong choice\n");
    }
    if (c == 1)
        while (rc != 0)
        {
            rc = param_input(&num, bor1, bor2, bor3, bor4);
            printf("Wrong data");
        }
    sl_list_free **pListFree;
    *pListFree = NULL;
    rc = simulation_list(num, bor1, bor2, bor3, bor4, pListFree);
    printf("Show free mem?[y/n]");
    char buf[4];
    memset(buf, 0, sizeof(char) * 4);
    if ((scanf("%s", buf) == 1) && strlen(buf) == 1 && (strcmp("y", buf) == 0 || strcmp("n", buf) == 0))
    {
        if (strcmp("y", buf) == 0)
            print_list_free(*pListFree);
    }
    else
        printf("Wrong choice\n");
    free_list_free(*pListFree);
    return rc;
}

int measure(int n, int iter)
{
    int rc = 0;
    double t1 = 0, t2 = 0;
    size_t m1 = 0, m2 = 0;
    queue_arr queueArr;
    queue_list queueList;
    int tmp = 0;


    clock_t begin = clock();
    for (int i = 0; i < iter; i++)
    {
        queueArr.pin = 0;
        queueArr.pout = 0;
        queueArr.is_not_empty = 0;
        for (int j = 0; j < n; j++)
        {
            rc = queue_arr_push(&queueArr, j);
            if (rc)
                return rc;
        }
        //        print_arr(&queueArr);
        for (int j = 0; j < n; j++)
        {
            rc = queue_arr_pop(&queueArr, &tmp);
            if (rc)
                return rc;
        }
    }
    clock_t end = clock();
    t1 = ((double) (end - begin));
    t1 /= iter;
    //    t1 /= CLOCKS_PER_SEC;
    m1 = sizeof(int) * (ARR_SIZE + 3);


    clock_t begin1 = clock();
    for (int i = 0; i < iter; i++)
    {
        queueList.begin = NULL;
        queueList.end = NULL;
        for (int j = 0; j < n; j++)
        {
            rc = queue_list_push(&queueList, j);
            if (rc)
                return rc;
        }
        //        print_list(&queueList);
        for (int j = 0; j < n; j++)
        {
            rc = queue_list_pop(&queueList, &tmp, NULL);
            if (rc)
                return rc;
        }
    }
    clock_t end1 = clock();
    t2 = ((double) (end1 - begin1));
    t2 /= iter;
    //    t2 /= CLOCKS_PER_SEC;
    m2 += sizeof(sl_list) * (n + 1);


    //    printf("Arr: %Iu\nList: %Iu\n", sizeof(int), sizeof(sl_list));
    printf("Arr: %lf %lu\nList: %lf %lu\n", t1, m1, t2, m2);
    return rc;
}
