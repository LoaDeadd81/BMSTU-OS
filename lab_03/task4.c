#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <string.h>

int main()
{
    pid_t childpid[2], ended_child;
    int fd[2], wait_status;
    char mes[2][20] = {"message", "another message"};

    if (pipe(fd) == -1)
    {
        printf("Can't pipe");
        return 1;
    }

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
            close(fd[0]);
            write(fd[1], mes[i], sizeof(mes[i]));
            printf("Child : pid=%d, gpid=%d, ppid=%d\n", getpid(), getpgrp(), getppid());
            return 0;
        }
        else printf("Parent: pid=%d, gpid=%d, child pid=%d\n", getpid(), getpgrp(), childpid[i]);
    }

    for (int i = 0; i < 2; ++i)
    {
        close(fd[1]);
        int rc = read(fd[0], mes[i], sizeof(mes[i]));
        if (rc == -1)
            printf("Read error: %s\n", strerror(errno));
        else
            printf("Message%d: %s\n", i + 1, mes[i]);

        ended_child = wait(&wait_status);

        if (ended_child == -1)
            printf("Wait error: %s\n", strerror(errno));

        printf("Child finished: pid=%d\n", ended_child);
        if (WIFEXITED(wait_status))
            printf("Child exited with code %d\n", WEXITSTATUS(wait_status));
        else if (WIFSIGNALED(wait_status))
            printf("Child process terminated by non-intercepted interrupt\nSignal number: %d\n", WTERMSIG(wait_status));
        else if (WIFSTOPPED(wait_status))
            printf("Child process has stopped\nSignal number: %d\n", WSTOPSIG(wait_status));
    }

    return 0;
}
