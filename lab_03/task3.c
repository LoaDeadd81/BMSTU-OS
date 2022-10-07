#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <string.h>

int main()
{
    int pid;
    const char path[2][100] = {"/home/loadeadd/Рабочий стол/study/BMSTU-OS/lab_03/stack.out", "/home/loadeadd/Рабочий стол/study/BMSTU-OS/lab_03/queue.out"};

    printf("Parent: pid=%d, gid=%d\n", getpid(), getpgrp());

    for (size_t i = 0; i < 2; ++i)
    {
        pid = fork();
        if (pid == -1)
            perror("Can’t fork\n");
        else if (pid == 0)
        {
            printf("Child : pid=%d, gid=%d, ppid=%d\n", getpid(), getpgrp(), getppid());
            int status = execl(path[i], 0);
            if(status == -1)
            {
                printf("Can't exec\n%s : %s\n", path[i], strerror(errno));
                return 1;
            }
            else
                return 0;
        }
    }

    for (int i = 0; i < 2; ++i)
    {
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
