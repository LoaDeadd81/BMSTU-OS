#include "pc.h"

struct sembuf producer_lock[2] =
        {
                {BUF_EMPTY, -1, 0},
                {BIN_SEM, -1, 0}
        };

struct sembuf producer_release[2] =
        {
                {BIN_SEM, 1, 0},
                {BUF_FULL, 1, 0}
        };

struct sembuf consumer_lock[2] =
        {
                {BUF_FULL, -1, 0},
                {BIN_SEM, -1, 0}
        };

struct sembuf consumer_release[2] =
        {
                {BIN_SEM, 1, 0},
                {BUF_EMPTY, 1, 0}
        };

void run_producer(char **ptr, char *ch, const int sid, const int pdid)
{
    srand(time(NULL) + pdid);
    int sleep_time = rand() % MAX_PRODUCER_DELAY_TIME + 1;
    sleep(sleep_time);

    if (semop(sid, producer_lock, 2) == -1)
    {
        perror("producer lock error");
        exit(1);
    }

    (*ch)++;
    (*ptr)++;
    **ptr = *ch;

    printf("Producer %d write: %c\n", pdid, *ch);

    if (semop(sid, producer_release, 2) == -1)
    {
        perror("procucer release error");
        exit(1);
    }
}

void create_producer(char **ptr, char *ch, const int sid, const int pdid)
{
    pid_t childpid;
    if ((childpid = fork()) == -1)
    {
        perror("fork producer error");
        exit(1);
    }
    else if (childpid == 0)
    {
        for (int i = 0; i < ITERS_NUM; i++)
            run_producer(ptr, ch, sid, pdid);

        exit(0);
    }
}

void run_consumer(char **ptr, const int sid, const int cid)
{
    srand(time(NULL) + cid);
    int sleep_time = rand() % MAX_CONSUMER_DELAY_TIME + 1;
    sleep(sleep_time);

    if (semop(sid, consumer_lock, 2) == -1)
    {
        perror("consumer lock error");
        exit(1);
    }

    char ch = **ptr;
    (*ptr)++;

    printf("Consumer %d  read: %c\n", cid, ch);

    if (semop(sid, consumer_release, 2) == -1)
    {
        perror("consumer release error");
        exit(1);
    }
}

void create_consumer(char **ptr, const int sid, const int cid)
{
    pid_t childpid;
    if ((childpid = fork()) == -1)
    {
        perror("fork error");
        exit(1);
    }

    if (childpid == 0)
    {
        for (int i = 0; i < ITERS_NUM; i++)
            run_consumer(ptr, sid, cid);

        exit(0);
    }
}
