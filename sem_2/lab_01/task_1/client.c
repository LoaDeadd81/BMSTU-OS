#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SOCK_NAME "mysock.s"
#define BUF_SIZE 32

int main()
{
    int sockfd;
    struct sockaddr_un server_addr;
    char buf[BUF_SIZE] = "";

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed");
        exit(1);
    }

    sprintf(buf, "pid: %d", getpid());

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_NAME);

    if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("send failed");
        close(sockfd);
        exit(1);
    }

    printf("Client send: %s\n", buf);

    close(sockfd);
    return 0;
}