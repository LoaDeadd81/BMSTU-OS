#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SOCK_NAME "mysock.s"
#define BUF_SIZE 64

int sockfd;
char sock_name[32] = "";

void close_sock()
{
    if (close(sockfd) < 0)
        perror("close failed");
//    if (unlink(sock_name) < 0)
//        perror("unlink failed");
}

int main()
{
    int size;
    struct sockaddr_un client_addr, server_addr;
    char recv_msg[BUF_SIZE] = "", send_msg[BUF_SIZE] = "";

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed");
        exit(1);
    }

    sprintf(sock_name, "sock%d.s", getpid());

    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, sock_name);
    if (bind(sockfd, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0)
    {
        perror("connect failed");
        close_sock();
        exit(1);
    }
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_NAME);


    sprintf(send_msg, "pid: %d", getpid());
    if (sendto(sockfd, send_msg, sizeof(send_msg), 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("sendto failed");
        close_sock();
        exit(1);
    }
    printf("Client send: %s\n", send_msg);

    memset(recv_msg, 0, BUF_SIZE);
    size = recvfrom(sockfd, recv_msg, sizeof(recv_msg), 0, NULL, NULL);
    if (size <= 0)
    {
        perror("recv failed");
        close_sock();
        exit(1);
    }
    recv_msg[size] = '\0';
    printf("Client read: %s\n", recv_msg);

    close_sock();
    return 0;
}