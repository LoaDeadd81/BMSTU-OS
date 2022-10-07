#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int pid;

    printf("Parent: pid=%d, gid=%d\n", getpid(), getpgrp());

    for (size_t i = 0; i < 2; ++i)
    {
        pid = fork();
        if(pid == -1)
            perror("Can’t fork\n");
        else if(pid == 0)
        {
            printf("Child : pid=%d, gid=%d, ppid=%d\n", getpid(), getpgrp(), getppid());
            sleep(2);
            printf("Child after sleep: pid=%d, gid=%d, ppid=%d\n", getpid(), getpgrp(), getppid());
            return 0;
        }
    }

    return 0;
}