#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "config.h"

int main()
{
    int sockfd, size;
    struct sockaddr_in addr;
    char recv_msg[BUF_SIZE] = "", send_msg[BUF_SIZE] = "";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SOCK_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        perror("connect failed");
        close(sockfd);
        exit(1);
    }

    sprintf(send_msg, "pid: %d", getpid());
    if (send(sockfd, send_msg, sizeof(send_msg), 0) < 0)
    {
        perror("send failed");
        close(sockfd);
        exit(1);
    }
    printf("Client send: %s\n", send_msg);


    memset(recv_msg, 0, BUF_SIZE);
    size = recv(sockfd, recv_msg, sizeof(recv_msg), 0);
    if (size <= 0)
    {
        perror("recv failed");
        close(sockfd);
        exit(1);
    }
    recv_msg[size] = '\0';
    printf("Client read: %s\n", recv_msg);

    close(sockfd);
    return 0;
}