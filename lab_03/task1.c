#include <stdio.h>
#include <unistd.h>

int main(void)
{
    pid_t childpid[2];

    for (size_t i = 0; i < 2; ++i)
    {
        childpid[i] = fork();
        if (childpid[i] == -1)
        {
            perror("Can’t fork\n");
            return 1;
        }
        else if (childpid[i] == 0)
        {
            printf("Child: pid=%d, gpid=%d, ppid=%d\n", getpid(), getpgrp(), getppid());
            sleep(2);
            printf("Child after sleep: pid=%d, gpid=%d, ppid=%d\n", getpid(), getpgrp(), getppid());
            return 0;
        }
        else printf("Parent: pid=%d, gpid=%d, child pid=%d\n", getpid(), getpgrp(), childpid[i]);
    }

    return 0;
}