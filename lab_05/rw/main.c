#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

#include "rw.h"


int main()
{
    setbuf(stdout, NULL);
    int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd = shmget(IPC_PRIVATE, sizeof(int), perms | IPC_CREAT);
    if (fd == -1)
    {
        perror("shmget failed");
        exit(1);
    }

    int *shared_mem = shmat(fd, 0, 0);
    if (shared_mem == (void *) -1)
    {
        perror("shmat error");
        exit(1);
    }

    int s_id = semget(IPC_PRIVATE, 5, perms | IPC_CREAT);
    if (s_id == -1)
    {
        perror("semget error");
        exit(1);
    }

    if (semctl(s_id, ACTIVE_READERS, SETVAL, 0) == -1 ||
        semctl(s_id, CAN_WRITE, SETVAL, 0) == -1 ||
        semctl(s_id, WAITING_WRITERS, SETVAL, 0) == -1 ||
        semctl(s_id, CAN_READ, SETVAL, 0) == -1 ||
        semctl(s_id, BIN_SEM, SETVAL, 1) == -1)
    {
        perror("ssemctl error");
        exit(1);
    }

    for (size_t i = 0; i < N_WRITERS; ++i)
    {
        int child_pid = fork();
        if (child_pid == -1)
        {
            perror("fork writer error");
            exit(1);
        }
        else if (child_pid == 0)
        {
            writer_run(shared_mem, s_id, i);
            exit(0);
        }
    }

    for (size_t i = 0; i < N_READERS; i++)
    {
        int child_pid = fork();
        if (child_pid == -1)
        {
            perror("fork reader error");
            exit(1);
        }
        else if (child_pid == 0)
        {
            reader_run(shared_mem, s_id, i);
            exit(0);
        }
    }

    for (size_t i = 0; i < N_WRITERS + N_READERS; i++)
    {
        int ch_status;
        int child_pid = wait(&ch_status);

        if (child_pid == -1)
        {
            perror("wait error");
            exit(1);
        }
        if (!WIFEXITED(ch_status))
            fprintf(stderr, "Child process %d terminated abnormally", child_pid);
        if (shmdt(shared_mem) == -1)
        {
            perror("shmdt error");
            exit(1);
        }
    }

    struct shmid_ds shm;
    shmctl(fd, IPC_STAT, &shm);
    printf("\n\n\n\n%d\n\n", shm.shm_nattch);

    if (shmdt(shared_mem) == -1)
    {
        perror("shmdt error");
        exit(1);
    }

    if (shmctl(fd, IPC_RMID, NULL) == -1)
    {
        perror("shmctl with command IPC_RMID error");
        exit(1);
    }

    if (semctl(s_id, 0, IPC_RMID) == -1)
    {
        perror("semctl with command IPC_RMID error");
        exit(1);
    }
    return 0;
}
