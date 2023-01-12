/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "bakery.h"

struct thread_arg_t
{
    int num;
    int res;
    int id;
};

time_t raw_time;
struct tm *timeinfo;

int choosing[30] = {0};
int number[30] = {0};
int symbol = 'a';
int num = 0;
int idx_thread_create = 0;
int idx_thread_join = 0;

pthread_t workers[30];

struct thread_arg_t threads_results[30] = {0};

void *
get_number(void *arg)
{
    struct thread_arg_t *targ = arg;
    int i = num;
    num++;
    targ->id = i;

    choosing[i] = 1;
    int max = 0;
    for (int j = 0; j < 30; j++)
        if (number[j] > max)
            max = number[j];
    number[i] = max + 1;
    targ->num = number[i];
    choosing[i] = 0;

    return 0;
}

void *
bakery(void *arg)
{
    struct thread_arg_t *targ = arg;
    setbuf(stdout, NULL);
    time(&raw_time);
    timeinfo = localtime(&raw_time);
    printf("Thread id = '%ld' started, client's number = %d, time = %s", syscall(SYS_gettid), number[targ->id],
           asctime(timeinfo));

    int i = targ->id;
    for (int j = 0; j < 30; j++)
    {
        while (choosing[j]);
        while ((number[j] > 0) && (number[j] < number[i] || (number[j] == number[i] && j < i)));
    }
    targ->res = symbol;
    symbol++;

    sleep(3);
    time(&raw_time);
    timeinfo = localtime(&raw_time);
    printf("Thread id = '%ld' stopped, client's number = %d, time = %s", syscall(SYS_gettid), number[i],
           asctime(timeinfo));
    number[i] = 0;

    return 0;
}

struct BAKERY *
get_number_1_svc(struct BAKERY *argp, struct svc_req *rqstp)
{
    setbuf(stdout, NULL);
    static struct BAKERY result;

    struct thread_arg_t tres;
    get_number(&tres);
    result.pid = tres.id;
    result.num = tres.num;

    return &result;
}

struct BAKERY *
wait_queue_1_svc(struct BAKERY *argp, struct svc_req *rqstp)
{
    setbuf(stdout, NULL);
    static struct BAKERY result;

    pthread_t thread;

    threads_results[idx_thread_create].id = argp->pid;
    pthread_create(&thread, NULL, bakery, &threads_results[idx_thread_create]);
    workers[idx_thread_create] = thread;
    idx_thread_create++;

    return &result;
}

struct BAKERY *
bakery_res_1_svc(struct BAKERY *argp, struct svc_req *rqstp)
{
    setbuf(stdout, NULL);
    static struct BAKERY result;

    pthread_join(workers[idx_thread_join], NULL);
    result.pid = argp->pid;
    result.result = threads_results[idx_thread_join].res;
    idx_thread_join++;

    return &result;
}
