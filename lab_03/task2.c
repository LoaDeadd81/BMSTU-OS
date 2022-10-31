#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <errno.h>


int main(void)
{
    pid_t childpid[2];
    int wait_status;

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
            printf("Child : pid=%d, gpid=%d, ppid=%d\n", getpid(), getpgrp(), getppid());
            return 0;
        }
        else printf("Parent: pid=%d, gpid=%d, child pid=%d\n", getpid(), getpgrp(), childpid[i]);
    }

    for (int i = 0; i < 2; ++i)
    {
        childpid[i] = waitpid(childpid[i], &wait_status, 0);

        if (childpid[i] == -1)
        {
            printf("Wait error: %s\n", strerror(errno));
            return 1;
        }

        printf("Child finished: pid=%d\n", childpid[i]);
        if (WIFEXITED(wait_status))
            printf("Child exited with code %d\n", WEXITSTATUS(wait_status));
        else if (WIFSIGNALED(wait_status))
            printf("Child process terminated by non-intercepted interrupt\nSignal number: %d\n", WTERMSIG(wait_status));
        else if (WIFSTOPPED(wait_status))
            printf("Child process has stopped\nSignal number: %d\n", WSTOPSIG(wait_status));
    }

    return 0;
}
