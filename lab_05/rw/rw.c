#include "rw.h"

struct sembuf SEM_START_READ[] =
        {
                {CAN_WRITE, 0, 0},
                {CAN_READ, 1, 0},
                {WAITING_WRITERS, 0, 0},
                {ACTIVE_READERS, 1, 0},
                {CAN_READ, -1, 0},
        };

struct sembuf SEM_STOP_READ[] =
        {
                {ACTIVE_READERS, -1, 0},
        };

struct sembuf SEM_START_WRITE[] =
        {
                {WAITING_WRITERS, 1, 0},
                {ACTIVE_READERS, 0, 0},
                {CAN_WRITE, 0, 0},
                {CAN_WRITE, 1, 0},
                {WAITING_WRITERS, -1, 0},
                {BIN_SEM, -1, 0},
        };

struct sembuf SEM_STOP_WRITE[] =
        {
                {CAN_WRITE, -1, 0},
                {BIN_SEM, 1, 0},
        };

int start_read(int s_id)
{
    return semop(s_id, SEM_START_READ, 5) != -1;
}

int end_read(int s_id)
{
    return semop(s_id, SEM_STOP_READ, 1) != -1;
}

int start_write(int s_id)
{
    return semop(s_id, SEM_START_WRITE, 5) != -1;
}

int end_write(int s_id)
{
    return semop(s_id, SEM_STOP_WRITE, 1) != -1;
}

int reader_run(int *const shared_mem, const int s_id, const int reader_id)
{
    int sleep_time;
    if (!shared_mem)
    {
        return 1;
    }
    srand(time(NULL) + reader_id);
    for (size_t i = 0; i < N; i++)
    {
        sleep_time = rand() % 7 + 1;
        sleep(sleep_time);

        if (!start_read(s_id))
        {
            perror("start_read error");
            exit(1);
        }

        int val = *shared_mem;
        printf("Reader %d  read: %2d\n", reader_id, val);

        if (!end_read(s_id))
        {
            perror("end_read error");
            exit(1);
        }
    }
    return 0;
}

int writer_run(int *const shared_mem, const int s_id, const int writer_id)
{
    int sleep_time;
    if (!shared_mem)
    {
        return 1;
    }
    srand(time(NULL) + writer_id);

    for (size_t i = 0; i < N; i++)
    {
        sleep_time = rand() % 3 + 1;
        sleep(sleep_time);

        if (!start_write(s_id))
        {
            perror("start_write error");
            exit(1);
        }

        int val = ++(*shared_mem);
        printf("Writer %d write: %2d\n", writer_id, val);

        if (!end_write(s_id))
        {
            perror("end_write error");
            exit(1);
        }
    }
    return 0;
}


