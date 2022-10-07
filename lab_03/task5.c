#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <string.h>

int flag = 1;

void no_write(int sig_numb)
{ flag = 0; }

int main()
{
    signal(SIGINT, 1);
    int pid, buf_size = 4;
    int fd[2];
    char mes[2][10] = {"mes1", "mes2"};
    char buf[20] = {};

    printf("Parent: pid=%d, gid=%d\n", getpid(), getpgrp());
    if (pipe(fd) == 1)
    {
        printf("Can't pipe");
        return 1;
    }

    for (size_t i = 0; i < 2; ++i)
    {
        pid = fork();
        if (pid == -1)
            perror("Can’t fork\n");
        else if (pid == 0)
        {
            signal(SIGINT, no_write);
            sleep(2);
            if(flag)
            {
                close(fd[0]);
                write(fd[1], mes[i], 4);
            }
            printf("Child : pid=%d, gid=%d, ppid=%d\n", getpid(), getpgrp(), getppid());
            return 0;
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        close(fd[1]);
        int rc = read(fd[0], buf, 4);
        if (rc == -1)
            printf("Read error: %s\n", strerror(errno));
        else
            printf("Message: %s\n", buf);

        pid_t exit_child;
        int status;
        exit_child = wait(&status);
        printf("Child finished: cpid=%d\n", exit_child);
        if (WIFEXITED(status))
            printf("Child exited with code %d\n", WEXITSTATUS(status));
        else
            printf("Child terminated abnormal\n");
    }

    return 0;
}
