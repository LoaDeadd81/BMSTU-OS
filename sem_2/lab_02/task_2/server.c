#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>

#include "config.h"

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
    struct sockaddr_un addr, client;
    int size, client_len = sizeof(client);
    char buf[BUF_SIZE] = "", msg[BUF_SIZE] = "";

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed");
        unlink(SOCK_NAME);
        exit(1);
    }

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_NAME);
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        perror("bind failed");
        close_sock();
        exit(1);
    }

    signal(SIGINT, sig_handler);

    printf("Server started listening\n");
    while (1)
    {
        size = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &client, (socklen_t *) &client_len);
        if (size <= 0)
        {
            perror("recvfrom failed");
            close_sock();
            exit(1);
        }
        buf[size] = '\0';
        printf("Server read: %s\n", buf);

        sprintf(msg, "\tResponse from the server with pid %d\n", getpid());
        strcat(buf, msg);
        if(sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &client, sizeof(client)) < 0)
        {
            perror("sendto failed");
            close_sock();
            exit(1);
        }
        printf("Server send: %s\n", buf);
        memset(buf, 0, BUF_SIZE);
    }

    printf("Server stopped\n");
    close_sock();
    return 0;
}