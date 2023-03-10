#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>

#include "config.h"

int sockd;

void sig_handler(int signum)
{
    if (close(sockd) < 0)
        perror("close failed");
    exit(0);
}

int main(void)
{
    struct sockaddr_in addr;
    char buf[BUF_SIZE] = "", msg[BUF_SIZE] = "";
    int size;
    int clients[W_NUM] = {0};
    struct timeval timeout;

    if ((sockd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket() failed");
        exit(1);
    }

    if (fcntl(sockd, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl failed");
        close(sockd);
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SOCK_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        perror("bind failed");
        close(sockd);
        exit(1);
    }

    signal(SIGTSTP, sig_handler);

    listen(sockd, W_NUM);

    timeout.tv_sec = 15;
    timeout.tv_usec = 0;

    while (1)
    {
        fd_set client_set;
        FD_ZERO(&client_set);
        FD_SET(sockd, &client_set);

        for (int i = 0; i < W_NUM; i++)
            FD_SET(clients[i], &client_set);

        int mx = 0;
        for (int i = 0; i < W_NUM; i++)
            if (clients[i] > mx)
                mx = clients[i];
        if (sockd > mx)
            mx = sockd;

        if (select(mx + 1, &client_set, NULL, NULL, &timeout) < 0)
        {
            perror("select failed");
            close(sockd);
            exit(1);
        }

        if (FD_ISSET(sockd, &client_set))
        {
            int sock = accept(sockd, NULL, NULL);
            if (sock == -1)
            {
                perror("accept failed");
                close(sockd);
                exit(1);
            }
            fcntl(sock, F_SETFL, O_NONBLOCK);

            int i = 0;
            for (; i < W_NUM && clients[i] != 0; i++);
                clients[i] = sock;
        }

        for (int i = 0; i < W_NUM; i++)
        {
            if (FD_ISSET(clients[i], &client_set))
            {
                size = recv(clients[i], buf, sizeof(buf), 0);
                if (size <= 0)
                {
                    close(clients[i]);
                    clients[i] = 0;
//                    perror("recvfrom() failed");
                }
                else
                {
                    buf[size] = '\0';
                    printf("Server read: %s\n", buf);

                    sprintf(msg, "\tResponse from the server with pid %d\n", getpid());
                    strcat(buf, msg);
                    if (send(clients[i], buf, strlen(buf) + 1, 0) < 0)
                    {
                        perror("send failed");
                        close(clients[i]);
                        exit(1);
                    }
                    printf("Server send: %s\n", buf);
                }
                memset(buf, 0, BUF_SIZE);
            }
        }

    }
    close(sockd);
    return EXIT_SUCCESS;
}