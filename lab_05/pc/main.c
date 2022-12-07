#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>
#include <unistd.h>
#include <sys/stat.h>

#include "pc.h"

int main(void)
{
    setbuf(stdout, NULL);
    int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int shmid = shmget(101, 2 * sizeof(char *) + sizeof(char) + sizeof(char) * N, IPC_CREAT | perms);
    if (shmid == -1)
    {
        perror("shmget error");
        exit(1);
    }

    char *addr = shmat(shmid, 0, 0);
    if (addr == (char *) -1)
    {
        perror("shmat error");
        exit(1);
    }

    char **prod_ptr = (char **) addr;
    char **cons_ptr = (char **) addr + 1;
    *prod_ptr = addr + 2 * sizeof(char *);
    *cons_ptr = addr + 2 * sizeof(char *) + sizeof(char);

    char *ch = addr + 2 * sizeof(char *);
    *ch = 'a' - 1;

    int sem_fd = semget(10, 3, IPC_CREAT | perms);
    if (sem_fd == -1)
    {
        perror("semget error");
        exit(1);
    }

    if (semctl(sem_fd, BIN_SEM, SETVAL, 1) == -1 ||
        semctl(sem_fd, BUF_FULL, SETVAL, 0) == -1 ||
        semctl(sem_fd, BUF_EMPTY, SETVAL, N) == -1)
    {
        perror("semctl error");
        exit(1);
    }

    for (int i = 0; i < PROD_NUM; i++)
        create_producer(prod_ptr, ch, sem_fd, i + 1);

    for (int i = 0; i < CONS_NUM; i++)
        create_consumer(cons_ptr, sem_fd, i + 1);

    for (size_t i = 0; i < PROD_NUM + CONS_NUM; i++)
    {
        int ch_status;
        int child_pid = wait(&ch_status);

        if (child_pid == -1)
        {
            perror("wait error");
            exit(1);
        }
        if (!WIFEXITED(ch_status))
        {
            fprintf(stderr, "Child process %d terminated abnormally", child_pid);
        }
    }

    if (shmdt(addr) == -1)
    {
        perror("shmdt error");
        exit(1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl with command IPC_RMID error");
        exit(1);
    }
    if (semctl(sem_fd, 0, IPC_RMID) == -1)
    {
        perror("semctl with command IPC_RMID error");
        exit(1);
    }

    return 0;
}
