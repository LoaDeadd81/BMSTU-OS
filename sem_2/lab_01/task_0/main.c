#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 32

int main()
{
    int socket[2];
    int pid;
    char buf[BUF_SIZE] = "";
    char parent_str[] = "pppppppppp";
    char str[][BUF_SIZE] = {"aaaaaaaaaa", "bbbbbbbbbb", "cccccccccc"};

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, socket) == -1)
    {
        perror("socketpair() eror\n");
        exit(1);
    }

    for (int i = 0; i < 3; i++)
    {
        if ((pid = fork()) == -1)
        {
            perror("fork() error\n");
            exit(1);
        }
        else if (pid == 0)
        {
//            close(socket[0]);
            buf[0] = '\0';
            read(socket[1], buf, sizeof(buf));
            printf("Child #%d read: %s\n", getpid(), buf);
            write(socket[1], str[i], sizeof(str[i]));
            printf("Child #%d write: %s\n", getpid(), str[i]);
//            close(socket[1]);
            return 0;
        }
    }

    sleep(3);
//    close(socket[1]);
    for (int i = 0; i < 3; i++)
    {
        write(socket[0], parent_str, sizeof(parent_str));
        printf("Parent write: %s\n", parent_str);
        buf[0] = '\0';
        read(socket[0], buf, sizeof(buf));
        printf("Parent read: %s\n", buf);
    }

//    close(socket[0]);
    return 0;
}