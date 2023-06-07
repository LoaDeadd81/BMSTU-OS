#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>

#define SOCK_NAME "mysock.s"
#define BUF_SIZE 32

int sockfd;

void close_sock()
{
    if (close(sockfd) < 0)
    {
        perror("close failed");
        return;
    }
    if (unlink(SOCK_NAME) < 0)
        perror("unlink failed");
}

void sig_handler(int signum)
{
    close_sock();
    exit(0);
}

int main()
{
    struct sockaddr_un server_addr;
    char buf[BUF_SIZE] = "";

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed");
        unlink(SOCK_NAME);
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_NAME);
    if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        unlink(SOCK_NAME);
        close(sockfd);
        exit(1);
    }

    signal(SIGINT, sig_handler);

    while(1)
    {
        int size = recv(sockfd, buf, sizeof(buf), 0);
        if (size <= 0)
        {
            perror("recv failed");
            close_sock();
            exit(1);
        }
        buf[size] = '\0';
        printf("Server read: %s\n", buf);
    }

    close_sock();
    return 0;
}