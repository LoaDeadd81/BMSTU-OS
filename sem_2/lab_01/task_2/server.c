#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>

#define SOCK_NAME "mysock.s"
#define BUF_SIZE 64

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
    struct sockaddr_un server_addr, client_addr;
    int size, client_len = sizeof(client_addr);
    char buf[BUF_SIZE] = "", msg[BUF_SIZE] = "";

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed");
        unlink(SOCK_NAME);
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_NAME);
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close_sock();
        exit(1);
    }

    signal(SIGINT, sig_handler);

    while (1)
    {
        size = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &client_addr, (socklen_t *) &client_len);
        if (size <= 0)
        {
            perror("recvfrom failed");
            close_sock();
            exit(1);
        }
        buf[size] = '\0';
        printf("Server read: %s\n", buf);

        sprintf(msg, "\tServer pid: %d\n", getpid());
        strcat(buf, msg);
        if(sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0)
        {
            perror("sendto failed");
            close_sock();
            exit(1);
        }
        printf("Server send: %s\n", buf);
        memset(buf, 0, BUF_SIZE);
    }

    close_sock();
    return 0;
}